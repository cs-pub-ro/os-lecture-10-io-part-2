# Bidirection Transfer with FIFO

This example demonstrates a bidirectional transfer using a FIFO (named pipe).
Because FIFOs / pipes are unidirectional, we use two FIFOs, one for each transfer direction.

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

A message is being sent from the client to server using the `to_server.fifo` FIFO.
And then a reply with the same message is being sent back from the server to the client using the `from_server.fifo` FIFO.
