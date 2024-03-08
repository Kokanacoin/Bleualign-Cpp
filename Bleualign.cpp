/*************************************************************************************
* File Name: Bleualign.cpp
* Author: Hu Keming
* Email: kokana0321@gmail.com
* Time: 2021.08.19
*************************************************************************************/
#include "stdafx.h"
#include "Bleualign.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <codecvt>
#include <sstream>
#include "algorithm"
#include "iterator"
#include <set>
#include <cmath>

using namespace std;

#include <string>
std::string Bleu_util::ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);        
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, _Source, _Dsize);
	std::string result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::wstring Bleu_util::s2ws(const std::string& s)
{
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, "C");
	return result;
}

void Bleu_util::split(vector<string>& sv, const string& s,  const char flag) {
	sv.clear();
	istringstream iss(s);
	string temp;

	while (getline(iss, temp, flag)) {
		sv.push_back(temp);
	}
	return;
}

void Bleualign::init_4files(string source_file, string target_file, vector<string> src2trans, vector<string> tar2trans) {

	this->source_file = source_file;
	this->target_file = target_file;
	this->src2trans = src2trans;
	this->tar2trans = tar2trans;

	this->readFile(this->source_file,this->source_data);
	this->readFile(this->target_file, this->target_data);
	for (int i = 0; i < src2trans.size(); i++) {
		vector<string> temp;
		this->readFile(src2trans[i], temp);
		this->source_trans_datas.push_back(temp);
	}
	for (int i = 0; i < tar2trans.size(); i++) {
		vector<string> temp;
		this->readFile(tar2trans[i], temp);
		this->target_trans_datas.push_back(temp);
	}
}

void Bleualign::init_4files(vector<string> source_data, vector<string> target_data, vector<vector<string>> source_trans_datas, vector<vector<string>> target_trans_datas) {

	for (auto each : source_data) {
		if (each != "") {
			this->source_data.push_back(each);
		}
	}
	for (auto each : target_data) {
		if (each != "") {
			this->target_data.push_back(each);
		}
	}

	for (int i = 0; i < source_trans_datas.size(); i++) {
		vector<string> temp;
		for (auto each : source_trans_datas[i]) {
			if (each != "") {
				temp.push_back(each);
			}
		}
		this->source_trans_datas.push_back(temp);
	}

	for (int i = 0; i < target_trans_datas.size(); i++) {
		vector<string> temp;
		for (auto each : target_trans_datas[i]) {
			if (each != "") {
				temp.push_back(each);
			}
		}
		this->target_trans_datas.push_back(temp);
	}


}

void Bleualign::readFile(string file_name, vector<string>& content_list) {
	
	/*std::ifstream ifs(file_name);
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	while (!ifs.eof()){
		string line;
		getline(ifs, line);
		wstring wb = conv.from_bytes(line);
		content_list.push_back(Bleu_util::ws2s(wb));
	}*/

	std::ifstream ifs(file_name);
	while (!ifs.eof()) {
		string line;
		getline(ifs, line);
		if (line == "") {
			continue;
		}
		content_list.push_back(line);
	}


}

void Bleualign::writeFile() {

	// final_result顺序与第一次遍历相同
	//source_data -- > target_data
	ofstream out1(Bleualign::source_file + "-s");
	ofstream out2(Bleualign::target_file + "-s");
	//if (out.is_open())
	//{
	//	out << "This is a line.\n";
	//	out << "This is another line.\n";
	//	out.close();
	//}

	//each_pair 前面的是汉语 后面是德语
	for (auto each_pair : Bleualign::final_result) {
		
		for (int i = 0; i < each_pair[1].size(); i++) {
			if (i == each_pair[1].size() - 1) {
				out1 << Bleualign::source_data[each_pair[1][i]];
			}
			else {
				out1 << Bleualign::source_data[each_pair[1][i]]<<" ";
			}
			
		}
		out1 << "\n";
		
		for (int i = 0; i < each_pair[0].size(); i++) {
			if (i == each_pair[0].size() - 1) {
				out2 << Bleualign::target_data[each_pair[0][i]];
			}
			else {
				out2 << Bleualign::target_data[each_pair[0][i]] << " ";
			}
			
		}
		out2 << "\n";
	}
	out1.close();
	out2.close();
}

