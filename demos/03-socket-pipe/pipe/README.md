# Bidirection Transfer with (Anonymous) Pipes

This example demonstrates a bidirectional transfer using (anonymous) pipes.
Because pipes are unidirectional, we use two pipes, one for each transfer direction.

Build with:

```console
make
```

Then run the "server" and "client" processes as parent-child processes:

```console
./client_server
```

Two pipes are created in the program: `pipe_to_server` an `pipe_from_server`.
A message is being sent from the client to server using a pipe.
And then a reply with the same message is being sent back from the server to the client using the other pipe.
Each process only using the corresponding endpoint of the pipe (`pipe[0]` is for reading and `pipe[1]` is for writing).
