# Bidirection Transfer with Network Sockets

This example demonstrates a bidirectional transfer using a pair of network sockets part of the same connection.
(Network stream) Sockets are bidirectional, so only a connection with two sockets is required: one for each side - client and server.
The server uses an additional listening socket.
This is used to manage connections.
A separate server connection socket is used for the actual connection.

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

A message is being sent from the client to server using the connection sockets.
And then a reply with the same message is being sent back from the server to the client using the same connection.
