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
// trie.h: the definiton of the tree

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
	// brief : build a trie tree from the dict
	// param : 
	// 		trie : a trie object
	//		words : a vector stored the ch words
	//		cur : the index of current word 
	// return :
	// 		None
	void build_tree(Trie *&trie, const std::vector<std::string> &words, int cur);
	
	// brief : split the chinese word to single one
	// param : 
	//		line : the chinese string
	//		words : a vector store the outcome
	// return : 
	//		None
	bool split_ch_words(const std::string &line, std::vector<std::string> &words);

public:
	bool _is_string_node;
	std::string _word;
	
	Trie() : _is_string_node(false) {}
	Trie(std::string word) : _word(word), _is_string_node(false) {}
	// display trees
	void display(Trie *&trie);


	// brief : given the string, find out all the possible segment result 
	// Param : 
	//		str string stored all the results
	//		results vector stored the results
	// return :
	// 		None
	bool find_all_results(std::string &str, 
			std::vector<std::vector<std::string> > &results);
	
	// brief : dfs seach
	void dfs_search(int i, int j,
			Trie *&tree,
			const std::vector<std::string> &words,
			std::vector<std::string> &temp_results,
			std::vector<std::vector<std::string> > &results) ;

	// brief : set the node status
	void set_string_node(bool is_string_node);

	// brief : load the dict to build a tree 
	// note  : each word in a line which's decoding as UTF-8
	// param : 
	//		file_name : the dict file name
	// return :
	//		false : load dict failed
	//		true : load dict successed
	bool load_dict(const char* file_name);

	// brief : simple segment method just using the maximum match strategy
	// param :
	//		str : the string gona to be segmented
	// return :
	//		None
	void simple_seg(std::string &str);

	// brief : search whether the current word is a child node of the 
	//		current node thus judging whether to end the search
	
	bool search(Trie * &tree, const std::vector<std::string> &word, int i, int j);

};
}

#endif