void Bleualign::splitLine(vector<string> original,vector<vector<string>> &original_split){
	for (int i = 0; i < original.size(); i++) {
		vector<string> temp;
		Bleu_util::split(temp, original[i], ' ');
		for (int j = 0; j < temp.size(); j++) {
			transform(temp[j].begin(), temp[j].end(), temp[j].begin(), ::tolower);
		}
		original_split.push_back(temp);
	}
}

void Bleualign::showVector(vector<string> list) {
	int size = list.size();
	for(int i = 0; i < size; i++) {
		cout << list[i] << "  ";
	}
	printf("\n");
}

void Bleualign::showVector(vector<int> list) {
	int size = list.size();
	for (int i = 0; i < size; i++) {
		cout << list[i] << "  ";
	}
	printf("\n");

}

void Bleualign::gapFiller(vector<int> sourcegap,
	vector<int> targetgap, 
	vector<vector<int>> lastpair,
	vector<int> src,
	vector<int> target, 
	vector<vector<string>> translate,
	vector<vector<string>> original,
	vector<vector<int>> &return_pair,
	set<vector<vector<int>>> &result) {
	
	//はじめる　ここからだ！
	//翻译 //多一个垃圾值
	vector<vector<string>> evalsrc;
	vector<vector<int>> evalsrc_index;

	//原文 //少一个 最后当前单独的
	vector<vector<string>> evaltarget;
	vector<vector<int>> evaltarget_index;

	// 如果刚刚好是第一个 这个lastpair为空
	//如果为空就不加入前者
	if (lastpair[0][0] == -1) {
		//evalsrc_index.push_back(vector<int>(1,-1));
		//evalsrc.push_back(vector<string>(1," "));

		evalsrc_index.push_back(src);
		evalsrc.push_back(translate[src[0]]);
	}
	else {

		evalsrc_index.push_back(lastpair[0]);
		evalsrc.push_back(translate[lastpair[0][0]]);

		evalsrc_index.push_back(src);
		evalsrc.push_back(translate[src[0]]);

		if (sourcegap.size() > 0) {
			//加入前者
			vector<int> vec3_temp;
			vec3_temp.insert(vec3_temp.end(), lastpair[0].begin(), lastpair[0].end());
			vec3_temp.insert(vec3_temp.end(), sourcegap.begin(), sourcegap.begin() + 1);
			evalsrc_index.push_back(vec3_temp);

			vector<string> vec_s_temp;
			vec_s_temp.insert(vec_s_temp.end(), translate[lastpair[0][0]].begin(), translate[lastpair[0][0]].end());
			vec_s_temp.insert(vec_s_temp.end(),translate[sourcegap[0]].begin(), translate[sourcegap[0]].end());
			evalsrc.push_back(vec_s_temp);

		}
	}
	if (sourcegap.size() > 0) {
		//加入后者
		vector<int> vec3_temp;
		vec3_temp.insert(vec3_temp.end(), sourcegap.end() - 1, sourcegap.end());
		vec3_temp.insert(vec3_temp.end(), src.begin(), src.end());
		evalsrc_index.push_back(vec3_temp);

		vector<string> vec_s_temp;
		vec_s_temp.insert(vec_s_temp.end(), translate[sourcegap[sourcegap.size() - 1]].begin(), translate[sourcegap[sourcegap.size() - 1]].end());
		vec_s_temp.insert(vec_s_temp.end(), translate[src[0]].begin(), translate[src[0]].end());
		evalsrc.push_back(vec_s_temp);
	}

	if (lastpair[1][0] == -1) {
		//evaltarget_index.push_back(vector<int>(1, -1));
		//evaltarget.push_back(vector<string>(1, " "));

		evaltarget_index.push_back(target);
		evaltarget.push_back(original[target[0]]);
	}
	else {
		evaltarget_index.push_back(lastpair[1]);
		evaltarget.push_back(original[lastpair[1][0]]);

		evaltarget_index.push_back(target);
		evaltarget.push_back(original[target[0]]);

		if (targetgap.size() > 0) {
			//加入前者
			vector<int> vec3_temp;
			vec3_temp.insert(vec3_temp.end(), lastpair[1].begin(), lastpair[1].end());
			vec3_temp.insert(vec3_temp.end(), targetgap.begin(), targetgap.begin() + 1);
			evaltarget_index.push_back(vec3_temp);

			vector<string> vec_s_temp;
			vec_s_temp.insert(vec_s_temp.end(), original[lastpair[1][0]].begin(), original[lastpair[1][0]].end());
			vec_s_temp.insert(vec_s_temp.end(), original[targetgap[0]].begin(), original[targetgap[0]].end());
			evaltarget.push_back(vec_s_temp);

		}
	}
	if (targetgap.size() > 0) {
		//加入后者
		vector<int> vec3_temp;
		vec3_temp.insert(vec3_temp.end(), targetgap.end() - 1, targetgap.end());
		vec3_temp.insert(vec3_temp.end(), target.begin(), target.end());
		evaltarget_index.push_back(vec3_temp);

		vector<string> vec_s_temp;
		vec_s_temp.insert(vec_s_temp.end(), original[targetgap[targetgap.size() - 1]].begin(), original[targetgap[targetgap.size() - 1]].end());
		vec_s_temp.insert(vec_s_temp.end(), original[target[0]].begin(), original[target[0]].end());
		evaltarget.push_back(vec_s_temp);
	}

	map<vector<int>, vector<Sorce>> sorce_dict_new;
	this->calculateBleu(evaltarget, evalsrc, sorce_dict_new,false);

	//小列表映射为大列表index
	map<vector<int>, vector<Sorce>> sorce_dict_re;
	map<vector<int>, vector<Sorce>>::iterator iter;
	for (iter = sorce_dict_new.begin(); iter != sorce_dict_new.end(); iter++)
	{
		vector<int> old_key = iter->first;
		vector<Sorce> old_value = iter->second;

		vector<int> new_key = evalsrc_index[old_key[0]];
		vector<Sorce> new_value;

		for (int each = 0; each < old_value.size(); each++) {
			Sorce temp_sorce;
			temp_sorce.bleu_sorce = old_value[each].bleu_sorce;
			temp_sorce.correct[0] = old_value[each].correct[0];
			temp_sorce.correct[1] = old_value[each].correct[1];
			temp_sorce.maped_index = evaltarget_index[old_value[each].maped_index[0]];
			new_value.push_back(temp_sorce);
		}

		sorce_dict_re.insert(make_pair(new_key, new_value));

	}

	//开始添加缝隙上去
	while (sourcegap.size() >= 0 || targetgap.size() >= 0) {
		vector<int> pregapsrc = lastpair[0];
		vector<int> pregaptarget = lastpair[1];
		vector<int> postgapsrc = src;
		vector<int> postgaptarget = target;

		if (sourcegap.size() > 0) {

			if (pregapsrc.size() > 0 && pregapsrc[0] != -1) {
				double oldscore = sorce_dict_re[pregapsrc][0].bleu_sorce;
				int oldcorrect[2] = { sorce_dict_re[pregapsrc][0].correct[0],sorce_dict_re[pregapsrc][0].correct[1] };

				vector<int> combinedID = { pregapsrc[0],sourcegap[0] };

				if (sorce_dict_re.find(combinedID) != sorce_dict_re.end()) {
					double newscore = sorce_dict_re[combinedID][0].bleu_sorce;
					vector<int> newtarget = sorce_dict_re[combinedID][0].maped_index;
					int newcorrect[2] = { sorce_dict_re[combinedID][0].correct[0],sorce_dict_re[combinedID][0].correct[1] };
					if (newscore > oldscore && newcorrect[0] > oldcorrect[0] && newtarget == pregaptarget) {
						lastpair = { combinedID,pregaptarget };
						sourcegap.erase(sourcegap.begin());
						continue;
					}
				}
			}
			if (postgapsrc.size() > 0 && postgapsrc[0] != -1) {
				double oldscore = sorce_dict_re[postgapsrc][0].bleu_sorce;
				int oldcorrect[2] = { sorce_dict_re[postgapsrc][0].correct[0],sorce_dict_re[postgapsrc][0].correct[1] };

				vector<int> combinedID = { sourcegap[sourcegap.size() - 1],postgapsrc[0] };

				if (sorce_dict_re.find(combinedID) != sorce_dict_re.end()) {
					double newscore = sorce_dict_re[combinedID][0].bleu_sorce;
					vector<int> newtarget = sorce_dict_re[combinedID][0].maped_index;
					int newcorrect[2] = { sorce_dict_re[combinedID][0].correct[0],sorce_dict_re[combinedID][0].correct[1] };
					if (newscore > oldscore && newcorrect[0] > oldcorrect[0] && newtarget == postgaptarget) {
						src = combinedID;
						//TODO
						target = postgaptarget;
						//target = postgapsrc;
						sourcegap.pop_back();
						continue;
					}

				}

			}

		}

		if (targetgap.size() > 0) {
			if (pregapsrc.size() > 0 && pregapsrc[0] != -1) {
				double newscore = sorce_dict_re[pregapsrc][0].bleu_sorce;
				vector<int> newtarget = sorce_dict_re[pregapsrc][0].maped_index;
				int newcorrect[2] = { sorce_dict_re[pregapsrc][0].correct[0],sorce_dict_re[pregapsrc][0].correct[1] };

				if (newtarget != pregaptarget && newtarget != postgapsrc) {
					lastpair = { pregapsrc,newtarget };
					for (int x = 0; x < newtarget.size(); x++) {
						for (int y = 0; y < targetgap.size(); y++) {
							if (newtarget[x] == targetgap[y]) {
								targetgap.erase(targetgap.begin() + y);
								break;
							}
						}
					}
					continue;
				}

			}
		
			if (postgapsrc.size() > 0 && postgapsrc[0] != -1) {
				double newscore = sorce_dict_re[postgapsrc][0].bleu_sorce;
				vector<int> newtarget = sorce_dict_re[postgapsrc][0].maped_index;
				int newcorrect[2] = { sorce_dict_re[postgapsrc][0].correct[0],sorce_dict_re[postgapsrc][0].correct[1] };

				if (newtarget != postgaptarget && newtarget != pregaptarget) {
					
					//TODO
					//lastpair = { postgapsrc,newtarget };
					src = postgapsrc;
					target = newtarget;
					for (int x = 0; x < newtarget.size(); x++) {
						for (int y = 0; y < targetgap.size(); y++) {
							if (newtarget[x] == targetgap[y]) {
								targetgap.erase(targetgap.begin() + y);
								break;
							}
						}
					}
					continue;
				}
			}
		
		}

		break;

	}


	bool flag = true;
	//首先lastpair部为空
	if (lastpair.size() > 0 && lastpair[0][0] != -1 && lastpair[1][0] != -1) {

		for (auto each : result)
		{	
			if (lastpair == each) {
				flag = false;
				break;
			}
		}
	}

	if (lastpair.size() > 0 && lastpair[0][0] != -1 && lastpair[1][0] != -1) {
		if (flag) {
			result.insert(lastpair);
		}
	}
	return_pair = { src,target };



}

