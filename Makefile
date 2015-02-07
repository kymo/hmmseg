cc=g++
target=trie
obj=obj/main.o obj/wordseg.o obj/hmm.o obj/trie.o
CFLAGS= -O2 -I ./include




$(target) : $(obj)
	$(cc) -o $(target) $(obj)

obj/main.o : test/main.cpp 
	$(cc) -c test/main.cpp $(CFLAGS) -o obj/main.o

obj/trie.o : src/trie.cpp
	$(cc) -c src/trie.cpp $(CFLAGS) -o obj/trie.o


obj/hmm.o : src/hmm.cpp
	$(cc) -c src/hmm.cpp $(CFLAGS) -o obj/hmm.o

obj/wordseg.o : src/wordseg.cpp
	$(cc) -c src/wordseg.cpp $(CFLAGS) -o obj/wordseg.o

clean :
	rm $(target) obj/*.o
