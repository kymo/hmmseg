#ifndef _HMM_H_
#define _HHH_H_
// hidden markov model for segmentation
// firstly we use the training file to train a model
// then we save the model into file, and then supply 
// a interface to find the hidden status(the segment
// outcome) given the observed words
// we use B, M, E, S to state the status of a chinese
// word
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
// auth : aron @ whu
// date : 2015-02-02


#include <iostream>
#include <hashmap>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

class HMM {
private:
	
	vector<vector<float> > _A;			// transfer matrix
	vector<vector<float> > _B;			// confused matrix
	vector<float> _pi;					// pi
	int _T;								// observed sequence cnt
	int _N;								// status cnt number
	vector<int> _sta_cnt;				// status cnt array
	vector<vector<int> > _obs_sta_cnt;	// cnt for co-exist of observed 
										// sequence and hidden status
	vector<vector<int> > _sta_co_cnt;	// cnt for co-exist of hidden status

public:

	HMM(int T, int N) : _T(T), _N(N) {}
	virtual ~HMM(){}

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
	
	// load training file
	void load_training_sample(const char *file_name);

	// calculate the transfering matrix
	// _A[i][j] indicating the probability of transfering from 
	// hidden status i to hidden status j
	// _A[i][j] = p(c_j | c_i) = count(c_j, c_i) / count(c_i)

	void calc_tranfer_matrix();

	// save model
	void save_model();
	
	// load hmm model actually load the matrixs
	void load_model();

	// hmm train model
	void statistic_train();

	// em train
	void em_train();
};

#endif
