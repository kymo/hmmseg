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
// firstly we use the training file to train a model then we save the
// model into file, and then supply a interface to find the hidden 
// status(the segment outcome) given the observed words we use 
// B, M, E, S to state the status of a chinese word in the training 
// process, we need to calculate the initial matrix pi, the confused
// matrix B in which  the element bij indicates the probability of
// tranfering from hidden status c_i to observed status o_i and the 
// tranfering matrix A in which the element aij indicates the probability
// of transfering from hidden status c_i to hidden status c_j, thus
// bij = p(o_j | c_i) aij = p(c_j | c_i)
// each line of the input feature file must has the following format
// observed_status:hidden_status\tobserverd_status:hidden_status this
// program contains two parts, first one is the training process, given
// the training sample, the program will build a model which contains
// three matrixes _A, _B and _pi which are saved in model file second one
// is the decoding process, given the observed sequence, the program will
// reture the most prossible hidden statuses that generate the sequence 
// by viterbi algorithm
//
//
// hmm.h: the definiton of the main model 

#ifndef HMM_H_
#define HMM_H_


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <exception>
#include <cmath>
#include <algorithm>

namespace hmmseg {
namespace hmm {
class HMM {
private:
	
	std::vector<std::vector<float> > _A;			// transfer matrix
	std::vector<std::vector<float> > _B;			// confused matrix
	std::vector<float> _pi;					// pi
	
	int _T;								// observed sequence cnt
	int _N;								// status cnt number
	std::vector<int> _sta_cnt;				// status cnt array
	std::vector<std::vector<int> > _hid_obv_cnt;	// cnt for co-exist of observed 
										// sequence and hidden status
	std::vector<std::vector<int> > _sta_co_cnt;	// cnt for co-exist of hidden status

	
	// brief : calculate the confused matrix _B
	// note : _B[i][j] indicating the probability of generating observed sequence j
	//		from hidden status i _B[i][j] = p(o_j | c_i) = count(o_j,c_i) / count(c_i)
	// 		count(o_j, c_i) means the times when o_j and c_i shows together in the 
	//		feature file, count(c_i) means the show times of c_i in the feature file
	// param : 
	//		None
	// return :
	// 		None
	void calc_confused_matrix();
	
	// brief : calculate the transfering matrix
	// note : _A[i][j] indicating the probability of transfering from hidden status
	//		i to hidden status j _A[i][j] = p(c_j | c_i) = count(c_j, c_i) / count(c_i)
	// param : 
	//		None
	// return :
	// 		None
	void calc_tranfer_matrix();

public:

	HMM() {}
	~HMM(){}
	HMM(int N, int T) : _T(T), _N(N) {
		init_space();
	}

	// init the parameters
	void init_space();

	
	// load training file
	bool load_training_sample(const char *file_name);

	// brief : save the model in file_name
	// param :
	//		file_name : model file name
	// return :
	//		None
	void save_model(const char *file_name);

	// brief : hmm train model
	// note : just calculate the two matrixes, and a model is formed by statistics.
	// param : 
	//		None
	// return :
	//		None
	void statistic_train();

	// em train
	void em_train();
	
	// brief : load hmm model actually load the matrixs
	// param :
	//		file_name : model file 
	// return :
	//		None
	bool load_model(const char *file_name);

	// brief : common viterbi algorithm to find the most prossible hidden statuses which
	//		generate the given observed data
	// param :
	// 		observed_seq : the observed sequences
	//		hidden_status : the status that generate the observed sequence with the max 
	//			probability
	// return :
	//		None
	bool viterbi(const std::vector<int> &observed_seq, std::vector<int> & hidden_status);	
	
	// brief : viterbi algorithm for segmentation interface to wordseg  module
	// note : in the path that generate the hidden status, we obey the rules that generate
	// 		the observed sequence, that means when the hidden status is B, then the next
	//		status cannot be S, but M or S only.
	// param : 
	//		observed_seq : observed sequence
	//		hidden_status : the status the generate the observed sequence with the max 
	//			probability.
	// return :
	//		None
	bool viterbi_seg(const std::vector<int> &observed_seq, std::vector<int> & hidden_status);	
	
	// brief : given the hidden status and the observed data, calculate the probability.
	// param : 
	//		observed_seq : observed sequence
	//		hidden_status : hidden status 
	// return :
	//		None
	float calculate_pro(const std::vector<int> &observed_seq,
			const std::vector<int> &hidden_status) ;
};
}
}
#endif
