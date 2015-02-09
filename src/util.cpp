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
// util.cpp: the definiton of several common function needed by the module

#include "util.h"


bool split_ch_words(const std::string &line, std::vector<std::string> &words) {
	std::string cn_word = "";
	std::string not_cn_word = "";
	for (int i = 0; i < line.length(); i ++) {
		if (line[i] < 0) {
			cn_word += line[i];
			if (not_cn_word != "") {
				words.push_back(not_cn_word); 
				not_cn_word = "";
			}
		} else {
			not_cn_word += line[i];
			if (cn_word != "") {
				for (int j = 0; j < cn_word.size() / 3; j ++) {	
					words.push_back(cn_word.substr(j * 3, 3));
				}
				cn_word = "";
			}
		}
	}

	if (not_cn_word != "") {
		words.push_back(not_cn_word);
	} 
	
	if (cn_word != "") {
		for (int j = 0; j < cn_word.size() / 3; j ++) {	
			words.push_back(cn_word.substr(j * 3, 3));
		}
	}
	return true;
}


bool split(std::string &str, std::vector<std::string> &split_ret, const std::string &tag) {
	split_ret.clear();
	if (s.find(tag) == std::string::npos) {
		if (s.length() != 0) {
			split_ret.push_back(s);
		}
		return false;
	}
	int cur_pos = 0, find_pos = 0;
	s += tag;
	while ((find_pos = s.find(tag, cur_pos)) != std::string::npos) {
		std::string subs = s.substr(cur_pos, find_pos - cur_pos);
		if (subs.size() != 0) {
			split_ret.push_back(s.substr(cur_pos, find_pos - cur_pos));
		}
		cur_pos = find_pos + 1;
	}
	return true;
}

std::string int_to_str(int val) {
	std::string ret = "";
	if (val == 0) {
		return "0";
	}
	while (val > 0) {
		ret += val % 10 + '0';
		val /= 10;
	}
	reverse(ret.begin(), ret.end());
	return ret;
}

bool str_to_number(int &val, const std::string &str) {
	val = 0;
	int ten = 1;
	for (int i = (str.length()) - 1; i >= 0; i --) {
		int num = str[i] - '0';
		if (! (num >= 0 && num <= 9)) {
			return false;
		}
		val += num * ten;
		ten *= 10;
	}
	return true;
}
