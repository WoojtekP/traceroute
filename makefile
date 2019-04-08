CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror

BINS = traceroute

all: $(BINS)

$(BINS):
	$(CC) $(CFLAGS) -o $@ $^

traceroute: traceroute.o common.o send.o receive.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o

distclean:
	rm -f *.o $(BINS) 