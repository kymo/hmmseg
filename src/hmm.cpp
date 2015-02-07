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
// hmm.cpp: main file for hmmseg
// Based on the following blogs:
//		http://www.cnblogs.com/zhangchaoyang/articles/2571110.html
//		http://www.52nlp.cn/itenyh
//



#include "hmm.h"

namespace hmmseg {

void HMM::init_space() {
	for (int i = 0; i <= _N; i ++) {
		_A.push_back(std::vector<float>(_N + 1, 0.0));
		_sta_co_cnt.push_back(std::vector<int>(_N + 1, 0));
		_hid_obv_cnt.push_back(std::vector<int>(_T + 1, 0));
		_B.push_back(std::vector<float>(_T + 1, 0.0));
		_pi.push_back(0.0);
		_sta_cnt.push_back(0);
	}
}

void HMM::split(std::string &s, std::vector<std::string> &split_ret, const std::string &tag) {
	split_ret.clear();
	if (s.find(tag) == std::string::npos) {
		if (s.length() != 0) {
			split_ret.push_back(s);
		}
		return ;
	}
	int cur_pos = 0, find_pos = 0;
	s += tag;
	while ((find_pos = s.find(tag, cur_pos)) != std::string::npos) {
		std::string subs = s.substr(cur_pos, find_pos - cur_pos);
		if (subs.size() != 0) {
			split_ret.push_back(s.substr(cur_pos, find_pos - cur_pos));
		}
		cur_pos = find_pos + 1;
	}
}

bool HMM::str_to_number(int &val, const std::string &str) {
	val = 0;
	int ten = 1;
	for (int i = (str.length()) - 1; i >= 0; i --) {
		int num = str[i] - '0';
		if (! (num >= 0 && num <= 9)) {
			return false;
		}
		val += num * ten;
		ten *= 10;
	}
	return true;
}

bool HMM::load_training_sample(const char* file_name) {
	std::ifstream fis(file_name);
	std::string line, tag;
	int line_num = 0;
	std::vector<std::string> split_ret;
	int obs_seq_id, hid_sta_id; 
	while (getline(fis, line)) {
		int last_hid_status = -1;
		tag = ",";
		line_num += 1;
		split(line, split_ret, tag);
		if (split_ret.size() == 0) continue;

		for (std::vector<std::string>::iterator it = split_ret.begin();
				it !=  split_ret.end(); it ++) {
			std::vector<std::string> cur_split_ret;
			tag = ":";
			split(*it, cur_split_ret, tag);
			if (cur_split_ret.size() == 0) continue;
			if (cur_split_ret.size() != 2) {
				std::cerr << "Error when loading training sample : data format is not legal in line" <<
					line_num << std::endl;
				return false;
			}
			if (! str_to_number(obs_seq_id, cur_split_ret[1]) ||
					! str_to_number(hid_sta_id, cur_split_ret[0])) {
				std::cerr << "Error when loading training sample : data type is not legal in line " <<
					line_num << std::endl;
				return false;
			}


			if (obs_seq_id > _T || hid_sta_id > _N) {
				std::cerr << "Error when loading training sample : observed sequence id or hidden status id is not legal in line " 
					<< line_num << std::endl;
				return false;

			}
		    _sta_cnt[hid_sta_id] += 1;
			_hid_obv_cnt[hid_sta_id][obs_seq_id] += 1;
			if (last_hid_status != -1) {
				_sta_co_cnt[last_hid_status][hid_sta_id] += 1;
				last_hid_status = hid_sta_id;
			}
			last_hid_status = hid_sta_id;
		}
	}
	return true;
}

void HMM::calc_confused_matrix() {
	for (int j = 1; j <= _N; j ++) {
		for (int k = 1; k <= _T; k ++) {
			_B[j][k] = 1.0 * (1.0 * _hid_obv_cnt[j][k]) / _sta_cnt[j];
		}
	}
}

void HMM::calc_tranfer_matrix() {
	for (int j = 1; j <= _N; j ++) {
		for (int k = 1; k <= _N; k ++) {
			_A[j][k] = 1.0 * (1.0 * _sta_co_cnt[j][k]) / _sta_cnt[j];
		}
	}
}


void HMM::save_model(const char *file_name) {
	std::ofstream fos(file_name);	
	// save _T, _N
	fos << _T << "\t" << _N << std::endl;
	// save pi
	for (int i = 1; i <= _N; i ++) {
		if (i < _N) {
			fos << _pi[i] << "\t";
		} else {
			fos << _pi[i] << "\n";
		}
	}
	// save transfer matrix _A
	for (int i = 1; i <= _N; i ++) {
		for (int j = 1; j <= _N; j ++) {
			if (j < _N) {
				fos << _A[i][j] << "\t";
			} else {
				fos << _A[i][j] << std::endl;
			}
		}
	}
	// save confused matrix _B
	for (int i = 1; i <= _N; i ++) {
		for (int j = 1; j <= _T; j ++) {
			if (j < _T) {
				fos << _B[i][j] << "\t";
			} else {
				fos << _B[i][j] << std::endl;
			}
		}
	}
	fos.close();
}

bool HMM::load_model(const char *file_name) {
	// load _T and _N
	std::ifstream fis(file_name);
	try {
		fis >> _T >> _N;
	} catch(std::exception &e) {
		std::cerr << "Error when loading model : loading T & N error!" << std::endl;
		return false;
	}
	init_space();
	// loading pi	
	try {
		for (int j = 1; j <= _N; j ++) {
			fis >> _pi[j];
		}
	} catch (std::exception &e) {
		std::cerr << "Error when loading model : loading pi error!" << std::endl;
		return false;
	}
	// loading transfer matrix
	try {
		for (int j = 1; j <= _N; j ++) {
			for (int k = 1; k <= _N; k ++) {
				fis >> _A[j][k];
			}
		}
	} catch (std::exception &e) {
		std::cerr << "Error when loading model : loading transfer matrix error! " << std::endl;
		return false;
	}
	// loading confused matrix
	try {
		for (int j = 1; j <= _N; j ++) {
			for (int k = 1; k <= _T; k ++) {
				fis >> _B[j][k];
			}
		}
	} catch (std::exception &e) {
		std::cerr << "Error when loading model : loading confused model error! " << std::endl;
		return false;
	}
	return true;
}


void HMM::statistic_train() {
	int tot_status_cnt = 0;
	// calculate confused matrix
	calc_confused_matrix();
	// calculate the transfer matrix
	calc_tranfer_matrix();
	// calculate the pi
	
	for (int i = 1; i <= _N; i ++) {
		tot_status_cnt += _sta_cnt[i];
	}

	for (int i = 1; i <= _N; i ++) {
		_pi[i] = 1.0 * _sta_cnt[i] / tot_status_cnt;
	}
	// save the model
}

bool HMM::viterbi_seg(const std::vector<int> &observed_seq, std::vector<int> &hidden_status) {
	int T;
	std::vector<std::vector<float> > beta;
	
	// opt path saves the optimal path from the source status to the end status
	// opt_path[i][j] saves the status from which current status transfers can gain the
	// maximum probability
	std::vector<std::vector<int> > opt_path;
	float max_value, max_index;
	T = observed_seq.size();
	for (int j = 0; j <= T; j ++) {
		beta.push_back(std::vector<float>(_N + 1, 0));
		opt_path.push_back(std::vector<int>(_N + 1, 0));
	}
	
	for (int j = 1; j <= _N; j ++) {
		beta[1][j] = log(_pi[j])  + log(_B[j][observed_seq[0]]);
		//beta[1][j] = _pi[j] * _B[j][observed_seq[0]];
		opt_path[1][j] = j;
	}

	for (int t = 2; t <= T; t ++) {
		for (int i = 1; i <= _N; i ++) {
			max_value = -1000000;
			for (int j = 1; j <= _N; j ++) {
				// float temp_val = beta[t - 1][j] + log(_A[j][i]) + log(_B[i][observed_seq[t - 1]]);
				if ((i == 1 && (j == 3 || j == 4)) ||
						(i == 2 && (j == 1 || j == 2 || j == 4)) ||
						(i == 3 && (j == 1 || j == 2 || j == 4)) ||
						(i == 4 && (j == 3 || j == 4))){	
				
				//	float temp_val = beta[t - 1][j] + _A[j][i] * _B[i][observed_seq[t-1]];
					float temp_val = beta[t - 1][j] + log(_A[j][i]) + log(_B[i][observed_seq[t - 1]]);
					if (temp_val > max_value) {
						max_value = temp_val;
						max_index = j;
					}
				}
			}
			beta[t][i] = max_value;
			opt_path[t][i] = max_index;
		}
	}


	max_value = 0.0;
	for (int j = 1; j <= _N; j ++) {
		if (max_value < beta[T][j] && (j == 3 || j == 4)) {
			max_value = beta[T][j];
			max_index = j;
		}
	}

	hidden_status.push_back(max_index);
	for (int j = T; j >= 2; j --) {
		max_index = opt_path[j][max_index];
		hidden_status.push_back(max_index);
	}

}

bool HMM::viterbi(const std::vector<int> &observed_seq, std::vector<int> &hidden_status) {
	int T;
	std::vector<std::vector<float> > beta;
	
	// opt path saves the optimal path from the source status to the end status
	// opt_path[i][j] saves the status from which current status transfers can gain the
	// maximum probability
	std::vector<std::vector<int> > opt_path;
	float max_value, max_index;
	T = observed_seq.size();
	for (int j = 0; j <= T; j ++) {
		beta.push_back(std::vector<float>(_N + 1, 0));
		opt_path.push_back(std::vector<int>(_N + 1, 0));
	}
	
	for (int j = 1; j <= _N; j ++) {
		//beta[1][j] = log(_pi[j])  + log(_B[j][observed_seq[0]]);
		beta[1][j] = _pi[j] * _B[j][observed_seq[0]];
		//cout << beta[1][j] << " " << std::endl;
		opt_path[1][j] = j;
	}

	for (int t = 2; t <= T; t ++) {
		for (int i = 1; i <= _N; i ++) {
			max_value = -1000000;
			for (int j = 1; j <= _N; j ++) {
				// float temp_val = beta[t - 1][j] + log(_A[j][i]) + log(_B[i][observed_seq[t - 1]]);
				float temp_val = beta[t - 1][j] + _A[j][i] * _B[i][observed_seq[t-1]];
				if (temp_val > max_value) {
					max_value = temp_val;
					max_index = j;
				}
			}
			beta[t][i] = max_value;
			opt_path[t][i] = max_index;
		}
	}

	max_value = 0.0;
	for (int j = 1; j <= _N; j ++) {
		if (max_value < beta[T][j]) {
			max_value = beta[T][j];
			max_index = j;
		}
	}

	hidden_status.push_back(max_index);
	for (int j = T; j >= 2; j --) {
		max_index = opt_path[j][max_index];
		hidden_status.push_back(max_index);
	}
}

void HMM::em_train() {

}

}
