CC = g++
THREAD = -pthread
FLAGS = -c $(THREAD)

ALL.O = util.o thread_routine.o main.o

all: $(ALL.O)
	$(CC) $(THREAD) $(ALL.O) -o prog

util.o: util.cpp util.h
	$(CC) $(FLAGS) util.cpp

thread_routine.o: thread_routine.cpp thread_routine.h util.h
	$(CC) $(FLAGS) thread_routine.cpp

main.o: main.cpp util.h thread_routine.h
	$(CC) $(FLAGS) main.cpp

clean:
	rm *.o *.out prog