void Bleualign::gapFinder(vector<vector<string>> original, vector<vector<string>> translate, vector<vector<int>> bleualign_map,set<vector<vector<int>>> &result) {
	
	vector<vector<int>> lastpair(2, vector<int>(1, -1));


	for (int i = 0; i < bleualign_map.size(); i++) {
		vector<int> src = { bleualign_map[i][0] };
		vector<int> target = { bleualign_map[i][1] };

		vector<int> oldsrc = lastpair[0];
		vector<int> oldtarget = lastpair[1];

		/*if (oldsrc.size() == 0) {
			
		}*/

		vector<int> sourcegap;
		for (int each = oldsrc[oldsrc.size() - 1] + 1; each < src[0]; each++) {
			sourcegap.push_back(each);
		}

		vector<int> targetgap;
		for (int each = oldtarget[oldtarget.size() - 1] + 1; each < target[0]; each++) {
			targetgap.push_back(each);
		}
		//如果存在缝隙就计算缝隙
		if (targetgap.size() > 0 || sourcegap.size() > 0) {

			vector<vector<int>> return_pair;
			this->gapFiller(sourcegap, targetgap, lastpair, src, target, translate, original, return_pair, result);
			lastpair = return_pair;
		}
		//不存在缝隙就加入上一找到的对子到最终关系里
		else {
			result.insert(lastpair);
			lastpair = { src ,target };
		}	

	}
	result.insert(lastpair);

}

