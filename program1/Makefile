# cs 537 proj1
# Adam McAvoy
#

CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = 537ps


all: 537ps.o getprocs.o parsecmdline.o parsestat.o
	$(CC) -o $(EXE) 537ps.o getprocs.o parsecmdline.o parsestat.o

537ps.o: 537ps.c getprocs.h parsecmdline.h parsestat.h
	$(CC) $(WARNING_FLAGS) -c 537ps.c

getprocs.o: getprocs.c getprocs.h
	$(CC) $(WARNING_FLAGS) -c getprocs.c	

parsecmdline.o: parsecmdline.c parsecmdline.h
	$(CC) $(WARNING_FLAGS) -c parsecmdline.c

parsestat.o: parsestat.o parsestat.h
	$(CC) $(WARNING_FLAGS) -c parsestat.c

clean:
	rm -f $(EXE) *.o 
