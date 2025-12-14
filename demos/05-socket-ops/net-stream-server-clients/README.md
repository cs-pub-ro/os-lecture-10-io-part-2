# Run Multiple TCP Clients and Server

This example demonstrates running multiple TCP clients on a single server.

Build with:

```console
make
```

First run the server on one console:

```console
./server
```

Then run the client on another console:

```console
$ ./client
Sending message "gogogo"
Received message "gogogo"
Round-trip time: 294 microseconds

$ ./client
Sending message "gogogo"
Received message "gogogo"
Round-trip time: 313 microseconds

$ ./client
Sending message "gogogo"
Received message "gogogo"
Round-trip time: 316 microseconds
```

We get about 300 microseconds of round-trip-time on a `localhost` system (no actual networking involved).

We can run more clients using the `run_many_client.sh` script:

```console
$ ./run_many_clients.sh 100 > out.txt

$ grep microseconds out.txt | cut -d ' ' -f 3 | sort -n | tail -10
331
356
414
443
448
531
544
596
601
941

$ ./run_many_clients.sh 1000 > out.txt

$ grep microseconds out.txt | cut -d ' ' -f 3 | sort -n | tail -10
944
955
991
1070
1071
1136
1136
1357
207496
207572

$ ./run_many_clients.sh 10000 > out.txt

$ grep microseconds out.txt | cut -d ' ' -f 3 | sort -n | tail -10
209160
209213
209494
209574
415962
608352
608381
608463
608518
608564
```

We can see that for more connections, there are scalability problems with response time taking longer as server is busy.
This requires a redesign of the server side to make it better serve more clients, as is the case with [The C10k problem](https://www.kegel.com/c10k.html).