void Bleualign::pathFinder(const int tran_len, const  int ori_len, map<vector<int>, vector<Sorce>> map_list, vector<vector<int>> &bleualign_map) {
	
	
	
	int i = tran_len + 1;
	int j = ori_len + 1;

	double process = 0;
	double all = tran_len * ori_len;

	vector<vector<double>> matrix(i, vector<double>(j, 0));
	vector<vector<string>> pointers(tran_len, vector<string>(ori_len, ""));
	for (int i = 0; i < tran_len; i++) {
		for (j = 0; j < ori_len; j++) {
			process = i * j * 100 / all;
			double best_score = matrix[i][j + 1];
			string best_pointer = "^";

			double score = matrix[i + 1][j];
			if (score > best_score) {
				best_score = score;
				best_pointer = "<";
			}
			
			vector<int> tempi = { i };
			vector<int> tempj = { j };
			bool flag = false;
			int find_k = -1;
			for (int k = 0; k < map_list[tempi].size(); k++) {
				if (map_list[tempi][k].maped_index == tempj) {
					flag = true;
					find_k = k;
					break;
				}
			}

			if (flag) {
				score = map_list[tempi][find_k].bleu_sorce + matrix[i][j];
				if (score > best_score) {
					best_score = score;
					best_pointer = "match";
				}
			}


			matrix[i + 1][j + 1] = best_score;
			pointers[i][j] = best_pointer;
		}
		printf("\r[INFO]---寻路找寻最后bleu解...%.1f%%", process);
	}
	printf("\n[INFO]---进行反向寻路\n");

	i = pointers.size() - 1;
	j = pointers[0].size() - 1;
	string pointer = "";
	while (i >= 0 && j >= 0) {
		pointer = pointers[i][j];
		if (pointer == "^") {
			i--;
		}
		else if (pointer == "<") {
			j--;
		}
		else if(pointer == "match"){
			vector<int> temp{ i,j };
			bleualign_map.push_back(temp);
			i--;
			j--;
		}
	}
	std::reverse(bleualign_map.begin(), bleualign_map.end());
	
}

