
#include "wordseg.h"
#include "trie.h"
#include "util.h"

using namespace hmmseg::wordseg;
using namespace hmmseg::util;
using namespace hmmseg::trie;
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
		//word_seg->segment(line, results);
		int len_ret = results.size();
		for (int i = 0; i < len_ret; i ++) {
			if (i < len_ret - 1) {
				std::cout << results[i] << " ";
			} else {
				std::cout << results[i] << std::endl;
			}
		}
	}
}

int main(int argv, char *argc[]) {
	test_seg_word(argv, argc);
	return 0;
}
