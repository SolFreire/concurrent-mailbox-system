# Concurrent Mailbox System using Pthreads

This project implements the mailbox communication concept using concurrent threads in C with POSIX Threads (`pthreads`) and semaphores.

The system was developed as an assignment for the STR (Software de Tempo-Real / Real-Time Systems Software) course.

---

# Overview

The application simulates a concurrent message passing system composed of:

## Source Threads

* A
* B
* C
* D

## Destination Threads

* E
* F
* G
* H

## Mailbox Thread

* `t_mailbox`

The mailbox acts as an intermediary communication service between producers and consumers.

---

# Communication Flow

```text
A -> mailbox -> E
B -> mailbox -> F
C -> mailbox -> G
D -> mailbox -> H
```

Each source thread sends a message to the mailbox thread using synchronous communication.

The mailbox stores incoming messages into a buffer with 12 positions.

When the mailbox becomes full:

1. Messages are decoded
2. Destination identifiers are extracted
3. Messages are forwarded to the corresponding destination threads

---

# Message Structure

Each message is composed of 2 bytes:

| Byte | Description                   |
| ---- | ----------------------------- |
| MSB  | Destination Thread Identifier |
| LSB  | Message Value                 |

Example:

```c
msg = ('E' << 8) | 10;
```

This message means:

* Destination: `E`
* Value: `10`

---

# Technologies Used

* C Language
* POSIX Threads (`pthread`)
* POSIX Semaphores (`semaphore.h`)
* Bit Masking
* Synchronous IPC Communication

---

# Synchronization Model

The project uses synchronous send/receive operations based on semaphores:

```c
send_sync()
receive_sync()
```

This guarantees:

* Mutual synchronization
* Ordered communication
* Safe message passing
* No race conditions

---

# Features

* Concurrent thread communication
* Mailbox-based architecture
* Continuous execution
* Synchronous message passing
* Bitwise message encoding/decoding
* Shared communication channels
* Semaphore synchronization

---

# Compilation

```bash
gcc mailbox.c -o mailbox -pthread
```

---

# Execution

```bash
./mailbox
```

---

# Example Output

```text
A enviando mensagem
B enviando mensagem
C enviando mensagem
D enviando mensagem

Mailbox armazenou mensagem[0] = 17674
Mailbox armazenou mensagem[1] = 17940
Mailbox armazenou mensagem[2] = 18206
Mailbox armazenou mensagem[3] = 18472

MAILBOX CHEIA
Encaminhando mensagens...

Thread E recebeu: 10
Thread F recebeu: 20
Thread G recebeu: 30
Thread H recebeu: 40
```

---

# Academic Context

This project was created for the STR course and aims to demonstrate concepts related to:

* Real-Time Systems
* Concurrent Programming
* Interprocess Communication (IPC)
* Synchronization
* Mailbox Architectures
* Message Passing Systems

---
