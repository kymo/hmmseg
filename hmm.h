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
	
	vector<vector<float> > _A;
	vector<vector<float> > _B;
	vector<float> _pi;

public:

	HMM() {}

	virtual ~HMM(){}

	// calca the confused matrix
	void calc_confused_matrix();
	
	// calc the transfering matrix
	void calc_tranfer_matrix();
	
	// save model
	void save_model();
	
	// load hmm model actually load the matrixs
	void load_model();

	// segment the outcome
	void segment(const string *words, vector<string> &seg_result);

};
