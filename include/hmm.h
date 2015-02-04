#ifndef _HMM_H_
#define _HHH_H_
// hidden markov model for segmentation
// firstly we use the training file to train a model
// then we save the model into file, and then supply 
// copyright@aron kymowind@gmail.com
// a interface to find the hidden status(the segment
// outcome) given the observed words
// we use B, M, E, S to state the status of a chinese word
// in the training process, we need to calculate the
// initial matrix pi, the confused matrix B in which 
// the element bij indicates the probability of tranfering
// from hidden status c_i to observed status o_i and 
// the tranfering matrix A in which the element aij
// indicates the probability of transfering from hidden
// status c_i to hidden status c_j, thus
// bij = p(o_j | c_i) aij = p(c_j | c_i)
// each line of the input feature file must has the following format
// observed_status:hidden_status\tobserverd_status:hidden_status
// this program contains two parts
// first one is the training process, given the training sample,
// the program will build a model which contains three matrix _A,
// _B and _pi which are saved in model file
// second one is the decoding process, given the observed sequence,
// the program will reture the most prossible hidden statuses that
// generate the sequence by viterbi algorithm

// this program obeys the GPL protocol, everyone can use or change the
// source code , and if you find any bugs in the program, please 
// contact me : kymowind#gmail.com

// date : 2015-02-02

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
using namespace std;

class HMM {
private:
	
	vector<vector<float> > _A;			// transfer matrix
	vector<vector<float> > _B;			// confused matrix
	vector<float> _pi;					// pi
	
	int _T;								// observed sequence cnt
	int _N;								// status cnt number
	vector<int> _sta_cnt;				// status cnt array
	vector<vector<int> > _hid_obv_cnt;	// cnt for co-exist of observed 
										// sequence and hidden status
	vector<vector<int> > _sta_co_cnt;	// cnt for co-exist of hidden status

	// string to number
	bool str_to_number(int &val, const string &str);

	// split data
	void split(string &s, vector<string> &split_ret, const string &tag);
	
	// calculate the confused matrix _B
	// _B[i][j] indicating the probability of generating observed
	// sequence j from hidden status i
	// _B[i][j] = p(o_j | c_i) = count(o_j,c_i) / count(c_i)
	// count(o_j, c_i) means the times when o_j and c_i shows together
	// in the feature file, count(c_i) means the show times of c_i
	// in the feature file
	void calc_confused_matrix();
	
	// calculate the transfering matrix
	// _A[i][j] indicating the probability of transfering from 
	// hidden status i to hidden status j
	// _A[i][j] = p(c_j | c_i) = count(c_j, c_i) / count(c_i)

	void calc_tranfer_matrix();

public:

	HMM() {}
	HMM(int N, int T) : _T(T), _N(N) {
		init_space();
	}

	void init_space() {
		for (int i = 0; i <= _N; i ++) {
			_A.push_back(vector<float>(_N + 1, 0.0));
			_sta_co_cnt.push_back(vector<int>(_N + 1, 0));
			_hid_obv_cnt.push_back(vector<int>(_T + 1, 0));
			_B.push_back(vector<float>(_T + 1, 0.0));
			_pi.push_back(0.0);
			_sta_cnt.push_back(0);
		}
	}

	virtual ~HMM(){}
	
	// load training file
	bool load_training_sample(const char *file_name);

	// save model
	void save_model(const char *file_name);

	// hmm train model
	void statistic_train();

	// em train
	void em_train();
	
	// load hmm model actually load the matrixs
	bool load_model(const char *file_name);

	// viterbi algorithm to find the most prossible hidden statuses which generate
	// the given observed data
	bool viterbi(const vector<int> &observed_seq, vector<int> & hidden_status);	
};

#endif
