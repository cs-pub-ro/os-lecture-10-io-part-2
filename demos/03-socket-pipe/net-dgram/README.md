# Bidirection Transfer with Network Datagram Sockets

This example demonstrates a bidirectional transfer using a pair of network datagram sockets.
(Network datagram) Sockets are bidirectional, so only a pair of two sockets is required: one for each side - client and server.

Build with:

```console
make
```

First run the server on one console:

```console
./server
```

Then write the client on another console:

```console
./client
```

A message is being sent from the client to server using the client socket - the server receives it via its own socket.
And then a reply with the same message is being sent back from the server to the client using the server socket - the client receives it via its own socket.