void Bleualign::calculateBleu(vector<vector<string>> original, vector<vector<string>> translate, map<vector<int>, vector<Sorce>> &map_list,bool logflag) {
	

	// 原文的统计信息
	vector<CookData> cook_cache;
	Bleualign::cookSet(original, cook_cache);
	if (logflag) {
		printf("[INFO]---正在计算bleu值\n");
	}
	int Number = 0;
	for (int i = 0; i < translate.size(); i++) {
		Number++;
		int cooked_testlen = translate[i].size();
		vector<int> cooked_guess;
		map<vector<string>, int> refmaxcounts;
		
		for (int a = 1; a < Bleualign::bleu_ngrams + 1; a++) {
			cooked_guess.push_back(cooked_testlen - a + 1 > 0 ? cooked_testlen - a + 1 : 0);
		}

		//统计词出现次数
		if (translate[i].size() > 0) {
			for (int j = 1; j <= Bleualign::bleu_ngrams; j++) {
				for (int k = 0; k < translate[i].size() - j + 1; k++) {

					vector<string>::const_iterator first1 = translate[i].begin() + k; // 第三个迭代器
					vector<string>::const_iterator last1 = translate[i].begin() + k + j; // 倒数第二个迭代器
					vector<string> ngram(first1, last1);
					//如果找到了
					if (refmaxcounts.find(ngram) != refmaxcounts.end()) {
						refmaxcounts[ngram]++;
					}//如果是新的
					else {
						refmaxcounts.insert(make_pair(ngram, 1));
					}
				}
			}
		}


		//每个ngram下的分词
		map<int, set<vector<string>>> ngrams_sorted;
		map<vector<string>, int>::iterator it;

		//按照每个ngram 将词区分开
		for (it = refmaxcounts.begin(); it != refmaxcounts.end(); it++) {

			ngrams_sorted[it->first.size() - 1].insert(it->first);
		}
		
		vector<Sorce> scorelist;

		//一一对应求解bleu
		for (int or_index = 0; or_index < cook_cache.size(); or_index++) {
			
			//这里需要 此行翻译文 原文取交集
			vector<vector<string>> ngrams_filtered;

			//当前行翻译
			//ngrams_sorted[N] 是一个集合 里面存这字符串列表
			set<vector<string>> now_tran = ngrams_sorted[Bleualign::bleu_ngrams - 1];
			set<vector<string>> now_ori = cook_cache[or_index].refset;

			set_intersection(ngrams_sorted[Bleualign::bleu_ngrams - 1].begin(), ngrams_sorted[Bleualign::bleu_ngrams - 1].end(), cook_cache[or_index].refset.begin(), cook_cache[or_index].refset.end(), back_inserter(ngrams_filtered));
			
			//出现了相同的元素的情况
			if (ngrams_filtered.size() > 0) {
				//int cooked_reflen = cook_cache[or_index].reflen;
				int cooked_reflen = original[or_index].size();
				int cooked_correct[Bleualign::bleu_ngrams] = { 0 };

				//gram最多的情况
				for (int each_ngrams_filtered = 0; each_ngrams_filtered < ngrams_filtered.size(); each_ngrams_filtered++) {
					cooked_correct[Bleualign::bleu_ngrams - 1] += cook_cache[or_index].refmaxcounts[ngrams_filtered[each_ngrams_filtered]] > refmaxcounts[ngrams_filtered[each_ngrams_filtered]] ? refmaxcounts[ngrams_filtered[each_ngrams_filtered]]:cook_cache[or_index].refmaxcounts[ngrams_filtered[each_ngrams_filtered]];
					
				}
				
				//其余gram的情况
				for (int order = 0; order < Bleualign::bleu_ngrams - 1; order++) {
					vector<vector<string>> temp_set;
					set_intersection(ngrams_sorted[order].begin(), ngrams_sorted[order].end(), cook_cache[or_index].refset.begin(), cook_cache[or_index].refset.end(), back_inserter(temp_set));
					for (int each = 0; each < temp_set.size(); each++) {
						cooked_correct[order] += cook_cache[or_index].refmaxcounts[temp_set[each]] > refmaxcounts[temp_set[each]] ? refmaxcounts[temp_set[each]] : cook_cache[or_index].refmaxcounts[temp_set[each]];
					}
				}
				double logbleu = 0.0;
				for (int n = 0; n < Bleualign::bleu_ngrams; n++) {
					logbleu += log(cooked_correct[n]) - log(cooked_guess[n]);
				}
				logbleu /= Bleualign::bleu_ngrams;
				logbleu += (1 - (double)cooked_reflen / cooked_testlen) > 0 ? 0 : (1 - (double)cooked_reflen / cooked_testlen);
				double score = exp(logbleu);

				//处理反方向的数
				vector<int> cooked2_guess;
				for (int k = 1; k < Bleualign::bleu_ngrams + 1; k++) {
					cooked2_guess.push_back(cooked_reflen - k + 1 > 0 ? cooked_reflen - k + 1 : 0);
				}
				logbleu = 0.0;

				for (int k = 0; k < Bleualign::bleu_ngrams; k++) {
					logbleu += log(cooked_correct[k]) - log(cooked2_guess[k]);
				}
				logbleu /= Bleualign::bleu_ngrams;
				logbleu += (1 - (double)cooked_testlen / cooked_reflen) > 0 ? 0 : 1 - (double)cooked_testlen / cooked_reflen;
				double score2 = exp(logbleu);

				double meanscore = (2 * score * score2) / (score + score2);

				Sorce temp_sorce;
				temp_sorce.bleu_sorce = meanscore;
				vector<int> va =  { or_index };
				temp_sorce.maped_index = va;
				temp_sorce.correct[0] = cooked_correct[0];
				temp_sorce.correct[1] = cooked_correct[1];
				
				
				//scorelist.push_back(temp_sorce);
				//最好只要3个最大的值
				if (scorelist.size() < 3) {
					scorelist.push_back(temp_sorce);
					for (int x = 0; x < scorelist.size() - 1; x++) {
						for (int y = 0; y < scorelist.size() - 1 - x; y++) {
							if (scorelist[y].bleu_sorce < scorelist[y + 1].bleu_sorce) {
								Sorce t;
								t = scorelist[y];
								scorelist[y] = scorelist[y + 1];
								scorelist[y + 1] = t;
							}
						}
					}
				}
				else {
					for (int x = 0; x < scorelist.size(); x++) {
						if (temp_sorce.bleu_sorce > scorelist[x].bleu_sorce) {
							scorelist.insert(scorelist.begin() + x, temp_sorce);
							scorelist.pop_back();
							break;
						}
					}
				}

			}
			
		}
		
		//printf("\n========[%d]===========\n",cccc);
		//for (int e = 0; e < scorelist.size(); e++) {
		//	printf("%f\t",scorelist[e].bleu_sorce);
		//}
		if (scorelist.size() != 0) {
			vector<int> temp_index = { i };
			map_list.insert(make_pair(temp_index, scorelist));
		}
		if (logflag) {
			printf("\r[INFO]---正在处理ing......[%d]/[%d]", Number, translate.size());
		}
		
	}
	if (logflag) {
		printf("\n[INFO]---计算bleu完毕\n");
	}
	
}

