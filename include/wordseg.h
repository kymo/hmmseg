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
// thisfile is used to do wordseg model training and also
// supply an interface to operate word segmentation .
// the training file comes from the http://sighan.cs.uchicago.edu/bakeoff2005/
// thanks to the great contributions of these geniuses.
// firstly ,we need to preprocess the training file which has been 
// segmented manually, we need to transfer its format to 
// type:word in which the type indicates the word's position 
// B : current single word is the head of the lexicon
// M : current single word is in the middle of the lexicon
// E : current single word is the tail of the lexicon which
//		means that a segmentation has been down
// S : current single word is single~
//
// wordseg.h: the definiton of the main model 

#ifndef WORDSEG_H_
#define WORDSEG_H_

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <map>
#include <exception>

#include "trie.h"
#include "hmm.h"

namespace hmmseg {
class WordSeg {

private:

	std::map<std::string, int> _word_to_index;	// word to index map
	std::map<char, int> _status_to_index;		// status to index map
	HMM*	_hmm;								// hmm model
	Trie*	_trie;								// trie tree
	// split the std::string
	void split(std::string &s, std::vector<std::string> &split_ret, const std::string &tag);
	
	// get word index
	void get_word_index(int &cur_word_cnt,
			std::vector<int> &str_indexes,
			const std::string &str);
	
	// int to std::string
	std::string int_to_str(int val);

	// init the dict
	bool init_dict(const char *dict_file);	


public:
	WordSeg() {}	
	~WordSeg() {}

	// brief: load data from training sample file  and process the data to the following 
	// note : the format in the file
	//		type:word whose meaning has been explained as above. 
	// param: 
	//		file_name	the name of the segmented corpus in which a line indicating the manual segmented outcome of a paragraph
	// return:
	//		None
	void load_data(const char *file_name); 

	// brief : save the word_to_index & status_to_index
	// param :
	//		file_name : the file gona to save the word dict
	// return :
	//		None
	void save_dict(const char *file_name);

	// brief :	init the model and dict
	// param :
	//		dict_file : the file that saved the word's index
	//		model_path : the file contains the hmm model
	// return :
	// 		true  : init enviroment successed
	//		false : init enviroment failed
	bool init_env(const char *dict_file, 
			const char *model_path,	
			const char *trie_dict_paht);
	// brief : segment the word
	// param :
	//		str : the string which gona to be segmented
	//		word_seg_result : segment results
	// return :
	//		None
	void segment(std::string &str, std::vector<std::string> &word_seg_result);

	// brief : segment method conbined the mm
	// param :
	//		str : the string which gona to be segmented
	//		word_seg_result : segment results
	// return :
	//		None
	void segment_mm(std::string &str, std::vector<std::string> &word_seg_results);
};

}
#endif
