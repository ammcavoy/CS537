# cs 537 proj1
# Adam McAvoy
#

CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = prodcomm 


all: main.o queue.o reader.o munch.o writer.o stats.o
	$(CC) -o $(EXE) main.o queue.o reader.o munch.o writer.o stats.o -pthread

main.o: main.c queue.h reader.h munch.h writer.h stats.h 
	$(CC) $(WARNING_FLAGS) -c main.c -pthread

queue.o: queue.c queue.h
	$(CC) $(WARNING_FLAGS) -c queue.c	

reader.o: reader.c reader.h
	$(CC) $(WARNING_FLAGS) -c reader.c

munch.o: munch.c munch.h 
	$(CC) $(WARNING_FLAGS) -c munch.c

writer.o: writer.c writer.h
	$(CC) $(WARNING_FLAGS) -c writer.c 

stats.o: stats.c stats.h
	$(CC) $(WARNING_FLAGS) -c stats.c

clean:
	rm -f $(EXE) *.o 
