CC=g++ -std=c++11
CFLAGS=-Wall -g
OFLAGS=-c

.PHONY: all clean tags

all: main.out preprocess.out

tags:
	cscope -Rb
	ctags -R ./*

main.out: data.h SHindex.o SHgeneral.o SHselection.o statisticsmodule.o knn.o io.o myrandom.o myvector.o main.cpp
	$(CC) $(CFLAGS) main.cpp knn.o statisticsmodule.o SHindex.o SHgeneral.o SHselection.o io.o myrandom.o myvector.o -o main.out

preprocess.out: constants.h io.o preprocess.cpp constants.h
	$(CC) $(CFLAGS) preprocess.cpp io.o -o preprocess.out

SHindex.o: constants.h data.h knn.o myvector.h myrandom.h SHindex.cpp SHindex.h
	$(CC) $(CFLAGS) $(OFLAGS) SHindex.cpp -o SHindex.o

SHgeneral.o: constants.h data.h myvector.h myrandom.h SHgeneral.cpp SHgeneral.h
	$(CC) $(CFLAGS) $(OFLAGS) SHgeneral.cpp -o SHgeneral.o

SHselection.o: constants.h data.h knn.o SHselection.cpp SHselection.h
	$(CC) $(CFLAGS) $(OFLAGS) SHselection.cpp -o SHselection.o

knn.o: constants.h data.h myvector.h knn.cpp knn.h
	$(CC) $(CFLAGS) $(OFLAGS) knn.cpp -o knn.o

statisticsmodule.o: constants.h data.h knn.o statisticsmodule.cpp statisticsmodule.h
	$(CC) $(CFLAGS) $(OFLAGS) statisticsmodule.cpp -o statisticsmodule.o

myvector.o: constants.h myvector.h myvector.cpp
	$(CC) $(CFLAGS) $(OFLAGS) myvector.cpp -o myvector.o

myrandom.o: constants.h myrandom.h myrandom.cpp
	$(CC) $(CFLAGS) $(OFLAGS) myrandom.cpp -o myrandom.o

io.o: constants.h io.h io.cpp
	$(CC) $(CFLAGS) $(OFLAGS) io.cpp -o io.o

clean:
	rm -f *.o *.out *.gch *.dat *.txt tags
