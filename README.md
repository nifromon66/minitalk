# Minitalk

## Project Overview

**Minitalk** is a minimal data exchange program written in C that demonstrates interprocess communication (IPC) using UNIX signals. The project involves the implementation of two executable programs: **server** and **client**. The server listens for messages sent by clients, and messages are transmitted bit-by-bit using only the `SIGUSR1` and `SIGUSR2` signals.

This project challenges you to handle low-level signaling mechanisms, synchronization, and message encoding/decoding—all while respecting stringent coding norms and safety practices imposed by the 42 curriculum.

## Contents

- [Features](#features)
- [Requirements](#requirements)
- [Compilation](#compilation)
- [Usage](#usage)
- [Architecture \& Program Flow](#architecture--program-flow)
- [Signals Protocol \& Message Encoding](#signals-protocol--message-encoding)
- [Bonus Features](#bonus-features)
- [Error Handling](#error-handling)
- [Testing](#testing)
- [Files Overview](#files-overview)
- [Credits](#credits)


## Features

- **Bidirectional communication** between a server and multiple clients using UNIX signals.
- **Client-to-server message delivery:** messages sent as strings.
- **Server prints each received message promptly after full transmission**.
- **Supports consecutive messages from multiple clients without restarting the server**.
- **Bonus:** Server acknowledges message receipt to client, and **Unicode support** for non-ASCII characters.


## Requirements

- **Language:** C (compliant with 42 Norm).
- **UNIX signals:** Only `SIGUSR1` and `SIGUSR2` for communication.
- **Allowed functions (mandatory):**
    - `write`, your own `ft_printf`, `signal`, `sigemptyset`, `sigaddset`, `sigaction`, `kill`, `getpid`, `malloc`, `free`, `pause`, `sleep`, `usleep`, and `exit`.
- **No more than one global variable per program (server/client).**
- **Must handle errors (no segmentation faults, memory leaks, etc.).**
- **Proper and thorough error handling and memory management.**


## Compilation

To build both the mandatory and bonus parts:

```sh
make        # Builds mandatory client and server
make bonus  # Builds the bonus version with advanced features
```

Output executables will be:

- `server` or `server_bonus`
- `client` or `client_bonus`


## Usage

### 1. **Start the Server**

```sh
./server
```

or, for bonus features:

```sh
./server_bonus
```

- On startup, the server displays its **PID**:

```
SERVER PID: [2392089]    # Example output
```


### 2. **Send a Message from Client**

```sh
./client <SERVER_PID> "<message>"
```

Example:

```sh
./client 2392089 "Hello, world!"
```

- The message will be transmitted (bit-by-bit via signals) to the server and displayed in the server output upon complete delivery:

```
Message received from client: Hello, world!
```


### 3. **Bonus Usage (Acknowledgement and Unicode)**

- With `server_bonus` and `client_bonus`, the server sends an acknowledgement signal after processing each message, and Unicode message transmission is supported:

```
$ ./client_bonus 2392089 "你好，世界!"
Server confirmed message reception...
```


## Architecture \& Program Flow

### **Server**

- Initializes `sigaction` handlers for `SIGUSR1` and `SIGUSR2`.
- Prints its PID for client reference.
- Reconstructs each message one bit at a time, building up bytes as signals arrive.
- When a full message is received (detected by null-terminator), it prints the message and, in bonus, sends an ACK signal to the client.


### **Client**

- Parses command-line arguments for server PID and message.
- Encodes each character of the message into bits.
- Sends each bit as either `SIGUSR1` (representing 0) or `SIGUSR2` (representing 1) using `kill()`.
- Optionally (bonus), waits for an ACK from the server before exiting.


##### **Control Flow Diagram Overview**

```
Client                   Server
  | -------------bit-by-bit---> |
  |                             |
  | <--Acknowledgement(bonus)-- |
```


## Signals Protocol \& Message Encoding

- Each character in the message is sent as a sequence of 8 bits.
- The client:
    - Iterates over each bit in the character.
    - Sends `SIGUSR1` for `0`, `SIGUSR2` for `1`.
- The server:
    - Reconstructs the bits for each character until it receives a null byte (`'\0'`), signifying the end of the message.
    - In the bonus version, sends an ACK signal back after full message received.

**Note:** The server is robust against consecutive messages, handling each full message-to-ACK cycle sequentially. Special care is required due to the fact that UNIX signals are not queued—if sent too rapidly, some may be lost, requiring careful synchronization and possibly usleep delays on the client side.

## Bonus Features

- **Acknowledgement Signal:**
Once a message is fully interpreted and printed, the server acknowledges receipt by sending a signal back to the client, allowing the client to confirm successful delivery.
- **Unicode Support:**
Full UTF-8 support lets messages contain multilingual text and emoji.
- **Concurrent Clients:**
Multiple clients can send messages to the server, either consecutively or simultaneously, as tested by automated scripts.


## Error Handling

- The programs detect and report errors such as missing/invalid arguments, invalid PIDs, and signal transmission failures.
- Memory allocation failure and all abnormal states are gracefully reported; neither program should ever crash or leak memory.


## Testing

A robust `test_minitalk.sh` script validates:

- **Single client, various message sizes:**
(Empty, short, medium, long, very long, extremely long, Unicode)
- **Correct message printing by server.**
- **Simultaneous client messages and server stability.**
- **Server-acknowledged delivery (bonus).**
- **Stress and extreme stress scenarios** (many rapid clients and messages).
- **Example excerpt from server output:**

```
Message received from client: Hello
Message received from client: This is a medium-length message for testing.
Message received from client: 你好，世界! Привет, мир! Hello, world! 🌍🌟
Closing Server...
```


## Files Overview

- `server.c` / `server_bonus.c` — Main server logic and signal handling.
- `client.c` / `client_bonus.c` — Client for message sending.
- `libft/` — Personal C library of utility functions (if used).
- `test_minitalk.sh` — Comprehensive test suite.
- `Makefile` — Compilation rules for all parts.


## Credits

**Author(s):** [Nicolas Fromont, alias nifromon], as part of 42 School curriculum.

