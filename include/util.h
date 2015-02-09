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
// util.h: the definiton of several common function needed by the module


#ifndef UTIL_H_
#define UTIL_H_


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// brief : split the chinese word to single one
// param : 
//		line : the chinese string
//		words : a vector store the outcome
// return : 
//		None
bool split_ch_words(const std::string &line, std::vector<std::string> &words);

// brief : transfer string to number
// param :
//		val : the int value saved the outcome
//		str : the string being transfered
// return :
//		false : transfer failed
//		true : transfer success
bool str_to_number(int &val, const std::string &str);

// brief : split the string to single one
// param :
//		str : the string being splited
//		split_ret : saved the outcome
//		tag : the substr by which the str is splited
// return :
//		None
bool split(std::string &str, std::vector<std::string> &split_ret, const std::string &tag);

// brief : int to string
// param :
//		val : the int that gona to be transfered
// return :
//		None
std::string int_to_string(int val);
#endif


