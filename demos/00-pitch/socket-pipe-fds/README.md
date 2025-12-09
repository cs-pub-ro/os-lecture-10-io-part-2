# Inter-Process Channels

File descriptors point to both device-endpoint channels and process-endpoint channels (e.g. sockets and pipes).
Common complex applications (such as chat applications, browsers, music players) end up using a diverse set of file descriptors.

For example, we check the file descriptor table for the Discord process:

```console
$ lsof -p $(pidof -s Discord) -a -d 0-1023
COMMAND    PID   USER FD      TYPE             DEVICE SIZE/OFF    NODE NAME
Discord 179400 razvan  0r      CHR                1,3      0t0       5 /dev/null
Discord 179400 razvan  1u     unix 0x0000000000000000      0t0    6038 type=STREAM (CONNECTED)
Discord 179400 razvan  2u     unix 0x0000000000000000      0t0    6039 type=STREAM (CONNECTED)
Discord 179400 razvan  3u  a_inode               0,16        0      70 [eventpoll:12]
Discord 179400 razvan  4u     unix 0x0000000000000000      0t0 1623176 type=SEQPACKET (CONNECTED)
Discord 179400 razvan  5r      REG                7,3 10467680     380 /snap/discord/262/usr/share/discord/icudtl.dat
Discord 179400 razvan  6r      REG                7,3   718769     450 /snap/discord/262/usr/share/discord/v8_context_snapshot.bin
Discord 179400 razvan  7r      REG                7,3   167677     375 /snap/discord/262/usr/share/discord/chrome_100_percent.pak
Discord 179400 razvan  8r      REG                7,3   259832     376 /snap/discord/262/usr/share/discord/chrome_200_percent.pak
Discord 179400 razvan  9r      REG                7,3   497937     398 /snap/discord/262/usr/share/discord/locales/en-US.pak
Discord 179400 razvan 10r      REG                7,3  6023504     448 /snap/discord/262/usr/share/discord/resources.pak
Discord 179400 razvan 11r      CHR                1,9      0t0      10 /dev/urandom
Discord 179400 razvan 12u  a_inode               0,16        0      70 [eventfd:55]
Discord 179400 razvan 13u     unix 0x0000000000000000      0t0 1609918 type=STREAM (CONNECTED)
Discord 179400 razvan 14u     unix 0x0000000000000000      0t0 1615848 @6e5e2 type=SEQPACKET (CONNECTED)
Discord 179400 razvan 15u  a_inode               0,16        0      70 [eventfd:56]
Discord 179400 razvan 16r     FIFO               0,15      0t0 1600332 pipe
Discord 179400 razvan 17w     FIFO               0,15      0t0 1600332 pipe
Discord 179400 razvan 18r      REG               0,24     4096   21244 /sys/devices/virtual/tty/tty0/active
Discord 179400 razvan 19u     unix 0x0000000000000000      0t0 1600333 type=STREAM (CONNECTED)
Discord 179400 razvan 20u     unix 0x0000000000000000      0t0 1600334 type=STREAM (CONNECTED)
Discord 179400 razvan 21u     unix 0x0000000000000000      0t0 1613225 type=STREAM (CONNECTED)
Discord 179400 razvan 22u      CHR            226,129      0t0    1311 /dev/dri/renderD129
Discord 179400 razvan 23u      CHR            226,129      0t0    1311 /dev/dri/renderD129
Discord 179400 razvan 24u      CHR            226,129      0t0    1311 /dev/dri/renderD129
Discord 179400 razvan 25u      CHR            226,129      0t0    1311 /dev/dri/renderD129
Discord 179400 razvan 26u      CHR            226,128      0t0    1303 /dev/dri/renderD128
Discord 179400 razvan 27u  a_inode               0,16        0      70 [eventpoll:31]
Discord 179400 razvan 28u      CHR            226,129      0t0    1311 /dev/dri/renderD129
Discord 179400 razvan 29u      CHR            226,129      0t0    1311 /dev/dri/renderD129
Discord 179400 razvan 30u      CHR            226,129      0t0    1311 /dev/dri/renderD129
Discord 179400 razvan 31u  a_inode               0,16        0      70 [eventfd:64]
Discord 179400 razvan 32u  a_inode               0,16        0      70 [eventpoll:13,33,42]
Discord 179400 razvan 33u  a_inode               0,16        0      70 [eventfd:65]
Discord 179400 razvan 34u     unix 0x0000000000000000      0t0 1623285 type=STREAM (CONNECTED)
Discord 179400 razvan 35u      REG               0,28        4 1078423 /dev/shm/.org.chromium.Chromium.NWJdDh (deleted)
Discord 179400 razvan 36u  a_inode               0,16        0      70 [eventfd:66]
Discord 179400 razvan 37r     FIFO               0,15      0t0 1621428 pipe
Discord 179400 razvan 38w     FIFO               0,15      0t0 1621428 pipe
[...]
```

