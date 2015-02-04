// wordseg source file
// 

// auth : aron
// date : 2015-02-02

#include "wordseg.h"


void WordSeg::split(string &s, vector<string> &split_ret, const string &tag) {
	split_ret.clear();
	if (s.find(tag) == string::npos) {
		return ;
	}
	int cur_pos = 0, find_pos = 0;
	s += tag;
	while ((find_pos = s.find(tag, cur_pos)) != string::npos) {
		string subs = s.substr(cur_pos, find_pos - cur_pos);
		if (subs.size() != 0) {
			split_ret.push_back(s.substr(cur_pos, find_pos - cur_pos));
		}
		cur_pos = find_pos + tag.length();
	}
}

void WordSeg::get_word_index(int &cur_word_cnt, 
		vector<int> &str_indexes, 
		const string &str) {
	int word_len = str.size() / 3;
	for (int i = 0; i < word_len; i ++) {
		string sub = str.substr(i * 3, 3);
		cout << sub << "~" << endl;
		if (word_to_index.find(sub) == word_to_index.end()) {
			cur_word_cnt += 1;
			word_to_index[sub] = cur_word_cnt;
		}
		str_indexes.push_back(word_to_index[sub]);
	}
}

string WordSeg::int_to_str(int val) {
	string ret = "";
	if (val == 0) return "0";
	while (val > 0) {
		ret += val % 10 + '0';
		val /= 10;
	}
	return string(ret.rbegin(), ret.rend());
}


void WordSeg::load_data(const char *file_name) {
	string status = "BMES";
	for (int i = 1; i <= 4; i ++) {
		status_to_index[status[i - 1]] = i;
	}
	ifstream fis(file_name);
	string line, tag;
	vector<string> split_ret;
	int cur_diff_word_cnt = 0;
	while (getline(fis, line)) {
		tag = "  ";
		split(line, split_ret, tag);
		string all_ans = "";
		for (int i = 0; i < split_ret.size(); i ++) {
			string ret_ans = "";
			vector<int> str_indexes;
			get_word_index(cur_diff_word_cnt, str_indexes, split_ret[i]);
			for (int j = 0; j < str_indexes.size(); j ++) {
				if (j == 0) {
					ret_ans += "1:" + int_to_str(str_indexes[j]);
				} else if (j < str_indexes.size() - 1) {
					ret_ans += "2:" + int_to_str(str_indexes[j]);
				} else {
					ret_ans += "3:" + int_to_str(str_indexes[j]);
				}
				if (j < str_indexes.size() - 1) {
					ret_ans += ",";
				}
			}
			
			if (str_indexes.size() == 1) {
				ret_ans[0] = '4';
			}
			all_ans += ret_ans;
			if (i < split_ret.size() - 1) {
				
				all_ans += ",";
			}
		}
		//cout << all_ans << endl;
	}
}

void WordSeg::save_dict(const char *file) {
	ofstream fos(file);
	// save number
	fos << "4:" << word_to_index.size() << endl;
	// save status
	for (map<char, int>::iterator it = status_to_index.begin();
			it != status_to_index.end(); it ++) {
		fos << it->first << ":" << it->second << endl;
	}

	// save word index
	for (map<string, int>::iterator it = word_to_index.begin();
			it != word_to_index.end(); it ++) {
		fos << it->first << ":" << it->second << endl;
	}
	fos.close();
}


// for segmentation 
bool WordSeg::init_dict(const char *dict_file) {
	int t, n;
	string str, tag;
	vector<string> split_ret;
	ifstream fis(dict_file);
	tag = ":";
	
	fis >> str ;
	//cout << str <<endl;
	split(str, split_ret, tag);
	n = atoi(split_ret[0].c_str());
	t = atoi(split_ret[1].c_str());
	//cout << t << " " << n << endl;
	
	for (int i = 0; i < n; i ++) {
		fis >> str;
		split(str, split_ret, tag);
		status_to_index[split_ret[0][0]] = atoi((split_ret[1]).c_str());
	}
	
	for (int j = 0; j < t;j ++) {
		fis >> str;
		split(str, split_ret, tag);
		word_to_index[split_ret[0]] = atoi(split_ret[1].c_str());

	}
	/*
	try {
		for (int i = 0; i < 4; i ++) {
			for (int j = 0; j < n; j ++) {
				fis >> first >> t;
				word_to_index[first.substr(0, first.size() - 1)] = t;
			}
		}
	} catch (exception &e) {
		cerr << "Error when loading dict!" << endl;
		return false;
	}
	*/
	
	return true;
}

bool WordSeg::init_env(const char *dict_file,
		const char *model_path) {
	hmm = new HMM();
	if (! hmm->load_model(model_path)) {
		return false;
	}
	if (! init_dict(dict_file)) {
		return false;
	}
	return true;
}

void WordSeg::clean_str(string &str) {

}

void WordSeg::segment(string str, vector<string> &word_seg_result) {
	// TODO clean string
	clean_str(str);
	int word_len = str.length() / 3;
	vector<int> sequence;
	for (int i = 0; i < word_len; i ++) {
		string substr = str.substr(3 * i, 3);
		sequence.push_back(word_to_index[substr]);
		cout << word_to_index[substr] << endl;
	}
	vector<int> hidden_status;
	hmm->viterbi(sequence, hidden_status);
	for (int j = 0; j < word_len; j ++) {
		string substr = str.substr(3 * j, 3);
		cout << substr;
		if (j < word_len - 1 && (hidden_status[j] == 4 || hidden_status[j] == 3)) {
			cout << "\\ ";
		}
	}
}

