CFLAGS := -m32 -std=c99 -O2 -pipe -Wall -Wextra -pedantic -Wno-unused-function -Wno-unused-parameter
.PHONY: clean
te: te-obj text-obj
	$(CC) $(CFLAGS) -o bin/te obj/text.o obj/te.o
te-obj:
	$(CC) $(CFLAGS) -c -o obj/te.o src/te.c
text-obj:
	$(CC) $(CFLAGS) -c -o obj/text.o src/text.c
clean:
	rm -f obj/te.o
	rm -f obj/text.o
	rm -f bin/te
