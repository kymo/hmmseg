// main program for test the file

// auth : aron @ whu
// date : 2015-02-20

#include "hmm.h"

int test_train_module(int argv, char *argc[]) {
	if (argv < 5) {
		cerr << "Usage : ./hmm [hidden status cnt] [observed sequence cnt] [train file] [model_file]" << endl;
		return 0;
	}

	int hid_sta_cnt = atoi(argc[1]);
	int obs_seq_cnt = atoi(argc[2]);
	cerr << "Loading trainning samples." << endl;
	HMM *hmm = new HMM(hid_sta_cnt, obs_seq_cnt);
	if(! hmm->load_training_sample(argc[3])) {
		return 0;
	}
	cerr << "Training. " << endl;
	hmm->statistic_train();
	cerr << "Training End." << endl;
	cerr << "Saving model. " << endl;
	hmm->save_model(argc[4]);
	cerr << "Model is saved in " << argc[4] << endl;
}


int test_decode_module(int argv, char *argc[]) {
	if (argv != 3) {
		cerr << "Usage : ./hmm [model file path] [test file]" << endl;
		return 0;
	}
	HMM *hmm = new HMM();
	hmm->load_model(argc[1]);
	ifstream fis(argc[2]);
	int n;
	
	fis >> n;
	
	for (int i = 0; i < n; i ++) {
		int m;
		fis >> m;
		vector<int> sequence;
		for (int j = 0; j < m; j ++) {
			int cur;
			fis >> cur;
			sequence.push_back(cur);
		}
		vector<int> status;
		hmm->viterbi(sequence, status);
		cout << "outcome:" << endl;
		for (int j = 0; j < m; j ++) {
			cout << status[j] << " ";
		}
		cout << endl;
	}
}

int main(int argv, char *argc[]) {
	test_decode_module(argv, argc);
	return 0;
}
