
// trie source file 

#include "trie.h"

bool Trie::split_ch_words(const string &line, vector<string> &words) {
	string cn_word = "";
	string not_cn_word = "";
	for (int i = 0; i < line.length(); i ++) {
		//if (line[i] >= 0x4e00 && line[i] <= 0x96a5) {
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

void Trie::set_string_node(bool is_string_node) {
	_is_string_node = is_string_node;
}

void Trie::build_tree(Trie *&root, const vector<string> &words, int cur) {

	if (cur >= words.size()) return;
	string cur_word = words[cur];
	
	if (root->_child_trees.find(cur_word) == root->_child_trees.end()) {
		root->_child_trees.insert(pair<string, Trie*>(cur_word, new Trie(cur_word)));
	}
	
	if (cur + 1 == words.size()) {
		root->_child_trees[cur_word]->set_string_node(true);
		return ;
	}
	build_tree(root->_child_trees[cur_word], words, cur + 1);
}


void Trie::display(Trie *&trie) {
	queue<Trie*> q;
	q.push(trie);
	while (! q.empty()) {
		Trie *tp = q.front();
		q.pop();
		if (tp->_word != "") {
			cout << "father: " << tp->_word << endl;
		} else {
			cout << "father: root!" << endl;
		}
		for (map<string, Trie*>::iterator it = tp->_child_trees.begin(); it != tp->_child_trees.end(); it ++) {
			cout << it->first << " ";
			q.push(it->second);
		}
		cout << endl;
	}

}

bool Trie::load_dict(const char* file_name) {
	// TODO judge the encoding of the file
	ifstream fis(file_name);
	string line;
	int line_num;
	line_num = 0;
	_root = new Trie();
	map<string, int> dic;
	while (getline(fis, line)) {
		vector<string> words;
		if (! split_ch_words(line, words)) {
			cerr << "Error when split the words in line : " << line_num << endl;
			return false;
		}
		build_tree(_root, words, 0);
	}
	return true;
}

void Trie::simple_seg(string &str) { 
	vector<string> ret;
	vector<string> words;
	string trunk;
	int start , ends;
	split_ch_words(str, words);
	start = 0, ends = words.size();
	ends --;
	while (start <= ends) {
		bool tag = false;
		for (int k = start; k <= ends; k ++) {
			if (search(_root, words, start, ends - k + start)) {
				trunk = "";	
				for (int j = start;j <= ends - k + start; j ++) {
					trunk += words[j];
				}
				ret.push_back(trunk);
				start = ends - k + start + 1;
				ends = words.size() - 1;
				tag = true;
				break;
			} 
		}
		if (! tag) {
			ret.push_back(words[start]);	
			start += 1;
		} 
	}	
}


bool Trie::search(Trie *&tree, 
		const vector<string> &words, 
		int i, int j) {
	if (i > j) return false;
	if (i == j) {
		if (tree->_child_trees.find(words[i]) != tree->_child_trees.end()) {
			return tree->_child_trees[words[i]]->_is_string_node;
		}
	}

	if (tree->_child_trees.find(words[i]) == tree->_child_trees.end()) {
		return false;
	}
	return search(tree->_child_trees[words[i]], words, i + 1, j);
}
