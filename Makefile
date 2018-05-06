CC = g++
CFLAG = -c

ALL.O = thread_routine.o util.o main.o

all: $(ALL.O)
	$(CC) thread_routine.o util.o main.o -o prog

util.o: util.cpp util.h
	$(CC) $(CFLAG) util.cpp

thread_routine.o: thread_routine.cpp thread_routine.h util.h util.o
	$(CC) $(CFLAG) thread_routine.cpp

main.o: main.cpp util.h util.o thread_routine.h thread_routine.o
	$(CC) $(CFLAG) main.cpp

clean:
	rm *.o *.out
