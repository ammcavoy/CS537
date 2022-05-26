# cs 537 proj1
# Adam McAvoy
#

CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE1 = 537pfsim-fifo
EXE2 = 537pfsim-lru
EXE3 = 537pfsim-clock

all: fifo.o lru.o clock.o parsecmdline.o
	$(CC) -o $(EXE1) fifo.o parsecmdline.o 
	$(CC) -o $(EXE2) lru.o parsecmdline.o
	$(CC) -o $(EXE3) clock.o parsecmdline.o

lru.o: lru.c parsecmdline.h
	$(CC) $(WARNING_FLAGS) -c lru.c

clock.o: clock.c parsecmdline.h
	$(CC) $(WARNING_FLAGS) -c clock.c

fifo.o: fifo.c parsecmdline.h 
	$(CC) $(WARNING_FLAGS) -c fifo.c

parsecmdline.o: parsecmdline.c parsecmdline.h
	$(CC) $(WARNING_FLAGS) -c parsecmdline.c

clean:
	rm -f $(EXE) *.o 
