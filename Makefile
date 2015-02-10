# makefile for hmmseg
# a execute program and dynamic shared library will be generated
# auth : aron
# date : 2015-02-03

cc=g++
target=test/wordseg
so_target=so/libhmmseg.so
obj=obj/main.o obj/util.o obj/wordseg.o obj/hmm.o obj/trie.o
so_obj=obj/util.o obj/wordseg.o obj/hmm.o obj/trie.o
CFLAGS= -O2 -g -I ./include
targets=$(target) $(so_target)

all : $(targets)
.PHONY : all

$(target) : $(obj)
	$(cc) -o $(target) $(obj)

$(so_target) : $(so_obj)
	$(cc) -fPIC -shared -o $(so_target) src/*.cpp -I./include/

obj/main.o : test/main.cpp 
	$(cc) -c test/main.cpp $(CFLAGS) -o obj/main.o

obj/trie.o : src/trie.cpp
	$(cc) -c src/trie.cpp $(CFLAGS) -o obj/trie.o

obj/wordseg.o : src/wordseg.cpp
	$(cc) -c src/wordseg.cpp $(CFLAGS) -o obj/wordseg.o

obj/hmm.o : src/hmm.cpp
	$(cc) -c src/hmm.cpp $(CFLAGS) -o obj/hmm.o

obj/util.o : src/util.cpp
	$(cc) -c src/util.cpp $(CFLAGS) -o obj/util.o


clean :
	rm $(target) obj/*.o
