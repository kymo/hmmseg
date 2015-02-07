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
// wordseg.cpp: the definiton of the wordseg 
// 


#include "wordseg.h"
namespace hmmseg {

void WordSeg::split(std::string &s, std::vector<std::string> &split_ret, const std::string &tag) {
	split_ret.clear();
	if (s.find(tag) == std::string::npos) {
		return ;
	}
	int cur_pos = 0, find_pos = 0;
	s += tag;
	while ((find_pos = s.find(tag, cur_pos)) != std::string::npos) {
		std::string subs = s.substr(cur_pos, find_pos - cur_pos);
		if (subs.size() != 0) {
			split_ret.push_back(s.substr(cur_pos, find_pos - cur_pos));
		}
		cur_pos = find_pos + tag.length();
	}
}

void WordSeg::get_word_index(int &cur_word_cnt, 
		std::vector<int> &str_indexes, 
		const std::string &str) {
	int word_len = str.size() / 3;
	for (int i = 0; i < word_len; i ++) {
		std::string sub = str.substr(i * 3, 3);
		if (_word_to_index.find(sub) == _word_to_index.end()) {
			cur_word_cnt += 1;
			_word_to_index[sub] = cur_word_cnt;
		}
		str_indexes.push_back(_word_to_index[sub]);
	}
}

std::string WordSeg::int_to_str(int val) {
	std::string ret = "";
	if (val == 0) return "0";
	while (val > 0) {
		ret += val % 10 + '0';
		val /= 10;
	}
	return std::string(ret.rbegin(), ret.rend());
}


void WordSeg::load_data(const char *file_name) {
	std::string status = "BMES";
	for (int i = 1; i <= 4; i ++) {
		_status_to_index[status[i - 1]] = i;
	}
	std::ifstream fis(file_name);
	std::string line, tag;
	std::vector<std::string> split_ret;
	int cur_diff_word_cnt = 0;
	while (getline(fis, line)) {
		tag = "  ";
		split(line, split_ret, tag);
		std::string all_ans = "";
		std::vector<int> test;
		for (int i = 0; i < split_ret.size(); i ++) {
			std::string ret_ans = "";
			std::vector<int> str_indexes;
			get_word_index(cur_diff_word_cnt, str_indexes, split_ret[i]);
			for (int j = 0; j < str_indexes.size(); j ++) {
				if (j == 0) {
					test.push_back(1);
					ret_ans += "1:" + int_to_str(str_indexes[j]);
				} else if (j < str_indexes.size() - 1) {
					test.push_back(2);
					ret_ans += "2:" + int_to_str(str_indexes[j]);
				} else {
					test.push_back(3);
					ret_ans += "3:" + int_to_str(str_indexes[j]);
				}
				if (j < str_indexes.size() - 1) {
					ret_ans += ",";
				}
			}

			if (str_indexes.size() == 1) {
				test.push_back(4);
				ret_ans[0] = '4';
			}

			all_ans += ret_ans;
			int len = split_ret.size() - 1;
			if (i < len) {
				all_ans += ",";
			}
		}
		std::cout << all_ans << std::endl;
	}

}

void WordSeg::save_dict(const char *file) {
	std::ofstream fos(file);
	// save number
	try {
		fos << "4:" << _word_to_index.size() << std::endl;
	// save status
	for (std::map<char, int>::iterator it = _status_to_index.begin();
			it != _status_to_index.end(); it ++) {
		fos << it->first << ":" << it->second << std::endl;
	}
	// save word index
	for (std::map<std::string, int>::iterator it = _word_to_index.begin();
			it != _word_to_index.end(); it ++) {
		fos << it->first << ":" << it->second << std::endl;
	}
	} catch(std::exception &e) {
		std::cerr << "Error when saveing the word dict !" << std::endl;
		return ;
	}
	fos.close();
}

bool WordSeg::init_dict(const char *dict_file) {
	int t, n;
	std::string str, tag;
	std::vector<std::string> split_ret;
	try {
		std::ifstream fis(dict_file);
		tag = ":";
		fis >> str ;
		split(str, split_ret, tag);
		n = atoi(split_ret[0].c_str());
		t = atoi(split_ret[1].c_str());
		for (int i = 0; i < n; i ++) {
			fis >> str;
			split(str, split_ret, tag);
			_status_to_index[split_ret[0][0]] = atoi((split_ret[1]).c_str());
		}
		
		for (int j = 0; j < t;j ++) {
			fis >> str;
			split(str, split_ret, tag);
			_word_to_index[split_ret[0]] = atoi(split_ret[1].c_str());

		}
	} catch (std::exception &e) {
		std::cerr << "Error when loading dict!" << std::endl;
		return false;
	}
	return true;
}

bool WordSeg::init_env(const char *dict_file,
		const char *model_path,
		const char *trie_dict_path) {
	
	hmm = new HMM();
	trie = new Trie();
	if (! hmm->load_model(model_path)) {
		return false;
	}
	if (! init_dict(dict_file)) {
		return false;
	}
	if (! trie->load_dict(trie_dict_path)) {
		return false
	}
	return true;
}

void WordSeg::segment(std::string &str, std::vector<std::string> &word_seg_result) {
	int word_len = str.length() / 3;
	std::vector<int> sequence;

	for (int i = 0; i < word_len; i ++) {
		std::string substr = str.substr(3 * i, 3);
		sequence.push_back(_word_to_index[substr]);
	}

	std::vector<int> hidden_status;
	hmm->viterbi_seg(sequence, hidden_status);
	reverse(hidden_status.begin(), hidden_status.end());

	std::string cur = "";
	for (int j = 0; j < word_len; j ++) {
		std::string substr = str.substr(3 * j, 3);	
		cur += substr;
		if ((hidden_status[j] == 4 || hidden_status[j] == 3)) {
			word_seg_result.push_back(cur);
			cur = "";
		}
	}
}

void WordSeg::segment_mm(std::string &str, std::vector<std::string> &word_seg_results) {

	vector<vector<string> > results;
	if (! trie->find_all_results(str, results)) {
	
	}
	for (int i = 0; i < results; i ++) {
		for (int j = 0; j < results[i].size(); j ++) {
			if (j + 1 < results[i].size()) {
				cout << results[i][j] << " ";
			} else {
				cout << results[i][j] << endl;
			}
		}
	}
}

}
