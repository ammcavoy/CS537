# Instructions:
#
# run "537make"
# run "touch module2.c"
# run "537make"
#
# The output of the second "537make" command should be:
# 	Building module2.o
# 	Building program

program: main.o module1.o module2.o
	echo "Building program"
	gcc -o program main.o module1.o module2.o

main.o: main.c module1.h module2.h
	echo "Building main.o"
	gcc -c main.c

module1.o: module1.c module1.h
	echo "Building module1.o"
	gcc -c module1.c

module2.o: module2.c module2.h
	echo "Building module2.o"
	gcc -c module2.c

clean:
	rm -f program main.o module1.o module2.o
