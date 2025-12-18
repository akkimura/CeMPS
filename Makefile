PROG=simCeMPS
OBJS=$(patsubst %.c, %.o, $(wildcard *.c))
HEADERS=$(wildcard *.h)
CC=gcc
CFLAGS=-Wall -g -O0 -fopenmp -fcommon
LDFLAGS=-lm -fopenmp

.PHONY: all
all: TAGS $(PROG)
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(PROG)

TAGS: $(SRCS) $(HEADERS)
	ctags -Re
