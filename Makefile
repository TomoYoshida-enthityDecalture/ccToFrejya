CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

ccToFreyja: $(OBJS)
	$(CC) -o ccToFreyja $(OBJS) $(LDFLSGS)

test: ccToFreyja
	./test.sh

clean: 
	rm -f ccToFreyja *.o *~ tmp*

.PHONY: test clean