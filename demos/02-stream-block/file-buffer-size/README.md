# Buffer Size for File Writing

When writing to a file, we can use different buffer sizes (for the user space buffer).
The size of the buffer affects the performance of the program.
If the buffer is too small, this results up in more system calls - and more overhead.
If the buffer is too big, this will mean the kernel has to have enough space to copy date from the (user space) buffer to its own kernel buffer, that will also take time (and counter the reduction in system calls).

The `write_file.c` program allows us to have different buffer sizes sent out as arguments.

First build the program:

```console
make
```

Now run the program with different buffer sizes argument and see the time duration:

```console
$ ./write_file 1024
time passed 852859 microseconds

$ ./write_file 1024
time passed 865894 microseconds

$ ./write_file 2048
time passed 492643 microseconds

$ ./write_file 2048
time passed 480754 microseconds

$ ./write_file 4096
time passed 302361 microseconds

$ ./write_file 4096
time passed 291755 microseconds

$ ./write_file 8192
time passed 225702 microseconds

$ ./write_file 8192
time passed 222080 microseconds

$ ./write_file 16384
time passed 205563 microseconds

$ ./write_file 16384
time passed 179069 microseconds

$ ./write_file 32768
time passed 205071 microseconds

$ ./write_file 32768
time passed 208430 microseconds

$ ./write_file 65536
time passed 197010 microseconds

$ ./write_file 65536
time passed 196062 microseconds
```

We observe that for smaller buffers (`1024`, `2048`) it takes more time to do the operations.
However, once the buffer size reaches 16K (or even 8k) there is no obvious improvement in duration.
So, a buffer size of approximately 8K, or 16K should be an optimal choice for file-based operations.
