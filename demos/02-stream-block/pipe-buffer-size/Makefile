CPPFLAGS = -I../../utils
CFLAGS = -Wall -Wextra -Wno-unused -Wno-unused-parameter -g

.PHONY: all clean

all: send_receive_pipe

send_receive_pipe: send_receive_pipe.o

send_receive_pipe.o: send_receive_pipe.c ../../utils/utils.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f *~
	-rm -f send_receive_pipe.o send_receive_pipe
