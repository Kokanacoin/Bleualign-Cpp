/*************************************************************************************
* File Name: Bleualign.h
* Author: Hu Keming
* Email: kokana0321@gmail.com
* Time: 2021.08.19
*************************************************************************************/
#pragma once
#include<vector>;
#include<string>
#include <algorithm> 
#include <map>
# include<set>

using namespace std;



/**
* @brief bleu分数
*/
typedef struct sorce
{
	double bleu_sorce;
	vector<int> maped_index;
	int correct[2];

}Sorce;

/**
* @brief 分词处理之后的列表
*/
typedef struct cookData {

	int index; /*这行的列数index*/
	int reflen; /*这行被分了多少个词*/
	map<vector<string>, int> refmaxcounts; /*分词后每个词出现的最大次数*/
	set<vector<string>> refset; /*refmaxcounts中的key的集合*/

}CookData;

/**
* @brief 文本对齐类
*/
class Bleualign {
public:
	
	static const int bleu_ngrams = 2;

	/**
	* @brief 读取文件函数
	* @param source_file	原文A
	* @param target_file	原文B
	* @param src2trans		原文A的翻译列表     
	* @param tar2trans		原文B的翻译列表
	*/
	void init_4files(string source_file, string target_file, vector<string> src2trans, vector<string> tar2trans);

	/**
	* @brief 读取文件内容
	* @param source_data	原文A
	* @param target_data	原文B
	* @param source_trans_datas		原文A的翻译列表
	* @param target_trans_datas		原文B的翻译列表
	*/
	void init_4files(vector<string> source_data, vector<string> target_data, vector<vector<string>> source_trans_datas, vector<vector<string>> target_trans_datas);


	/**
	* @brief 读取文件
	* @param file_name	文件名字
	* @param content_list	存储的列表
	*/
	void readFile(string file_name, vector<string>& content_list);

	/**
	* @brief 打印voctor列表
	* @param list	一维vector列表
	*/
	static void showVector(vector<string> list);
	static void showVector(vector<int> list);

	/**
	* @brief 主循环开始
	*
	* @return 是否完毕对齐
	*/
	bool mainLoop();
	
	/*
	* @brief 获取匹配成功的对子
	*
	* @return
	*/
	set<vector<vector<int>>> getPair();


private:

	string source_file = "./source_file";
	string target_file = "./target_file";
	vector<string> src2trans;
	vector<string> tar2trans;

	vector<string> source_data;
	vector<string> target_data;
	vector<vector<string>> source_trans_datas;
	vector<vector<string>> target_trans_datas;

	set<vector<vector<int>>> final_result;




	/**
	* @brief 分割列表
	* @param original			切分前的原文
	* @param original_split		切分后的原文列表
	*/
	void splitLine(vector<string>original, vector<vector<string>> &original_split);

	/**
	* @brief 按照行计算bleu
	* @param original 原文分词后
	* @param translate 翻译分词后
	* @param &map_list 结果得分字典
	* @param logflag 是否输出日志状态
	*/
	void calculateBleu(vector<vector<string>> original,vector<vector<string>> translate, map<vector<int>, vector<Sorce>> &map_list,bool logflag = true);
	
	/**
	* @brief 将分词后的列表进行统计
	* @param content 分词后的文章
	* @param cook_data 返回的处理列表
	*/
	void cookSet(vector<vector<string>> content,vector<CookData> &cook_data);

	/**
	* @brief 动态规划寻找最优组合
	* @param tran_len 翻译行数
	* @param ori_len 原文行数
	* @param map_list 分数列表
	* @param bleualign_map 返回结果对
	*/
	void pathFinder(const  int tran_len, const int ori_len, map<vector<int>, vector<Sorce>> map_list, vector<vector<int>> &bleualign_map);
	
	/**
	* @brief 寻找缝隙
	* @param original 分词后的原文
	* @param translate 分词后的翻译
	* @param bleualign_map 得分列表
	* @param result 最终结果集
	*/
	void gapFinder(vector<vector<string>> original, vector<vector<string>> translate, vector<vector<int>> bleualign_map,set<vector<vector<int>>> &result);

	/**
	* @brief 填补缝隙
	* @param sourcegap 翻译文的缝隙列表
	* @param targetgap	原文的缝隙列表
	* @param lastpair	上一对对应关系
	* @param src		当前翻译对应index
	* @param target		当前原文对应index
	* @param translate	切分好的翻译
	* @param original	切分好的原文
	* @param return_pair	返回的新对应对子
	* @param result		结果集合
	*/
	void gapFiller(vector<int> sourcegap, vector<int> targetgap, vector<vector<int>> lastpair, vector<int> src, vector<int> target, vector<vector<string>> translate, vector<vector<string>> original, vector<vector<int>> &return_pair, set<vector<vector<int>>> &result);

	/**
	* @brief 写入文件
	*/
	void writeFile();

};

/**
* @brief 工具类
*/
class Bleu_util {
public:
	/**
	* @brief wstring转换string
	* @param ws	待转换wstring字符串
	*
	* @return string字符串
	*/
	static std::string ws2s(const std::wstring& ws);

	/**
	* @brief string转换wstring
	* @param s	待转换string字符串
	*
	* @return wstring字符串
	*/
	static std::wstring s2ws(const std::string& s);

	/**
	* @brief 切分字符串
	* @param sv	返回的字符串容器
	* @param s 要切分的字符串
	* @param flag 切分规则
	*/
	static void split(vector<string>& sv, const string& s,  const char flag);




};