CFLAGS=-lm

build: bitwise

bitwise: ../bitwise.c
	gcc ../bitwise.c $(CFLAGS) -o ../bitwise

clean:
	rm -f ../bitwise

clean_all:
	rm -f ../bitwise
	rm -rf tests_out
