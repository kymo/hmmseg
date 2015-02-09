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

namespace hmmseg {
namespace util {

bool split_ch_words(const std::string &line, std::vector<std::string> &words) {
	std::string cn_word = "";
	std::string not_cn_word = "";
	for (int i = 0; i < line.length();) {
		int tag = 0;
		if ((int)(line[i] & 0xf0) == 0xe0) {
			if (not_cn_word != "") {
				words.push_back(not_cn_word);
			}
			not_cn_word = "";
			if ((int)(line[i + 1] & 0xc0) == 0x80 && (int)(line[i + 2] & 0xc0) == 0x80) {
				words.push_back(line.substr(i, 3));
				i += 3;
				continue;
			}
		} else if (line[i] == ' ') {
			if (not_cn_word != "") {
				words.push_back(not_cn_word);
			}
			not_cn_word = "";
			i += 1;
			continue;
		}
		not_cn_word += line[i];
		i += 1;
	}
	if (not_cn_word != "") {
		words.push_back(not_cn_word);
	}
	return true;
}


bool split(std::string &str, std::vector<std::string> &split_ret, const std::string &tag) {
	split_ret.clear();
	if (str.find(tag) == std::string::npos) {
		if (str.length() != 0) {
			split_ret.push_back(str);
		}
		return false;
	}
	int cur_pos = 0, find_pos = 0;
	str += tag;
	while ((find_pos = str.find(tag, cur_pos)) != std::string::npos) {
		std::string subs = str.substr(cur_pos, find_pos - cur_pos);
		if (subs.size() != 0) {
			split_ret.push_back(str.substr(cur_pos, find_pos - cur_pos));
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

bool is_sign(std::string str) {
	if (str.size() != 3) return true;

	if (((int)(str[0] & 0xf0) == 0xe0 && (int)(str[0] & 0xc0 == 0x20) && (int)(str[2] & 0xc0 == 0x20))) {
		return false;
	}

	if (str == "。" || str == "，" || str == "：" || str == "，" 
			|| str == "”" || str == "“" || str == "‘" 
			|| str == "’" || str == "、" || str == "？" 
			|| str == "』" || str == "『" || str == "【" 
			|| str == "】" || str == "！" || str == "；")
		return true;
	return false;
}

}
}
