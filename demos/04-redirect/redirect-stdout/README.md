# Redirect Standard Output

The output of a command can be directed to a file.
This is what the `>` operators does in the shell.
This is achieved by duplicating the standard output descriptor (`1`) into a standard file descriptor of a file.

There are two demos in the current directory.
First build them using:

```console
make
```

The first one, `redirect_stdout`, redirects standard output for the current process.
Run the commands below:

```console
$ ./redirect_stdout

$ cat out.txt
Hello, World!

$ strace -e openat,dup2,write,close ./redirect_stdout 
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
close(3)                                = 0
openat(AT_FDCWD, "out.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644) = 3
dup2(3, 1)                              = 1
close(3)                                = 0
write(1, "Hello, World!\n", 14)         = 14
+++ exited with 0 +++
```

We can see that the process opens a file and then uses `dup2(3, 1)` to redirect the file descriptor (`3`) to the standard output file descriptor (`1`).
Then, anything written to standard output (via `write`, `printf` or `puts`) is redirected to the `out.txt` file.

A more realistic use case is replicating what the shell is doing when using the standard output redirection operator (`>`).
In this case, the sequence is:

1. Open the file.
1. Create a new process with `fork()`.
1. Redirect standard output to the file via `dup2()`.
1. Execute an actual program, such as `/bin/ls`.

This is what the `redirect_child_stdout` program does.
Run the commands below:

```console
$ ./redirect_child_stdout

$ cat out.txt
Makefile
out.txt
README.md
redirect_child_stdout
redirect_child_stdout.c
redirect_child_stdout.o
redirect_stdout
redirect_stdout.c
redirect_stdout.o

$ strace -f -e openat,close,write,dup2,clone,execve ./redirect_child_stdout
execve("./redirect_child_stdout", ["./redirect_child_stdout"], 0x7ffee87396b8 /* 59 vars */) = 0
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
close(3)                                = 0
openat(AT_FDCWD, "out.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644) = 3
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7148d99c0a10) = 226742
close(3)                                = 0
strace: Process 226742 attached
[pid 226742] dup2(3, 1)                 = 1
[pid 226742] execve("/bin/ls", ["/bin/ls"], 0x7ffc688af448 /* 59 vars */) = 0
[pid 226742] openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 4
[pid 226742] close(4)                   = 0
[pid 226742] openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libselinux.so.1", O_RDONLY|O_CLOEXEC) = 4
[pid 226742] close(4)                   = 0
[pid 226742] openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 4
[pid 226742] close(4)                   = 0
[pid 226742] openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libpcre2-8.so.0", O_RDONLY|O_CLOEXEC) = 4
[pid 226742] close(4)                   = 0
[pid 226742] openat(AT_FDCWD, "/proc/filesystems", O_RDONLY|O_CLOEXEC) = 4
[pid 226742] close(4)                   = 0
[pid 226742] openat(AT_FDCWD, "/usr/lib/locale/locale-archive", O_RDONLY|O_CLOEXEC) = 4
[pid 226742] close(4)                   = 0
[pid 226742] openat(AT_FDCWD, ".", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 4
[pid 226742] close(4)                   = 0
[pid 226742] write(1, "Makefile\nout.txt\nREADME.md\nredir"..., 149) = 149
[pid 226742] close(1)                   = 0
[pid 226742] close(2)                   = 0
[pid 226742] +++ exited with 0 +++
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=226742, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
+++ exited with 0 +++
```
