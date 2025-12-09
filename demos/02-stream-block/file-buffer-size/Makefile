CPPFLAGS = -I../../utils
CFLAGS = -Wall -Wextra -Wno-unused -Wno-unused-parameter -g

.PHONY: all clean

all: write_file

write_file: write_file.o

write_file.o: write_file.c ../../utils/utils.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f *~
	-rm -f write_file.o write_file
