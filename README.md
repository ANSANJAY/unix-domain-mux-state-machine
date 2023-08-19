# Multiplexed Server-Client State Machine with `select()` System Call

This document describes how to use the `select()` system call to create a multiplexed server-client state machine. We'll focus on the maintenance of the `fd_set` data structure which allows for efficient monitoring of multiple file descriptors.

## Table of Contents
- [Introduction](#introduction)
- [State Machine Overview](#state-machine-overview)
- [Understanding `fd_set`](#understanding-fd_set)
- [Steps to Implement](#steps-to-implement)
- [Examples](#examples)
- [Best Practices](#best-practices)
- [References](#references)

## Introduction

Multiplexing in server-client systems involves handling multiple client connections simultaneously without spawning separate threads or processes for each connection. The `select()` system call in UNIX allows us to monitor multiple file descriptors to see if they have data ready for reading or writing.

## State Machine Overview

A multiplexed server-client state machine can be broken down into the following states:

```
     +------------------------------------+
     |            SERVER                  |
     |                                    |
     |    +-------------------------+     |
     |    |                         |     |
     |    |       select()          |<-----------------------+
     |    |                         |                        |
     |    +-------------------------+                        |
     |                                    |                  |
     |    +-------------------------+     |                  |
     |    |   Process Ready FDs     |     |                  |
     |    +-------------------------+     |                  |
     +------------------------------------+                  |
            |            |            |                       |
   +--------+     +-----+------+  +----+------+               |
   |        |     |            |  |           |               |
   | Client |     | Client 2   |  | Client N  |               |
   |    1   |     |            |  |           |               |
   +--------+     +------------+  +-----------+               |
      |                |              |                       |
      +------------------------------------------------------+
```

## Explanation:

1. The **Server** listens for incoming client connections.
2. Multiple **Clients** (Client 1, Client 2, ... Client N) connect to the server.
3. The **Server** uses `select()` to monitor the file descriptors (FDs) of all connected clients to check if any of them have data that's ready to be read or processed.
4. When the `select()` call identifies ready FDs, the server then processes them accordingly.
5. Server can send responses back to the clients, and this loop continues, allowing the server to handle multiple clients concurrently without the need for threading or multiprocessing.

Using the `select()` syscall, the server can efficiently handle many clients at once without being blocked by any single client's activity.



## Understanding `fd_set`

`fd_set` is a data structure used to represent a set of file descriptors. It plays a pivotal role in utilizing the `select()` function effectively.

### Key Functions & Macros for `fd_set` Management:

- `FD_ZERO(fd_set *set)`: Initializes the file descriptor set to have zero bits for all file descriptors.
- `FD_SET(int fd, fd_set *set)`: Set the bit for the file descriptor `fd` in the set.
- `FD_CLR(int fd, fd_set *set)`: Clear the bit for the file descriptor `fd` in the set.
- `FD_ISSET(int fd, fd_set *set)`: Test to see if the bit for the file descriptor `fd` is set in the set, returning a non-zero value if true.

### Maintaining `fd_set`:

1. **Initialization**: Always initialize an `fd_set` using `FD_ZERO` before use.
2. **Add New Connections**: As new clients connect, use `FD_SET` to add their file descriptors to the monitoring set.
3. **Remove Disconnected Clients**: When clients disconnect or after processing a client's data, use `FD_CLR` to remove their file descriptors from the set.
4. **Checking Activity**: After `select()` returns, use `FD_ISSET` in a loop to check which file descriptors are "ready" for reading or writing.

## Steps to Implement

1. Create a master `fd_set` and use `FD_ZERO` to initialize it.
2. As clients connect, add their file descriptors using `FD_SET`.
3. Call `select()` to block until any client file descriptor becomes ready.
4. After `select()` returns, iterate through all file descriptors and use `FD_ISSET` to determine which ones are ready.
5. Process ready file descriptors accordingly (read, write, or disconnect).
6. Repeat steps 3-5 as needed.

## Examples

For illustrative code samples that demonstrate using `select()` with the server-client state machine model, consider diving deeper into UNIX networking programming tutorials or textbooks.

## Best Practices

- Always reinitialize your `fd_set` before a new call to `select()`.
- Handle errors from `select()` gracefully, especially `EINTR` which indicates an interrupted system call.
- Ensure you handle the case where `select()` times out without any file descriptor activity.
  
## References

- [man select](http://man7.org/linux/man-pages/man2/select.2.html)
- "UNIX Network Programming" by W. Richard Stevens


# `select()` System Call in UNIX

The `select()` system call is an essential tool in UNIX-based systems for multiplexing input/output operations across multiple file descriptors. This README highlights its significance, particularly in multiplexing UNIX domain client-server interactions.

## Table of Contents
* [Overview](#overview)
* [Limitations and Recommendations](#limitations-and-recommendations)
* [Usage in Multiplexing UNIX Domain Sockets](#usage-in-multiplexing-unix-domain-sockets)
* [Key Functions and Macros](#key-functions-and-macros)
* [Examples](#examples)
* [References](#references)

## Overview

`select()` allows programs to monitor multiple file descriptors, waiting until one or more of them become "ready" for some kind of I/O operation.

## Limitations and Recommendations

**FD_SETSIZE Limitation**: 
The `select()` system call can monitor only file descriptor numbers less than `FD_SETSIZE` (commonly 1024). This can be restrictive for many modern applications.

**Recommendation**: 
Modern applications requiring more scalability should use `poll(2)` or `epoll(7)`.

## Usage in Multiplexing UNIX Domain Sockets

In a UNIX domain client-server scenario, a server might need to handle numerous client connections simultaneously. Using `select()`, the server can keep tabs on all its client sockets, reacting whenever any of the sockets have data ready for reading or are ready for writing.

**Steps**:

1. **Server Initialization**: Set up a UNIX domain socket and start listening for client connections.
2. **Accepting Clients**: Store file descriptors as clients connect.
3. **Multiplexing with `select()`**: Monitor all client sockets at once.
4. **Processing Data**: If `select()` indicates one or more descriptors are ready, process them.
5. **Closing Connections**: Close the respective socket once data exchange with a client concludes.

## Key Functions and Macros

* `FD_ZERO(fd_set *set)`: Clear the file descriptor set.
* `FD_SET(int fd, fd_set *set)`: Add a file descriptor to the set.
* `FD_CLR(int fd, fd_set *set)`: Remove a file descriptor from the set.
* `FD_ISSET(int fd, fd_set *set)`: Check if a file descriptor is in the set.

## Examples

For code samples showing the `select()` function in action with UNIX domain sockets, refer to renowned UNIX networking programming resources or online tutorials.

## References

* [man select](http://man7.org/linux/man-pages/man2/select.2.html)
* "UNIX Network Programming" by W. Richard Stevens
* "Advanced Programming in the UNIX Environment" by W. Richard Stevens

------
# Multiplexed Client-Server Model using `select()` System Call for Sum Calculation

## Overview:

A server is designed to accept connections from multiple clients. Each client sends integers to the server. The server maintains a running sum of integers received from each individual client. When a client sends the integer value `0`, the server responds back with the total sum of integers sent by that client up to that point and closes the connection to that client.

## Problem Diagram:

         +----------------------------------+
         |            SERVER                |
         |                                  |
         |    +-----------------------+     |
         |    |      select()         |<--------------------+
         |    +-----------------------+     |               |
         |                                  |               |
         |    +-----------------------+     |               |
         |    |  Calculate & Respond  |     |               |
         |    +-----------------------+     |               |
         +--------+----+----------+---------+               |
         | Init FD | Add| Refresh | Remove  |               |
         +--------+----+----------+---------+               |
              |          |          |                       |
     +--------+   +------+-----+  +----+-----+               |
     |        |   |            |  |         |               |
     | Client |   | Client 2   |  | Client N|               |
     |    1   |   |            |  |         |               |
     +--------+   +------------+  +---------+               |
        |              |            |                       |
        +--------------------------------------------------+

## Design:

1. **Server Initialization**: 
   - The server initializes and binds to a known UNIX domain address.
   - It starts listening for incoming client connections.
   - Initializes arrays for monitored file descriptors and client results.

2. **Client Connection**: 
   - Each client connects to the server.
   - Server, on accepting a new client connection, adds the client's file descriptor to the monitored set.

3. **Data Transmission**:
   - Clients send integers to the server.
   - Server, on receiving an integer from a client:
     - If the integer is `0`, responds back with the sum for that client, resets the client's sum to zero, and closes the client connection.
     - If the integer is not `0`, the server adds the integer to that client's running total.

4. **Server Multiplexing**:
   - The server uses `select()` to efficiently manage and monitor file descriptors of all connected clients without blocking on any individual client's activity. It also refreshes the FD set before each call to `select()`.
   
5. **Utilities**:
   - The server has utility functions to initialize, add, remove, and refresh the monitored FD set. It also has a utility function to get the maximum FD for use with `select()`.

## Pseudocode based on Provided Code:

```
initialize monitored FD set
create server socket
bind and listen on server socket
add server socket to monitored FD set

while True:
    refresh FD set for select()
    use select() to wait for activity

    if there's activity on the server socket:
        accept new client connection
        add new client's FD to monitored set

    else:
        for each client FD in monitored set:
            if there's activity on client FD:
                read integer from client

                if integer == 0:
                    send accumulated sum to client
                    close client connection
                    reset client's sum to zero
                    remove client FD from monitored set

                else:
                    add integer to client's accumulated sum

```
## Expexted Output 
![](./Screenshot%20from%202023-08-18%2023-51-27.png)