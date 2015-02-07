// Copyright (C) 2010, 2011, 2012, 2013, 2014, 2015 Aron
// contact: kymowind@gmail.com www.idiotaron.org
//
// This file is part of hmmseg
//
// hmmseg is a segmentation module conbined hidden markov model with
// maximum match segmentation, you can redistribute it and modify it
// under the term of the GNU Genural Public License as published by
// the free software Foundation, either version of 3 of the Lisence
// 
// In the project of hmmseg, we use the trie tree to store the dict
// so as to reduce the memory which speed up the search for word in
//
// hmm.h: the definiton of the main model 


#ifndef __TRIE_H_
#define __TRIE_H_


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>

namespace hmmseg {
class Trie {
private:

	Trie* _root;

	std::map<std::string, Trie*> _child_trees;
	
	// build a trie tree from the dict
	void build_tree(Trie *&trie, const std::vector<std::string> &words, int cur);
	// split the words in a std::string
	bool split_ch_words(const std::string &line, std::vector<std::string> &words);

public:
	bool _is_string_node;
	std::string _word;
	Trie() : _is_string_node(false) {}
	Trie(std::string word) : _word(word), _is_string_node(false) {
	}

	// display trees
	void display(Trie *&trie);

	// set std::string node
	void set_string_node(bool is_string_node);

	// load dict()
	// each word in a line which's decoding as UTF-8
	bool load_dict(const char* file_name);

	// simple segment
	void simple_seg(std::string &str);

	// search whether the current word is a child node of the
	// current node thus judging whether to end the search
	bool search(Trie * &tree, const std::vector<std::string> &word, int i, int j);

};
}

#endif
