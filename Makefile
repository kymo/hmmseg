cc=g++
target=wordseg
obj=obj/main.o obj/wordseg.o obj/hmm.o
CFLAGS= -O2 -I ./include


$(target) : $(obj)
	$(cc) -o $(target) $(obj)

obj/main.o : test/main.cpp 
	$(cc) -c test/main.cpp $(CFLAGS) -o obj/main.o

obj/hmm.o : src/hmm.cpp
	$(cc) -c src/hmm.cpp $(CFLAGS) -o obj/hmm.o

obj/wordseg.o : src/wordseg.cpp
	$(cc) -c src/wordseg.cpp $(CFLAGS) -o obj/wordseg.o

clean :
	rm $(target) obj/*.o
