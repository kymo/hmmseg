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
//
// trie.cpp: the definiton of the tree structure to save the dict for
//		minimize the store of the data and quick search for the word 
//		in the dict.
//

#include "trie.h"
#include "util.h"

namespace hmmseg {
namespace trie {

void Trie::set_string_node(bool is_string_node) {
	_is_string_node = is_string_node;
}

void Trie::build_tree(Trie *&root, const std::vector<std::string> &words, int cur) {

	if (cur >= words.size()) return;
	std::string cur_word = words[cur];
	
	if (root->_child_trees.find(cur_word) == root->_child_trees.end()) {
		root->_child_trees.insert(std::pair<std::string, Trie*>(cur_word, new Trie(cur_word)));
	}
	
	if (cur + 1 == words.size()) {
		root->_child_trees[cur_word]->set_string_node(true);
		return ;
	}
	build_tree(root->_child_trees[cur_word], words, cur + 1);
}


void Trie::display(Trie *&trie) {
	std::queue<Trie*> q;
	q.push(trie);
	while (! q.empty()) {
		Trie *tp = q.front();
		q.pop();
		if (tp->_word != "") {
			std::cout << "father: " << tp->_word << std::endl;
		} else {
			std::cout << "father: root!" << std::endl;
		}
		for (std::map<std::string, Trie*>::iterator it = tp->_child_trees.begin(); it != tp->_child_trees.end(); it ++) {
			std::cout << it->first << " ";
			q.push(it->second);
		}
		std::cout << std::endl;
	}

}

bool Trie::load_dict(const char* file_name) {
	// TODO judge the encoding of the file
	std::ifstream fis(file_name);
	std::string line;
	int line_num;
	line_num = 0;
	_root = new Trie();
	std::map<std::string, int> dic;
	while (getline(fis, line)) {
		std::vector<std::string> words;
		if (! hmmseg::util::split_ch_words(line, words)) {
			std::cerr << "Error when split the words in line : " << line_num << std::endl;
			return false;
		}
		build_tree(_root, words, 0);
	}
	return true;
}

bool Trie::find_all_results(std::string &str, 
		std::vector<std::vector<std::string> > &results) {
	int len;
	std::vector<std::string> words;
	std::vector<std::string> temp_results;
	hmmseg::util::split_ch_words(str, words);
	len = words.size();
	dfs_search(0, len - 1, words, temp_results, results);
	return true;
}

void Trie::dfs_search(int i, int j,
		const std::vector<std::string> &words,
		std::vector<std::string> &temp_results,
		std::vector<std::vector<std::string> > &results) {
	
	if (i > j)  {	
		results.push_back(temp_results);
		return ;
	}
	
	int k;
	std::string temp_str;
	std::vector<int> indexes;
	Trie *tree = _root;
	if (tree->_child_trees.find(words[i]) != tree->_child_trees.end()) {
		tree = tree->_child_trees[words[i]];
		for (k = i + 1; k <= j; k ++) {	
			if (tree->_child_trees.find(words[k]) != tree->_child_trees.end()) {
				if (tree->_child_trees[words[k]]->_is_string_node) {
					indexes.push_back(k);
				}
				tree = tree->_child_trees[words[k]];
			} else {
				break;
			}
		}
	}
	
	if (indexes.size() == 0) {
		indexes.push_back(i);
	}
	
	int last = -1;
	for (int k = 0; k < indexes.size(); k ++) {
		temp_str = "";
		for (int l = i; l <= indexes[k]; l ++) {
			temp_str += words[l];
		}
		temp_results.push_back(temp_str);
		dfs_search(indexes[k] + 1, j, words, temp_results, results);
		temp_results.pop_back();
	}

}

void Trie::simple_seg(std::string &str) { 
	std::vector<std::string> ret;
	std::vector<std::string> words;
	std::string trunk;
	int start , ends;
	hmmseg::util::split_ch_words(str, words);
	start = 0, ends = words.size();
	ends --;
	while (start <= ends) {
		bool tag = false;
		for (int k = start; k <= ends; k ++) {
			if (search(_root, words, start, ends - k + start)) {
				trunk = "";	
				for (int j = start;j <= ends - k + start; j ++) {
					trunk += words[j];
				}
				ret.push_back(trunk);
				start = ends - k + start + 1;
				ends = words.size() - 1;
				tag = true;
				break;
			} 
		}
		if (! tag) {
			ret.push_back(words[start]);	
			start += 1;
		} 
	}
}


bool Trie::search(Trie *&tree, 
		const std::vector<std::string> &words, 
		int i, int j) {
	if (i > j) return false;	
	if (i == j) {
		if (tree->_child_trees.find(words[i]) != tree->_child_trees.end()) {
			return tree->_child_trees[words[i]]->_is_string_node;
		}
	}
	if (tree->_child_trees.find(words[i]) == tree->_child_trees.end()) {
		return false;
	}	
	return search(tree->_child_trees[words[i]], words, i + 1, j);
}
}
}
