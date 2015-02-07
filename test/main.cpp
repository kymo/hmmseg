// main program for test the file
// auth : aron @ whu
// date : 2015-02-20

#include "wordseg.h"
#include "trie.h"

using namespace hmmseg;

int test_train_module(int argv, char *argc[]) {
	if (argv < 5) {
		std::cerr << "Usage : ./hmm [hidden status cnt] [observed sequence cnt] [train file] [model_file]" << std::endl;
		return 0;
	}

	int hid_sta_cnt = atoi(argc[1]);
	int obs_seq_cnt = atoi(argc[2]);
	std::cerr << "Loading trainning samples." << std::endl;
	HMM *hmm = new HMM(hid_sta_cnt, obs_seq_cnt);
	if(! hmm->load_training_sample(argc[3])) {
		return 0;
	}
	std::cerr << "Training. " << std::endl;
	hmm->statistic_train();
	std::cerr << "Training End." << std::endl;
	std::cerr << "Saving model. " << std::endl;
	hmm->save_model(argc[4]);
	std::cerr << "Model is saved in " << argc[4] << std::endl;
}


int test_decode_module(int argv, char *argc[]) {
	if (argv != 3) {
		std::cerr << "Usage : ./hmm [model file path] [test file]" << std::endl;
		return 0;
	}
	HMM *hmm = new HMM();
	hmm->load_model(argc[1]);
	std::ifstream fis(argc[2]);
	int n;
	fis >> n;
	for (int i = 0; i < n; i ++) {
		int m;
		fis >> m;
		std::vector<int> sequence;
		for (int j = 0; j < m; j ++) {
			int cur;
			fis >> cur;
			std::cout << cur << "-" << std::endl;
			sequence.push_back(cur);
		}
		std::vector<int> status;
		hmm->viterbi(sequence, status);
		std::cout << "outcome:" << std::endl;
		for (int j = 0; j < m; j ++) {
			std::cout << status[j] << " ";
		}
		std::cout << std::endl;
	}

}

void test_word_seg(int argv, char *argc[]) {
	
	if (argv < 3) {
		std::cout << "Usage ./wordseg [training file] [dict file path]" << std::endl;
		return ;
	}
	WordSeg *word_seg = new WordSeg();
	word_seg->load_data(argc[1]);
	word_seg->save_dict(argc[2]);
}

void test_seg_word(int argv, char *argc[]) {
	if (argv < 5) {
		std::cout << "Usage ./wordseg [dict file] [model file] [trie_file] [test file]" << std::endl;
		return ;
	}
	WordSeg *word_seg = new WordSeg();
	word_seg->init_env(argc[1], argc[2], argc[3]);
	std::ifstream fis(argc[4]);
	std::string line;
	while (getline(fis, line)) {
		std::vector<std::string> results;
		word_seg->segment_mm(line, results);
		/*
		int len_ret = results.size();
		for (int i = 0; i < len_ret; i ++) {
			if (i < len_ret - 1) {
				std::cout << results[i] << "  ";
			} else {
				std::cout << results[i] << std::endl;
			}
		}
		*/
	}
}

void test_trie_tree(int argv, char* argc[]) {
	Trie *trie = new Trie();
	trie->load_dict(argc[1]);

	std::ifstream fis("test_file");
	std::string line;
	while (getline(fis, line)) {
		trie->simple_seg(line);
	}
}

int main(int argv, char *argc[]) {
	// test_train_module(argv, argc);
	// test_word_seg(argv, argc);     
    test_seg_word(argv, argc);
	// test_decode_module(argv, argc);
	// test_trie_tree(argv, argc);
	return 0;
}
