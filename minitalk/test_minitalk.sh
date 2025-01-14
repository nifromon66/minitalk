#!/bin/bash

# Enhanced test script for Minitalk project
# Assumes the server executable is ./server and the client executable is ./client

# Colors for output
green="\033[0;32m"
red="\033[0;31m"
yellow="\033[0;33m"
reset="\033[0m"

# success variables
success_1=1
success_2=0
success_3=0
success_4=0

# Start the server in the background
./server &
SERVER_PID=$!
echo -e "${green}Server started with PID ${SERVER_PID}${reset}"

# Allow the server to initialize
sleep 2

# Define test messages
short_message="Hello"
medium_message="This is a medium-length message for testing."
long_message="$(head -c 1000 < /dev/urandom | tr -dc 'a-zA-Z0-9')"
very_long_message="$(head -c 10000 < /dev/urandom | tr -dc 'a-zA-Z0-9')"

# Function to send a message from a client and check result
test_message() {
  local message="$1"
  local label="$2"

  ./client "$SERVER_PID" "$message" > /tmp/phase1_client_output 2>&1
  sleep 0.75

  # removing colors codes
  clean_output=$(sed 's/\x1b\[[0-9;]*m//g' < /tmp/phase1_client_output)

  if echo "$clean_output" | grep -q "^Server confirmed message reception\.\.\." ; then
    echo -e "${green}[PASS] ${label}${reset}"
  elif echo "$clean_output" | grep -q "^ERROR" ; then
    echo -e "${red}[FAIL] ${label}${reset}"
    cat /tmp/phase1_client_output
    success_1=0
  else
    echo -e "${yellow}[UNKNOWN] ${label} - Unexpected output${reset}"
    cat /tmp/phase1_client_output
    success_1=0
  fi
}

# Enhanced function to test if a client finishes successsfully and tracks waiting line
test_client_waiting_line() {
  local client_id="$1"
  local message="$2"
  local label="$3"
  local phase="$4"

  sleep 0.1
  (
    ./client "$SERVER_PID" "$message" > /tmp/${phase}_client_output_${client_id} 2>&1
    clean_output=$(sed 's/\x1b\[[0-9;]*m//g' < /tmp/${phase}_client_output_${client_id})
    if echo "$clean_output" | grep -q "^Server confirmed message reception\.\.\." ; then
      echo "done_${client_id}" >> /tmp/${phase}_clients_done
    elif echo "$clean_output" | grep -q "^ERROR" /tmp/${phase}_client_output_${client_id}; then
      echo "fail_${client_id}" >> /tmp/${phase}_clients_fail
      cat /tmp/${phase}_client_output_${client_id} >> /tmp/${phase}_clients_fail
    else
      echo "unknow_${client_id}" >> /tmp/${phase}_clients_unknown
      cat /tmp/${phase}_client_output_${client_id} >> /tmp/${phase}_clients_unknown
    fi
  ) &
}

# Phase 1: Test single client with various message lengths
echo -e "${yellow}Phase 1: Single client tests with various message lengths${reset}"
test_message "$short_message" "Short message"
sleep 1
test_message "$medium_message" "Medium message"
sleep 2
test_message "$long_message" "Long message"
sleep 3
test_message "$very_long_message" "Very long message"
sleep 4

if [ $success_1 -eq 1 ]; then
  echo -e "${green}[PASS] All single clients completed.${reset}"
else
  echo -e "${red}[FAIL] Some clients did not complete.${reset}"
fi

# Phase 2: Test multiple clients sending messages simultaneously
echo -e "${yellow}Phase 2: Testing simultaneous clients${reset}"
> /tmp/phase2_clients_done
> /tmp/phase2_clients_fail
> /tmp/phase2_clients_unknown
for i in {1..10}; do
    sleep 0.1
    test_client_waiting_line "$i" "$medium_message Simultaneous message from client $i" "Simultaneous client $i" "phase2"
done

sleep 25
# Validate that all clients finished successsfully
if [ $(wc -l < /tmp/phase2_clients_done) -eq 10 ]; then
  echo -e "${green}[PASS] All simultaneous clients completed.${reset}"
  success_2=1
