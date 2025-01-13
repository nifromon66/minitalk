#!/bin/bash

# Enhanced test script for Minitalk project
# Assumes the server executable is ./server and the client executable is ./client

# Colors for output
green="\033[0;32m"
red="\033[0;31m"
yellow="\033[0;33m"
reset="\033[0m"

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
# Generate a message near the maximum limit (assumed 1 MB here)
max_limit_message="$(head -c 1048576 < /dev/urandom | tr -dc 'a-zA-Z0-9')"

# Function to send a message from a client and check result
test_message() {
  local message="$1"
  local label="$2"

  ./client "$SERVER_PID" "$message" > /tmp/client_output 2>&1
  sleep 0.75
  if [ $? -eq 0 ]; then
    echo -e "${green}[PASS] ${label}${reset}"
  else
    echo -e "${red}[FAIL] ${label}${reset}"
    cat /tmp/client_output
  fi
}

# Enhanced function to test if a client finishes successfully and tracks waiting line
test_client_waiting_line() {
  local client_id="$1"
  local message="$2"
  local label="$3"

  sleep 0.07
  (./client "$SERVER_PID" "$message" > /tmp/client_output_${client_id} 2>&1 && echo "done_${client_id}" >> /tmp/clients_done) &
}

# Test single client with various message lengths
test_message "$short_message" "Short message"
sleep 1
test_message "$medium_message" "Medium message"
sleep 2
test_message "$long_message" "Long message"
sleep 3
test_message "$very_long_message" "Very long message"
sleep 4
test_message "$max_limit_message" "Maximum limit message"
sleep 5

# Test multiple clients sending messages simultaneously
echo -e "${green}Testing simultaneous clients...${reset}"
sleep 5
for i in {1..10}; do
    sleep 0.1
    test_client_waiting_line "$i" "$medium_message Simultaneous message from client $i" "Simultaneous client $i"
done

sleep 25
# Validate that all clients finished successfully
if [ $(wc -l < /tmp/clients_done) -eq 10 ]; then
  echo -e "${green}[PASS] All simultaneous clients completed.${reset}"
else
  echo -e "${red}[FAIL] Some clients did not complete.${reset}"
fi

sleep 10
# Stress test: 100 clients sending random messages with waiting line considerations
echo -e "${green}Starting stress test with 100 clients and waiting line...${reset}"
sleep 5
> /tmp/clients_done
for i in {1..100}; do
  random_message="$(head -c 500 < /dev/urandom | tr -dc 'a-zA-Z0-9')"
  test_client_waiting_line "$i" "$random_message" "Stress client $i"
  sleep 0.2  # Small delay to simulate staggered client arrivals and waiting line
done

sleep 300
# Validate that all clients finished successfully in stress test
if [ $(wc -l < /tmp/clients_done) -eq 100 ]; then
  echo -e "${green}[PASS] All stress test clients completed.${reset}"
else
  echo -e "${red}[FAIL] Some stress test clients did not complete.${reset}"
fi

sleep 10
# Extreme stress test: Continuous clients for 10 seconds with waiting line considerations
echo -e "${green}Starting extreme stress test for 10 seconds with waiting line...${reset}"
sleep 5
> /tmp/clients_done
end_time=$((SECONDS + 10))
client_id=1
while [ $SECONDS -lt $end_time ]; do
  random_message="$(head -c 200 < /dev/urandom | tr -dc 'a-zA-Z0-9')"
  test_client_waiting_line "$client_id" "$random_message" "Extreme stress client $client_id"
  sleep 0.1  # Small delay to ensure the server queues clients correctly
  client_id=$((client_id + 1))
done

sleep 300

# Validate that all clients finished successfully in extreme stress test
if [ $(wc -l < /tmp/clients_done) -eq $((client_id - 1)) ]; then
  echo -e "${green}[PASS] All extreme stress test clients completed.${reset}"
else
  echo -e "${red}[FAIL] Some extreme stress test clients did not complete.${reset}"
fi

sleep 10
# Clean up
kill "-SIGINT" "$SERVER_PID"
sleep 1  # Give the server some time to shut down

# If the server is still running, force termination
if ps -p "$SERVER_PID" > /dev/null; then
  echo -e "${red}Server did not terminate. Forcing termination...${reset}"
  kill -9 "$SERVER_PID"
fi

echo -e "${green}Server process terminated.${reset}"


