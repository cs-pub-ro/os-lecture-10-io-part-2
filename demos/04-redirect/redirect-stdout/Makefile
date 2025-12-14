CPPFLAGS = -I../../utils
CFLAGS = -Wall -Wextra -Wno-unused -Wno-unused-parameter -g

.PHONY: all clean

all: redirect_stdout redirect_child_stdout

redirect_stdout: redirect_stdout.o

redirect_stdout.o: redirect_stdout.c ../../utils/utils.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

redirect_child_stdout: redirect_child_stdout.o

redirect_child_stdout.o: redirect_child_stdout.c ../../utils/utils.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f *~
	-rm -f redirect_stdout.o redirect_stdout
	-rm -f redirect_child_stdout.o redirect_child_stdout
