# The | (pipe) Operator

The `|` (_pipe_) operator used in the shell redirects the standard output of one command to the standard input of another command.
When using:

```console
writer | reader
```

the standard output of `writer` is passed to a pipe, from where it is read by `reader` with a redirected standard input.

In the current demo, we use a manager program, to start two new processes: a reader and a writer.

Build the three programs:

```console
make
```

Run the `manager` and see how the message is printed by `reader`, after it received it by pipe from `writer`:

```console
$ ./manager
Message is: Hello, world!
```

To get a better view of what is happening, we use `strace`:

```console
$ strace -f -e clone,execve,dup2,pipe2,close,exit_group,waitpid,write ./manager
execve("./manager", ["./manager"], 0x7ffc1a95b2b8 /* 59 vars */) = 0
close(3)                                = 0
close(3)                                = 0
pipe2([3, 4], 0)                        = 0
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7de6096c7a10) = 225425
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLDstrace: Process 225426 attached
strace: Process 225425 attached
, child_tidptr=0x7de6096c7a10) = 225426
[pid 225424] close(3)                   = 0
[pid 225424] close(4 <unfinished ...>
[pid 225426] close(3 <unfinished ...>
[pid 225424] <... close resumed>)       = 0
[pid 225426] <... close resumed>)       = 0
[pid 225426] dup2(4, 1)                 = 1
[pid 225425] close(4 <unfinished ...>
[pid 225426] execve("./writer", ["./writer"], 0x7ffd51ccdd98 /* 59 vars */ <unfinished ...>
[pid 225425] <... close resumed>)       = 0
[pid 225425] dup2(3, 0)                 = 0
[pid 225426] <... execve resumed>)      = 0
[pid 225425] execve("./reader", ["./reader"], 0x7ffd51ccdd98 /* 59 vars */ <unfinished ...>
[pid 225426] close(3)                   = 0
[pid 225425] <... execve resumed>)      = 0
[pid 225426] close(3)                   = 0
[pid 225425] close(4)                   = 0
[pid 225425] close(4)                   = 0
[pid 225426] write(1, "Hello, world!\0", 14) = 14
[pid 225426] exit_group(0)              = ?
[pid 225426] +++ exited with 0 +++
[pid 225424] --- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=225426, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
[pid 225425] write(1, "Message is: Hello, world!\n", 26Message is: Hello, world!
) = 26
[pid 225425] exit_group(0)              = ?
[pid 225425] +++ exited with 0 +++
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=225425, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
exit_group(0)                           = ?
+++ exited with 0 +++
```

From the `strace` output we can see a correspondence with the source code:

1. The `manager` creates a pipe - the descriptors are 3 and 4.
1. The `manager`program starts two processes.
1. Each processes closes the non-used part of the pipe.
   And then it redirects (with `dup2()`) either standard output (`writer`) or standard input (`reader`) to the pipe.
1. Each process uses `exec()` to run the corresponding executable (`writer` or `reader`).
1. The `writer` process (PID `225426`) writes the message to the standard output file descriptor (`1`), now redirected to a pipe.
1. The `reader` process (PID `225425`) writes the message to the actual standard output.

We can observe a similar behavior by tracing the shell.

We need two consoles.
On one console, we find the PID of the shell process:

```console
$ echo $$
7728
```

On another console we trace the first shell process:

```console
$ sudo strace -f -e clone,execve,dup2,pipe2,close,exit_group,waitpid,write -p 7728
[...]
pipe2([3, 4], 0)                        = 0
pipe2([5, 6], 0)                        = 0
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f7789e98a10) = 225708
strace: Process 225708 attached
[pid  7728] close(4)                    = 0
[pid  7728] close(4)                    = -1 EBADF (Bad file descriptor)
[pid  7728] clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD <unfinished ...>
[pid 225708] close(6)                   = 0
strace: Process 225709 attached
[pid  7728] <... clone resumed>, child_tidptr=0x7f7789e98a10) = 225709
[pid  7728] close(3)                    = 0
[pid  7728] close(5)                    = 0
[pid  7728] close(6)                    = 0
[pid 225709] close(5)                   = 0
[pid 225709] close(6)                   = 0
[pid 225709] dup2(3, 0 <unfinished ...>
[pid 225708] close(5 <unfinished ...>
[pid 225709] <... dup2 resumed>)        = 0
[pid 225709] close(3 <unfinished ...>
[pid 225708] <... close resumed>)       = 0
[pid 225709] <... close resumed>)       = 0
[pid 225708] close(3)                   = 0
[pid 225708] dup2(4, 1)                 = 1
[pid 225708] close(4)                   = 0
[pid 225709] execve("./reader", ["./reader"], 0x5a3be1a47ca0 /* 59 vars */ <unfinished ...>
[pid 225708] execve("./writer", ["./writer"], 0x5a3be1a47ca0 /* 59 vars */ <unfinished ...>
[pid 225709] <... execve resumed>)      = 0
[pid 225708] <... execve resumed>)      = 0
[pid 225709] close(3 <unfinished ...>
[pid 225708] close(3 <unfinished ...>
[pid 225709] <... close resumed>)       = 0
[pid 225708] <... close resumed>)       = 0
[pid 225708] close(3 <unfinished ...>
[pid 225709] close(3 <unfinished ...>
[pid 225708] <... close resumed>)       = 0
[pid 225709] <... close resumed>)       = 0
[pid 225708] write(1, "Hello, world!\0", 14) = 14
[pid 225708] exit_group(0)              = ?
[pid 225709] write(1, "Message is: Hello, world!\n", 26) = 26
[pid 225709] exit_group(0)              = ?
[pid 225708] +++ exited with 0 +++
[pid 225709] +++ exited with 0 +++
```

We can see a similar pattern as in our own implementation.
Notable difference are:

1. The shell creates an additional pipe, probably for synchronization purposes with the child processes.
1. The process creation (`clone`) and `execve` syscall invocation are closer in the output.

Otherwise, the same redirection to pipe endpoints happens, via `dup2()`.
