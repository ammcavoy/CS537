#
# Sample makefile updated to include the commands to run the Clang Static Analyzer
#
#    scan-build will "make" your program and run CSA at the same time.
#
#    scan-view will do a scan-build (if necessary) and then open a web browser
#      window to review the results.
#
CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = 537make
SCAN_BUILD_DIR = scan-build-out
all: main.o parser.o graph.o proc.o
	$(CC) -o $(EXE) main.o parser.o graph.o proc.o
main.o: main.c parser.h
	$(CC) -g -O0 $(WARNING_FLAGS) -c main.c
parser.o: parser.c parser.h
	$(CC) -g -O0 $(WARNING_FLAGS) -c parser.c
graph.o: graph.c graph.h
	$(CC) -g -O0 $(WARNING_FLAGS) -c graph.c
proc.o: proc.c proc.h
	$(CC) -g -O0 $(WARNING_FLAGS) -c proc.c

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

#
# Run the Clang Static Analyzer
#
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#
# View the one scan available using firefox
#
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html 
