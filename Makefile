CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

ccToFrejya: $(OBJS)
	$(CC) -o ccToFrejya $(OBJS) $(LDFLSGS)

test: ccToFrejya
	./test.sh

clean: 
	rm -f ccToFrejya *.o *~ tmp*

.PHONY: test clean