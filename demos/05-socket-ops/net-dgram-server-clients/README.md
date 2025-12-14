# Run Multiple UDP Clients and Server

This example demonstrates running multiple UDP clients on a single server.

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
160
177
190
206
209
245
265
303
483
660

$ ./run_many_clients.sh 1000 > out.txt

$ grep microseconds out.txt | cut -d ' ' -f 3 | sort -n | tail -10
585
588
592
640
670
681
725
756
875
921

$ ./run_many_clients.sh 10000 > out.txt

$ grep microseconds out.txt | cut -d ' ' -f 3 | sort -n | tail -10
611
627
666
668
680
698
706
927
961
1041

$ ./run_many_clients.sh 20000 > out.txt

$ grep microseconds out.txt | cut -d ' ' -f 3 | sort -n | tail -10
705
708
712
713
723
727
733
758
770
807
```

We can see that UDP scales really well, and there is no great delay in packet processing, even for `20000` clients.
