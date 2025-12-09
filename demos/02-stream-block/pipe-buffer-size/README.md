# Buffer Size for Pipes

When writing to a pipe, we can use different buffer sizes (for the user space buffer).
The size of the buffer affects the performance of the program.
If the buffer is too small, this results up in more system calls - and more overhead.
If the buffer is too big, this will mean the kernel has to have enough space to copy date from the (user space) buffer to its own kernel buffer, that will also take time (and counter the reduction in system calls).

The `send_receive_pipe.c` program allows us to have different buffer sizes sent out as arguments.

First build the program:

```console
make
```

Now run the program with different buffer sizes argument and see the time duration:

```console
$ ./send_receive_pipe 1024
writer: time passed 744894 microseconds
reader: time passed 744943 microseconds

$ ./send_receive_pipe 4096
reader: time passed 427815 microseconds
writer: time passed 427738 microseconds

$ ./send_receive_pipe 1024
reader: time passed 729098 microseconds
writer: time passed 729010 microseconds

$ ./send_receive_pipe 1024
reader: time passed 731339 microseconds
writer: time passed 731279 microseconds

$ ./send_receive_pipe 2048
reader: time passed 540483 microseconds
writer: time passed 540433 microseconds

$ ./send_receive_pipe 2048
reader: time passed 527423 microseconds
writer: time passed 527343 microseconds

$ ./send_receive_pipe 4096
reader: time passed 455335 microseconds
writer: time passed 455293 microseconds

$ ./send_receive_pipe 4096
reader: time passed 440389 microseconds
writer: time passed 440329 microseconds

$ ./send_receive_pipe 8192
reader: time passed 427251 microseconds
writer: time passed 427203 microseconds

$ ./send_receive_pipe 8192
reader: time passed 439151 microseconds
writer: time passed 439073 microseconds

$ ./send_receive_pipe 16384
reader: time passed 503341 microseconds
writer: time passed 503259 microseconds

$ ./send_receive_pipe 16384
reader: time passed 405287 microseconds
writer: time passed 405228 microseconds

$ ./send_receive_pipe 32768
reader: time passed 525500 microseconds
writer: time passed 525421 microseconds

$ ./send_receive_pipe 32768
reader: time passed 536910 microseconds
writer: time passed 536855 microseconds

$ ./send_receive_pipe 65536
reader: time passed 625059 microseconds
writer: time passed 625015 microseconds

$ ./send_receive_pipe 65536
reader: time passed 629571 microseconds
writer: time passed 629497 microseconds
```

We observe that for smaller buffers (`1024`, `2048`) it takes more time to do the operations.
However, once the buffer size reaches 4K (or even 8k) there is no obvious improvement in duration.
Moreover, paste 32K, the performance actually decreases:
the pipe itself is a buffer and the pipe buffer is filled (or emptied) more often leading to block operations from the reader (or writer) process.
