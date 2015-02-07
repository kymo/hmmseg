#ifndef _WORDSEG_H_
#define _WORDSEG_H_

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

// auth : aron
// date : 2015-02-20


#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <map>

#include "hmm.h"

using namespace std;

class WordSeg {

private:

	map<string, int> word_to_index;
	map<char, int> status_to_index;
	HMM *hmm;

	//hash_map<string, int> word_to_index;
	//hash_map<string, int> status_to_index;
	// split the string
	void split(string &s, vector<string> &split_ret, const string &tag);
	
	// get word index
	void get_word_index(int &cur_word_cnt,
			vector<int> &str_indexes,
			const string &str);
	
	// int to string
	string int_to_str(int val);

	// init the dict
	bool init_dict(const char *dict_file);
	
	
	// clean the string
	void clean_str(string &str);										

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
	void segment(string str, vector<string> &word_seg_result);

};

#endif
