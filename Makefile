ccToFrejya: ccToFrejya.c
	gcc -o ccToFrejya -g -static ccToFrejya.c

test: ccToFrejya
	./test.sh

clean: 
	rm -f ccToFrejya *.o *~ tmp*

