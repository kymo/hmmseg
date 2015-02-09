# hmmseg
hmm for segmentation : leverage the maximum match strategy and the hmm model for chinese word's segmentation with great robust and acceptable precision.

1. use trie tree to store the dict and supply a interface for quick search for all the matched segmention by maximum match strategy
2. use hmm model to assess the score of each match, and leverage the one with max score.
3. hmm model is trained by statistics, em training algorithm will be updated soon.
4. this segmentation method will be robust engough for your application, and especially when you apply it to long document segmentation. the speed and outcome will shine you eyes~
5. you can use the whole project or just the dynamic shared library in the test directory, and the example is also given in the test_so.cpp , you just need to copy the compiled libhmmseg.so to the /usr/local/lib, and type into the command "ldconfig or sudo ldconfig", then compile you own file with "g++ -o seg test.cpp -lhmmseg -I../include/"
6. email me(kymowind@gmail.com) if you have any problem with it or idiot bugs that i left in the program, thanks.
