prog2:main2.o queue.o reader.o parser.o
	echo Building prog2
	gcc -o prog2 main2.o queue.o reader.o parser.o
	echo prog2 build successfully

prog1 : main1.o queue.o reader.o writer.o
	echo Building prog1

	gcc -o prog1 main1.o queue.o reader.o writer.o

	echo prog1 build successfully


all: prog1 prog2 prog3


prog3: main3.o process1.o process2.o graph.o djikstra.o
	echo Building prog3
	gcc -o prog3 main3.o process1.o process2.o graph.o djikstra.o
	echo prog3 built successfully

main1.o: main1.c queue.h reader.h writer.h
	echo Building main1.o!
	gcc -c main1.c

main1.c   :   main_1_draft.c writer.o
	cp main_1_draft.c main1.c

reader.o : reader.c reader.h queue.h
	echo Building reader.o!
	gcc -c reader.c

main2.o: main2.c queue.h writer.h
	echo Building main2.o!
	gcc -c main2.c
queue.o:queue.c queue.h
	echo Building queue.o!
	gcc -c queue.c
writer.o:writer.c writer.h queue.h
	echo Building writer.o!
	gcc -c writer.c
parser.o: parser.c parser.h
	echo Building Parser!
	gcc -c parser.c
queue.h: graph.o

main3.o: main3.c process1.h process2.h graph.h djikstra.h
	echo Building main3.o!
	gcc -c main3.c


process1.o: process1.c process1.h graph.h djikstra.h
	echo Building process1.o!
	gcc -c process1.c

process2.o: process2.c process2.h graph.h djikstra.h
	echo Building process2.o!
	gcc -c process2.c

graph.o    :    graph.c   graph.h main1.o
	echo BUILDING graph.o !!!!! !!!!!
	gcc -c graph.c
djikstra.o: djikstra.c djikstra.h graph.h
	echo building djikstra.o!
	gcc -c djikstra.c

clean_objs:
	rm main1.o
	rm reader.o
	rm writer.o
	rm queue.o
	rm main2.o
	rm parser.o
	rm main3.o
	rm process1.o
	rm process2.o
	rm graph.o
	rm djikstra.o
	rm main1.c
clean_prog1:
	rm prog1
clean_prog2:
	rm prog2
clean_prog3:
	rm prog3

clean: clean_objs clean_prog1 clean_prog2 clean_prog3