We see, looking at the `TYPE` column that apart from the common `REG` type (_regular file_) there are other types:

- `FIFO`: is used for pipes: simple channels to communicate with other processes
- `unix`: is for UNIX sockets: complex bidirectional channels used to communicate locally

Another type are network sockets, used to communicate between applications located on different hosts.
We can list network sockets by opening a simple server and a simple client (in two different consoles).
We open a TCP simple server with `nc` (_netcat_):

```console
nc -l -p 12345
```

On another console, we open the client:

```console
nc localhost 12345
```

We can write a message on the client console, press `Enter`, and see that message delivered to the server.

Finally, on another console, we can see the open file descriptors for both the server and the client processes:

```console
$ ps -f -C nc
UID          PID    PPID  C STIME TTY          TIME CMD
razvan    310492  204208  0 08:44 pts/30   00:00:00 nc -l -p 12345
razvan    310698  310494  0 08:44 pts/32   00:00:00 nc localhost 12345

$ lsof -p 310492 -a -d 0-1023
COMMAND    PID   USER FD   TYPE  DEVICE SIZE/OFF NODE NAME
nc      310492 razvan 0u   CHR  136,30      0t0   33 /dev/pts/30
nc      310492 razvan 1u   CHR  136,30      0t0   33 /dev/pts/30
nc      310492 razvan 2u   CHR  136,30      0t0   33 /dev/pts/30
nc      310492 razvan 3u  IPv4 2815744      0t0  TCP *:12345 (LISTEN)
nc      310492 razvan 4u  IPv4 2815745      0t0  TCP localhost:12345->localhost:45126 (ESTABLISHED)

$ lsof -p 310698 -a -d 0-1023
COMMAND    PID   USER FD   TYPE  DEVICE SIZE/OFF NODE NAME
nc      310698 razvan 0u   CHR  136,32      0t0   35 /dev/pts/32
nc      310698 razvan 1u   CHR  136,32      0t0   35 /dev/pts/32
nc      310698 razvan 2u   CHR  136,32      0t0   35 /dev/pts/32
nc      310698 razvan 3u  IPv4 2832204      0t0  TCP localhost:45126->localhost:12345 (ESTABLISHED)
```

We can see that:

- The server has a listening network socket file descriptor: `FD: 3`, type `IPv4`.
  It is listening on TCP port `12345`.
- The server and the client are connected by a pair of network socket file descriptors (type `IPv4`) with the connection marked as `ESTABLISHED` 
  The connection is between port `45216` on the client side and port `12345` on the server side.

You can also have an IPv6 connection (and then check it with `lsof`).
To create an IPv6 connection use:

```console
# server
nc -6 -l -p 12345
# client
nc -6 ::1 12345
```

So, `lsof` points us to the file descriptors used by a process, some of which are process-endpoint file descriptors (pipes and sockets).
