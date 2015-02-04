cc=g++
target=hmm
obj=main.o wordseg.o hmm.o
$(target) : $(obj)
	$(cc) -o $(target) $(obj)

main.o : main.cpp
	$(cc) -c main.cpp

hmm.o : hmm.cpp
	$(cc) -c hmm.cpp

wordseg.o : wordseg.cpp
	$(cc) -c wordseg.cpp

clean :
	rm *.o hmm
