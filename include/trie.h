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


using namespace std;

class Trie {
private:

	Trie* _root;

	map<string, Trie*> _child_trees;
	
	// build a trie tree from the dict
	void build_tree(Trie *&trie, const vector<string> &words, int cur);
	// split the words in a string
	bool split_ch_words(const string &line, vector<string> &words);

public:
	bool _is_string_node;
	string _word;
	Trie() : _is_string_node(false) {}
	Trie(string word) : _word(word), _is_string_node(false) {
	}

	// display trees
	void display(Trie *&trie);

	// set string node
	void set_string_node(bool is_string_node);

	// load dict()
	// each word in a line which's decoding as UTF-8
	bool load_dict(const char* file_name);

	// simple segment
	void simple_seg(string &str);

	// search whether the current word is a child node of the
	// current node thus judging whether to end the search
	bool search(Trie * &tree, const vector<string> &word, int i, int j);

};

#endif
