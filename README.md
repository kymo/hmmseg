# hmmseg
hmm for segmentation : leverage the maximum match strategy and the hmm model for chinese word's segmentation with great robust and acceptable precision.

1. use trie tree to store the dict and supply a interface for quick search for all the matched segmention by maximum match strategy
2. use hmm model to assess the score of each match, and leverage the one with max score.
3. hmm model is train by statistics, em training algorithm will be updated soon.
