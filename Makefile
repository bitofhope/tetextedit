CFLAGS := -m32 -std=c99 -O2 -pipe -Wall -Wextra -pedantic -Wno-unused-function -Wno-unused-parameter
.PHONY: clean
te: te-obj
	$(CC) $(CFLAGS) -o bin/te obj/te.o
te-obj:
	$(CC) $(CFLAGS) -c -o obj/te.o src/te.c
clean:
	rm -f obj/te.o
	rm -f bin/te