fi
if [ $(wc -l < /tmp/phase4_clients_fail) -eq 1 ]; then
  echo -e "${red}[FAIL] Some clients did not complete.${reset}"
  echo -e "Failed clients:"
  cat /tmp/phase2_clients_fail
fi
if [ $(wc -l < /tmp/phase4_clients_unknown) -eq 1 ]; then
  echo -e "${yellow}[UNKNOWN]${reset}"
  echo -e "clients with unknown errors:"
  cat /tmp/phase2_clients_unknown
  success_2=0
fi

sleep 5
# Phase 3: Stress test with 100 clients
echo -e "${yellow}Phase 3: Stress test with 100 clients${reset}"
sleep 20
> /tmp/phase3_clients_done
> /tmp/phase3_clients_fail
> /tmp/phase3_clients_unknown
for i in {1..100}; do
  random_message="$(head -c 500 < /dev/urandom | tr -dc 'a-zA-Z0-9')"
  test_client_waiting_line "$i" "$random_message" "Stress client $i" "phase3"
  sleep 0.2  # Small delay to simulate staggered client arrivals and waiting line
done

sleep 300
# Validate that all clients finished successsfully in stress test
if [ $(wc -l < /tmp/phase3_clients_done) -eq 100 ]; then
  echo -e "${green}[PASS] All stress test clients completed.${reset}"
  success_3=1
fi
if [ $(wc -l < /tmp/phase4_clients_fail) -eq 1 ]; then
  echo -e "${red}[FAIL] Some clients did not complete.${reset}"
  echo -e "Failed clients:"
  cat /tmp/phase3_clients_fail
  success_3=0
fi
if [ $(wc -l < /tmp/phase4_clients_unknown) -eq 1 ]; then
  echo -e "${yellow}[UNKNOWN]${reset}"
  echo -e "clients with unknown errors:"
  cat /tmp/phase3_clients_unknown
  success_3=0
fi

# Phase 4: Extreme stress test
echo -e "${yellow}Phase 4: Extreme stress test for 10 seconds${reset}"
sleep 5
> /tmp/phase4_clients_done
> /tmp/phase4_clients_fail
> /tmp/phase4_clients_unknown
end_time=$((SECONDS + 10))
client_id=1
while [ $SECONDS -lt $end_time ]; do
  random_message="$(head -c 200 < /dev/urandom | tr -dc 'a-zA-Z0-9')"
  test_client_waiting_line "$client_id" "$random_message" "Extreme stress client $client_id" "phase4"
  sleep 0.1  # Small delay to ensure the server queues clients correctly
  client_id=$((client_id + 1))
done

sleep 50

# Validate that all clients finished successsfully in extreme stress test
if [ $(wc -l < /tmp/phase4_clients_done) -eq $((client_id - 1)) ]; then
  echo -e "${green}[PASS] All extreme stress test clients completed.${reset}"
  success_4=1
fi
if [ $(wc -l < /tmp/phase4_clients_fail) -eq 1 ]; then
  echo -e "${red}[FAIL] Some clients did not complete.${reset}"
  echo -e "Failed clients:"
  cat /tmp/phase4_clients_fail
  success_4=0
fi
if [ $(wc -l < /tmp/phase4_clients_unknown) -eq 1 ]; then
  echo -e "${yellow}[UNKNOWN]${reset}"
  echo -e "clients with unknown errors:"
  cat /tmp/phase4_clients_unknown
  success_4=0
fi

if [ $success_1 -eq 1 ] && [ $success_2 -eq 1 ] && [ $success_3 -eq 1 ] && [ $success_4 -eq 1 ]; then
  echo -e "${green}[SUCCESS] Congratulations. All tests passed successfully. :)${reset}"
else
  echo -e "${red}[FAILED] Too bad. Some test failed. :(${reset}"
fi

# Clean up
kill "-SIGINT" "$SERVER_PID"
sleep 1  # Give the server some time to shut down

# If the server is still running, force termination
if ps -p "$SERVER_PID" > /dev/null; then
  echo -e "${red}Server did not terminate. Forcing termination...${reset}"
  kill -9 "$SERVER_PID"
fi

echo -e "${green}Server process terminated.${reset}"