void Bleualign::cookSet(vector<vector<string>> content, vector<CookData> &cook_data) {
	 //每一行分词了
	for (int i = 0; i < content.size(); i++) {
		CookData now_cookdata;
		now_cookdata.reflen = 0;
		now_cookdata.index = i;
		if (content[i].size() <= 0) {
			//now_cookdata.reflen = 0;
			//cook_data.push_back(now_cookdata);
			continue;
		}

		//要按照ngram中的n分词
		for (int j = 1; j <= Bleualign::bleu_ngrams; j++) {
			for (int k = 0; k < content[i].size() - j + 1; k++) {

				now_cookdata.reflen++;
				vector<string>::const_iterator first1 = content[i].begin() + k; // 第三个迭代器
				vector<string>::const_iterator last1 = content[i].begin() + k + j; // 倒数第二个迭代器
				vector<string> ngram(first1, last1);
				//如果找到了
				if (now_cookdata.refmaxcounts.find(ngram) != now_cookdata.refmaxcounts.end()) {
					now_cookdata.refmaxcounts[ngram]++;
				}//如果是新的
				else {
					now_cookdata.refmaxcounts.insert(make_pair(ngram, 1));
					now_cookdata.refset.insert(ngram);
				}
			}

		}
		//now_cookdata.refmaxcounts = refmaxcounts;
		//now_cookdata.refset = refset;
		//now_cookdata.reflen = reflen;
		cook_data.push_back(now_cookdata);
	}
}

