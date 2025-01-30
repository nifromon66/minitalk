#!/bin/bash

# Enhanced test script for Minitalk project
# Assumes the server executable is ./server and the client executable is ./client_bonus

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
success_unicode=0
success_valgrind=1
success_diff=1

# Start the server in the background
./server_bonus &> server_output.log &
SERVER_PID=$!
echo -e "${green}Server started with PID ${SERVER_PID}${reset}"

# Allow the server to initialize
sleep 2

# Define test messages
short_message="Hello"
medium_message="This is a medium-length message for testing."
long_message="$(head -c 1000 < /dev/urandom | tr -dc 'a-zA-Z0-9')"
very_long_message="$(head -c 10000 < /dev/urandom | tr -dc 'a-zA-Z0-9')"
extremely_long_message="$(head -c 100000 < /dev/urandom | tr -dc 'a-zA-Z0-9')"
unicode_message="你好，世界! Привет, мир! Hello, world! 🌍🌟"

# Function to send a message from a client and check result
test_message() {
  local message="$1"
  local label="$2"

  if [ "$label" = "Extremely long message - 100 000" ]; then
    sleep 2
  fi
  ./client_bonus "$SERVER_PID" "$message" > /tmp/phase1_client_output 2>&1
  sleep 1
  if [ "$label" = "Extremely long message - 100 000" ]; then
    sleep 10
  fi

  # removing color codes
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

  if [ "$phase" = "phase3" ]; then
    sleep 0.2
  else
    sleep 0.1
  fi
  (
    ./client_bonus "$SERVER_PID" "$message" > /tmp/${phase}_client_output_${client_id} 2>&1
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
  if [ "$phase" = "phase3" ]; then
    sleep 0.05
  else
    sleep 0.01
  fi
}

# Validate server output matches client message
validate_server_output() {
  local message="$1"
  local label="$2"

  ./client_bonus "$SERVER_PID" "$message" > /tmp/validate_client_output 2>&1
  sleep 0.75

  server_log=$(cat valgrind_server_output.log)

  if echo "$server_log" | grep -q "$message"; then
    echo -e "${green}[PASS] ${label} - Server output matches message${reset}"
  else
    echo -e "${red}[FAIL] ${label} - Server output mismatch${reset}"
    echo "Expected: $message"
    echo "Server log: $server_log"
    success_diff=0
  fi
}

# Mandatory part
echo -e "${red}[---------------------------]${yellow}MANDATORY${red}[---------------------------]${reset}"
# Phase 1: Test single client with various message lengths
echo -e "${yellow}Phase 1: Single client tests with various message lengths${reset}"
sleep 1
test_message "" "empty message - 0"
sleep 1
test_message "$short_message" "Short message - 5"
sleep 1
test_message "$medium_message" "Medium message - 45"
sleep 2
test_message "$long_message" "Long message - 1000"
sleep 3
test_message "$very_long_message" "Very long message - 10 000"
sleep 5
test_message "$extremely_long_message" "Extremely long message - 100 000"
sleep 25

if [ $success_1 -eq 1 ]; then
  echo -e "${green}[PASS] All single clients completed.${reset}"
else
  echo -e "${red}[FAIL] Some clients did not complete.${reset}"
fi

sleep 5

echo -e "${yellow}Phase 2: Check that server output match message sent by client${reset}"
sleep 5
# Validate server output
validate_server_output "$short_message" "Short message validation"
validate_server_output "$medium_message" "Medium message validation"
validate_server_output "$long_message" "Long message validation"
validate_server_output "$very_long_message" "Very long message validation"

sleep 5

if [ $success_diff -eq 1 ]; then
  echo -e "${green}[PASS] Server output match message sent by client.${reset}"
else
  echo -e "${red}[FAIL] Some outputs from server does not match message sent by client.${reset}"
fi

sleep 5
echo -e "${red}[-----------------------------]${yellow}BONUS${red}[-----------------------------]${reset}"
# Phase 2: Test multiple clients sending messages simultaneously
sleep 5
echo -e "${yellow}Phase 1: Testing simultaneous clients${reset}"
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
if [ $(wc -l < /tmp/phase2_clients_fail) -eq 1 ]; then
  echo -e "${red}[FAIL] Some clients did not complete.${reset}"
  echo -e "Failed clients:"
  cat /tmp/phase2_clients_fail
fi
if [ $(wc -l < /tmp/phase2_clients_unknown) -eq 1 ]; then
  echo -e "${yellow}[UNKNOWN]${reset}"
  echo -e "clients with unknown errors:"
  cat /tmp/phase2_clients_unknown
  success_2=0
fi

sleep 5
# Phase 3: Stress test with 100 clients
echo -e "${yellow}Phase 2: Stress test with 100 clients${reset}"
sleep 5
> /tmp/phase3_clients_done
> /tmp/phase3_clients_fail
> /tmp/phase3_clients_unknown
for i in {1..100}; do
  random_message="$(head -c 500 < /dev/urandom | tr -dc 'a-zA-Z0-9')"
  test_client_waiting_line "$i" "$random_message" "Stress client $i" "phase3"
  sleep 0.2  # Small delay to simulate staggered client arrivals and waiting line
done

sleep 400
# Validate that all clients finished successsfully in stress test
if [ $(wc -l < /tmp/phase3_clients_done) -eq 100 ]; then
  echo -e "${green}[PASS] All stress test clients completed.${reset}"
  success_3=1
fi
if [ $(wc -l < /tmp/phase3_clients_fail) -eq 1 ]; then
  echo -e "${red}[FAIL] Some clients did not complete.${reset}"
  echo -e "Failed clients:"
  cat /tmp/phase3_clients_fail
  success_3=0
fi
if [ $(wc -l < /tmp/phase3_clients_unknown) -eq 1 ]; then
  echo -e "${yellow}[UNKNOWN]${reset}"
  echo -e "clients with unknown errors:"
  cat /tmp/phase3_clients_unknown
  success_3=0
fi

sleep 10
# Phase 4: Extreme stress test
echo -e "${yellow}Phase 3: Extreme stress test for 10 seconds${reset}"
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

sleep 60

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

sleep 2
# Phase 5: Unicode message test
echo -e "${yellow}Phase 4: Unicode message test${reset}"
validate_server_output "$unicode_message" "Unicode message validation"
sleep 1
./client_bonus "$SERVER_PID" "$unicode_message" > /tmp/unicode_client_output 2>&1
clean_unicode_output=$(sed 's/\x1b\[[0-9;]*m//g' < /tmp/unicode_client_output)
sleep 1

if [ $success_diff -eq 1 ]; then
  echo -e "${green}[PASS] Server output match message sent by client.${reset}"
else
  echo -e "${red}[FAIL] Some outputs from server does not match message sent by client.${reset}"
fi
sleep 1
if echo "$clean_unicode_output" | grep -q "^Server confirmed message reception\.\.\." ; then
  echo -e "${green}[PASS] Unicode message test${reset}"
  success_unicode=1
else
  echo -e "${red}[FAIL] Unicode message test${reset}"
  cat /tmp/unicode_client_output
  success_unicode=0
fi

# Clean up
kill "-SIGINT" "$SERVER_PID"
sleep 1  # Give the server some time to shut down

# If the server is still running, force termination
if ps -p "$SERVER_PID" > /dev/null; then
  echo -e "${red}Server did not terminate. Forcing termination...${reset}"
  kill -9 "$SERVER_PID"
fi
rm server_output.log

sleep 3
echo -e "${red}[-------------------------]${yellow}FINAL SUMMARY${red}[-------------------------]${reset}"
sleep 3
# Final validation
if [ $success_1 -eq 1 ] && [ $success_2 -eq 1 ] && [ $success_3 -eq 1 ] && [ $success_4 -eq 1 ] && [ $success_unicode -eq 1 ] && [ $success_valgrind -eq 1 ] && [ $success_diff -eq 1 ]; then
  echo -e "${green}[SUCCESS] Congratulations. All tests passed successfully. :)${reset}"
else
  echo -e "${red}[FAILED] Too bad. Some test failed. :(${reset}"
fi

