CC=gcc
CFLAGS=-I. -lm -g
DEPS = variates.h


OBJ = variates.o SDV_sim.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

SDV: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o SDV
