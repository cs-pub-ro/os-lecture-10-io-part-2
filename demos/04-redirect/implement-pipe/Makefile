CPPFLAGS = -I../../utils
CFLAGS = -Wall -Wextra -Wno-unused -Wno-unused-parameter -g

.PHONY: all clean

all: manager reader writer

manager: manager.o

manager.o: manager.c ../../utils/utils.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

reader: reader.o

reader.o: reader.c ../../utils/utils.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

writer: writer.o

writer.o: writer.c ../../utils/utils.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f *~
	-rm -f manager.o manager
	-rm -f reader.o reader
	-rm -f writer.o writer
