CC=gcc
CFLAGS=-lwiringPi -lstdc++
DEPS=switch.hpp

OBJ=switch.o switch_test.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

radio: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)


.PHONY: clean

clean:
	rm -f *.o *~ core  
