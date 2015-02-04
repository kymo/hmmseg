cc=g++
target=hmm
obj=hmm.o main.o
$(target) : $(obj)
	$(cc) -o $(target) $(obj)

main.o : main.cpp
	$(cc) -c main.cpp

hmm.o : hmm.cpp
	$(cc) -c hmm.cpp
clean :
	rm *.o hmm
