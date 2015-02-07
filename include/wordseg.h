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

#ifndef _WORDSEG_H_
#define _WORDSEG_H_

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <map>
#include <exception>
#include "hmm.h"

namespace hmmseg {
class WordSeg {

private:

	std::map<std::string, int> word_to_index;
	std::map<char, int> status_to_index;
	HMM *hmm;

	//hash_map<std::string, int> word_to_index;
	//hash_map<std::string, int> status_to_index;
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
	
	
	// clean the std::string
	void clean_str(std::string &str);										

public:
	WordSeg() {}	
	virtual ~WordSeg() {}

	// init the model and dict
	bool init_env(const char *dict_file, const char *model_path);
	// load data from training sample file  and process the data to the following 
	// format : type:word whose meaning has been explained as above. 
	void load_data(const char *file_name); 

	// save the word_to_index & status_to_index
	void save_dict(const char *file);

	// segment the word
	void segment(std::string str, std::vector<std::string> &word_seg_result);

};

}
#endif