bool Bleualign::mainLoop(){
	
	if (this->source_data.size() == 0 || this->target_data.size() == 0 || (this->source_trans_datas.size() == 0 && this->target_trans_datas.size() == 0)) {
		cout << "[ERROR]---输入数据找不到或可能出现问题" << endl;
		return false;
	}

	vector<vector<string>> source_split;
	vector<vector<string>> target_split;
	
	printf("[INFO]---正在对原文文本进行切割\n");
	this->splitLine(this->source_data, source_split);
	this->splitLine(this->target_data, target_split);


	//A 译B的映射列表
	map<vector<int>, vector<int>> map_list1;
	//B 译A的映射列表要
	map<vector<int>, vector<int>> map_list2;


	set<vector<vector<int>>> A_B_set; //target_trans
	set<vector<vector<int>>> B_A_set; //source_trans

	// 循环遍历翻译列表 使之与原文一一对应
	for (int i = 0; i < this->target_trans_datas.size(); i++) {

		//切割翻译
		vector<vector<string>> target_trans_split;
		this->splitLine(this->target_trans_datas[i], target_trans_split);


		// 先分词 出现两组列表
		// 进行计算bleu
		map<vector<int>,vector<Sorce>> sorce_dict;
		Bleualign::calculateBleu(source_split, target_trans_split, sorce_dict);
		
		//寻路
		vector<vector<int>> bleualign_map;
		Bleualign::pathFinder(target_trans_split.size(), source_split.size(), sorce_dict, bleualign_map);

		//找到空隙并且整理好结果
		set<vector<vector<int>>> result;
		Bleualign::gapFinder(source_split, target_trans_split, bleualign_map, result);

		if (A_B_set.size() == 0) {
			A_B_set = result;
		}
		else {
			set<vector<vector<int>>> temp;
			std::set_intersection(std::begin(A_B_set), std::end(A_B_set), std::begin(result), std::end(result), std::inserter(temp, std::begin(temp)));
			A_B_set = temp;
		}
		
		
	}

	for (int i = 0; i < this->source_trans_datas.size(); i++) {
		//切割翻译
		vector<vector<string>> source_trans_split;
		this->splitLine(this->source_trans_datas[i], source_trans_split);

		map<vector<int>, vector<Sorce>> sorce_dict;
		Bleualign::calculateBleu(target_split, source_trans_split, sorce_dict);

		vector<vector<int>> bleualign_map;
		Bleualign::pathFinder(source_trans_split.size(), target_split.size(), sorce_dict, bleualign_map);

		set<vector<vector<int>>> result;
		Bleualign::gapFinder(target_split, source_trans_split, bleualign_map, result);
		
		if (B_A_set.size() == 0) {
			B_A_set = result;
		}
		else {
			set<vector<vector<int>>> temp;
			std::set_intersection(std::begin(B_A_set), std::end(B_A_set), std::begin(result), std::end(result), std::inserter(temp, std::begin(temp)));
			B_A_set = temp;
		}

	}


	set<vector<vector<int>>> B_A_set_new;
	//需要B_A 对调
	for (auto each : B_A_set) {
		B_A_set_new.insert({ each[1],each[0] });
	}

	if (this->source_trans_datas.size() == 0) {
		this->final_result = A_B_set;
	}
	else if (this->target_trans_datas.size() == 0) {
		this->final_result = B_A_set_new;
	}
	else {
		std::set_intersection(std::begin(A_B_set), std::end(A_B_set), std::begin(B_A_set_new), std::end(B_A_set_new), std::inserter(this->final_result, std::begin(this->final_result)));
	}
	
	this->writeFile();
	

	return true;
}

