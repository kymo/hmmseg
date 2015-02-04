


#ifndef _HMM_H_
#define _HMM_H_

// hidden markov model implementation file
// auth : aron @ whu
// date : 2015-02-02

#include "hmm.h"


void HMM::split(string &s, vector<string> &split_ret, const string &tag) {
	if (s.find(tag) == string::npos) {
		return ;
	}
	split_ret.clear();
	int cur_pos = 0, find_pos = 0;
	s += tag;
	while ((find_pos = s.find(tag, cur_pos)) != string::npos) {
		split_ret.push_back(s.substr(cur_pos, find_pos - cur_pos));
		cur_pos = find_pos + 1;
	}
}

bool HMM::str_to_number(int &val, const string &str) {
	int val = 0, ten = 1;
	for (int i = str.length() - 1; i >=0 ; i --) {
		int num = str[i] - '0';
		if (! (num >= 0 && num <= 9)) {
			return false;
		}
		val += (str[i] - '0') * ten;
		ten *= 10;
	}
	return true;
}

void HMM::load_training_sample(const char* file_name) {
	ifstream fis(file_name);
	string line, tag;
	int line_num = 0;
	vector<string> split_ret;
	while (getline(fis, line)) {
		tag = "\t";
		line_num += 1;
		split(line, split_ret, tag);
		int last_hid_status = -1;
		for (vector<string::iterator it = split_ret.begin();
				it !=  split_ret.end(); it ++) {
			
			vector<string> cur_split_ret;
			tag = ":";
			split(*it, cur_split_ret, tag);
			
			if (cur_split_ret.size() != 2) {
				cerr << "Error when loading training sample : data format is not legal in line" <<
					line_num << endl;
			}
			
			int obs_seq_id, hid_sta_id; 
			if (! str_to_number(obs_seq_id, cur_split_ret[1]) ||
					! str_to_number(hid_sta_id, cur_split_ret[2])) {
				cerr << "Error when loading training sample : data type is not legal in line " <<
					line_num << endl;
			}
			
			if (obs_seq_id > _T || hid_sta_id > _N) {
				cerr << "Error when loading training sample : observed sequence id or hidden status id is not legal in line " 
					<< line_num << endl;
			}
		    _sta_cnt[hid_sta_id] += 1；	
			_obs_sta_cnt[hid_sta_id][obs_seq_id] += 1;
			if (last_hid_status == -1) {
				last_hid_status = hid_sta_id;
			} else {
				_sta_co_cnt[last_hid_status][hid_sta_id] += 1;
				last_hid_status = hid_sta_id;
			}
		}
	}
}

void HMM::calc_confused_matrix() {
	for (int j = 1; j <= _N; j ++) {
		for (int k = 1; k <= _T; k ++) {
			_B[j][k] = _obs_sta_cnt[j][k] / _sta_cnt[j];
		}
	}
}

void HMM::calc_tranfer_matrix() {
	for (int j = 1; j <= _N; j ++) {
		for (int k = 1; k <= _N; k ++) {
			_A[j][k] = _sta_co_cnt[j][k] / _sta_cnt[j];
		}
	}
}

void HMM::save_model() {

}

void HMM::load_model() {

}


void HMM::statistic_train() {
	calc_confused_matrix();
	calc_tranfer_matrix();
	
}


void 

#endif
