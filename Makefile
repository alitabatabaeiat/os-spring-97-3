CC = g++
THREAD = -pthread -std=c++11
FLAGS = -c $(THREAD)

ALL.O = util.o main.o

all: $(ALL.O)
	$(CC) $(THREAD) $(ALL.O) -o prog

util.o: util.cpp util.h
	$(CC) $(FLAGS) util.cpp

main.o: main.cpp util.h
	$(CC) $(FLAGS) main.cpp

clean:
	rm *.o *.out prog