set<vector<vector<int>>> Bleualign::getPair() {
	return this->final_result;
}

int main()
{

	Bleualign bleualign;
	// 事先准备文本数据
	string source_file = "data/de.seg.token";
	string target_file = "data/zh.seg.token";
	vector<string> source_trans = {"data/de.seg.trans.de2zh.token"};
	vector<string> target_trans = {"data/zh.seg.trans.zh2de.token"};

	//string source_file = "japandata/src.txt.token";
	//string target_file = "japandata/tgt.txt.token";
	//vector<string> source_trans = { "japandata/src.txt.f.token" };
	//vector<string> target_trans = { "japandata/tgt.txt.f.token" };

	vector<string> source_data;
	vector<string> target_data;
	vector<vector<string>> source_trans_datas;
	vector<vector<string>> target_trans_datas;

	bleualign.readFile(source_file, source_data);
	bleualign.readFile(target_file, target_data);
	for (int i = 0; i < source_trans.size(); i++) {
		vector<string> temp;
		bleualign.readFile(source_trans[i], temp);
		source_trans_datas.push_back(temp);
	}
	for (int i = 0; i < target_trans.size(); i++) {
		vector<string> temp;
		bleualign.readFile(target_trans[i], temp);
		target_trans_datas.push_back(temp);
	}

	//start
	//bleualign.init_4files(source_file, target_file, source_trans, target_trans);
	//source_trans_datas target_trans_datas可能其中一个为空
	bleualign.init_4files(source_data, target_data, source_trans_datas, target_trans_datas);
	bool sucess = bleualign.mainLoop();
    return 0;
}

