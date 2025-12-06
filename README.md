# IO (part 2): Inter-Process Channels

## 0. Pitch

- **demo**: `lsof -p $(pidof Discord | head -1) -a -d 0-1023`: show how many are sockets, FIFOs and pipes
- **demo**: `subprocess.Popen()` in Python

## 1. File Descriptors, File Descriptor Table, I/O Channels

- interaction with the outside world is via an I/O channel
- I/O channel presented as a file descriptor
- file descriptor table is a per-process array of file descriptors: `struct io_channel *fdt[MAX_FILES]`
- **diagram**: process, file descriptors, file descriptor table, I/O channels

## 2. Stream vs Block I/O Channels

- use of buffers
- **diagram**: stream vs block
- stream: "pipe" buffer, producer-consumer, ring-buffers
- block: "area" buffers, seek operations, sync with underlying storage
- **demo**: size of buffer
- all inter-process channels are stream
- char devices are process-to-device channels: stream - terminal, keyboard, serial communication
- block devices are process-to-device channels: block - disks, storage

## 3. Sockets vs Pipes

- 2 endpoints for a connection: each belonging to another process
- single buffer (pipes - one buffer for both endpoints) vs 4 buffers (sockets - 2 buffers for each endpoint)
- identifying the channel to "attach" to:
  - pipes: inherited (related processes)
  - FIFO: filesystem entry
  - UNIX socket: filesystem entry (created by server)
  - network socket: source IP address, destination IP address, source port, destination port
- **demo**: bidirectional communication with pipes, FIFOs, network sockets (TCP, UDP), UNIX sockets (stream, datagram)

## 4. Redirection

- two FDT entries pointing to the same I/O channel
- **diagram**: redirection
- **demo**: redirect standard output
- **demo**: implement the `|` shell operator

## 5. Socket Operations

- one socket is a server, the other(s) are clients
- **diagram**: typical sequence:
   - S: create server socket
   - S: wait for connections
   - C: initiate connection
   - S: create connection
   - S-C: communicate
   - C: close connection
- connection-oriented (stream) (TCP) vs datagram (UDP)
- for TCP: a separate connection socket is created per each client
  - for N clients: 1 server socket, N connection sockets
- for UDP: no actual connection; the server socket receives all datagrams for all clients
  - for N clients: 1 socket
- **demo**: N client connections to stream (TCP) and datagram (UDP) servers
- how to handle multiple (simultaneous) connections: next sessions

## 6. Blocking Actions

- I/O is blocking
- reader / writer for block I/O channel
  - reader blocks if no data available - has to be brought from storage
  - writer rarely blocks: allocate memory, write
- sender / receiver for stream I/O channel
  - sender blocks if buffer is full
  - receiver blocks is buffer is empty
  - sender either:
    - returns number of bytes available to send
    - blocks until at least a slot is available in the buffer
    - returns error
  - receiver either:
    - returns number of bytes available to receive
    - blocks waiting for at least on byte is in the buffer
    - return 0 if connection is closed
    - returns error

## Conclusion and Takeaways

- I/O channels are stream channels and block channels.
- Stream channels are implemented as ring buffers: one side pushes data, the other side pulls data.
- Char devices, pipes, FIFOs, sockets are stream channels.
- Inter-process stream channels have two endpoints, one in each process: pipes and FIFOs have one buffer, sockets have 4 buffers (2 per socket, receive and send buffer).
- Redirection can be used to point file descriptors to other channel; this is how the pipe operator is implemented.
- Socket operations have a sequence of step to establish a communication channel.
- I/O operations are blocking. For stream channels, blocking happens when buffer is full (for send) or when buffer is empty (for receive).
