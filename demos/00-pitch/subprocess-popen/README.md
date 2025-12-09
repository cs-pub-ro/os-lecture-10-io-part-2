# Pipes (and Subprocess, in Python)

The basic inter-process channel is the (anonymous) pipes.
Pipes are the basics for implementing the `|` (_pipe_) operator in shell (duh!).

A pipe is used to send data unidirectionally from a process to another process.
If we need bidirectional transfer, we will use two pipes.

Pipes are used by the [`popen()` Unix function](https://man7.org/linux/man-pages/man3/popen.3.html) and by the [`subprocess.Popen()` Python function](https://docs.python.org/3/library/subprocess.html#subprocess.Popen).

`run_grep.py` is a Python program that runs `grep` tool and then uses a pipe to capture its output and process it.
We have two processes:

1. The `grep` process is started and its output is sent to the pipe.
1. The Python process starts the `grep` process and then reads its output from the pipe.

The program uses `subprocess.Popen()` to create a `grep` process that reads input from `/etc/passwd` and that sends data (as a replacement for standard output) to a pipe:

```Python
    with subprocess.Popen(["grep", "a", "/etc/passwd"], stdout=subprocess.PIPE) as proc:
```

Run the Python program using:

```console
python run_grep.py
```

While the program waits for your input, inspect the two processes on another console:

```console
$ ps -ef | grep grep
razvan    312243   10007  0 09:02 pts/8    00:00:00 python run_grep.py
razvan    312244  312243  0 09:02 pts/8    00:00:00 [grep] <defunct>
razvan    312261  310699  0 09:02 pts/33   00:00:00 grep --color=auto grep

$ lsof -p 312243 -a -d 0-1023
COMMAND    PID   USER FD   TYPE DEVICE SIZE/OFF    NODE NAME
python  312243 razvan 0u   CHR  136,8      0t0      11 /dev/pts/8
python  312243 razvan 1u   CHR  136,8      0t0      11 /dev/pts/8
python  312243 razvan 2u   CHR  136,8      0t0      11 /dev/pts/8
python  312243 razvan 3r  FIFO   0,15      0t0 2803507 pipe

$ lsof -p 312244 -a -d 0-1023
```

We draw the following observations from the output above:

- The `grep` process (PID: `312244`) terminates after reading all contents of its input file `/etc/passwd`.
  It remains as a zombie (`<defunct>`) process.
- The Python process (PID: `312243`) has a pipe file descriptor where it reads from.
  This pipe is / was used by both processes:
  the `grep` process sent data to the pipe;
  the Python process read data from the pipe.

In order to get an actual channel on two running processes, we use the `run_grep_input.py` program.
In this program, the `grep` reads data from standard input (directed from a pipe) and sends data to standard output (redirected to another pipe).
This is achieved via the `subprocess.Popen()` call:

```python
    with subprocess.Popen(["grep", "a"], stdin=subprocess.PIPE, stdout=subprocess.PIPE) as proc:
```

Run the Python program using:

```console
python run_grep.py
```

While the program waits for your input, inspect the two processes on another console:

```console
$ ps -ef | grep grep
razvan    312497   10007  0 09:07 pts/8    00:00:00 python run_grep_input.py
razvan    312498  312497  0 09:07 pts/8    00:00:00 grep a
razvan    312533  310699  0 09:07 pts/33   00:00:00 grep --color=auto grep

$ lsof -p 312497 -a -d 0-1023
COMMAND    PID   USER FD   TYPE DEVICE SIZE/OFF    NODE NAME
python  312497 razvan 0u   CHR  136,8      0t0      11 /dev/pts/8
python  312497 razvan 1u   CHR  136,8      0t0      11 /dev/pts/8
python  312497 razvan 2u   CHR  136,8      0t0      11 /dev/pts/8
python  312497 razvan 4w  FIFO   0,15      0t0 2836980 pipe
python  312497 razvan 5r  FIFO   0,15      0t0 2836981 pipe

$ lsof -p 312498 -a -d 0-1023
COMMAND    PID   USER FD   TYPE DEVICE SIZE/OFF    NODE NAME
grep    312498 razvan 0r  FIFO   0,15      0t0 2836980 pipe
grep    312498 razvan 1w  FIFO   0,15      0t0 2836981 pipe
grep    312498 razvan 2u   CHR  136,8      0t0      11 /dev/pts/8
```

We draw the following observations from the output above:

- The `grep` process (PID: `312298`) has its standard input file descriptor (`FD: 0`) redirected from a pipe (`NODE: 2836980`).
  It also has its standard output descriptor (`FD: 1`) redirected to a pipe (`NODE: 2836981`).
- The Python process (PID: `312243`) has two pipe file descriptors:
  a. `FD: 4` is connected to the pipe that is read by the `grep` process (the replacement for `stdin` in `grep`).
  b. `FD: 5` is connected to the pipe that is written to by the `grep` process (the replacement for `stdout` in `grep`).
- The Python process writes to the pipe that is used as standard input by `grep`.
  Conversely, the Python process reads from the pipe that is used as standard output by `grep`.

So, in this program, `subprocess.Popen()` creates two unidirectional pipes, to achieve bidirectional transfer between the Python process and the `grep` process.
