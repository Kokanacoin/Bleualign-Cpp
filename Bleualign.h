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
* @brief bleu����
*/
typedef struct sorce
{
	double bleu_sorce;
	vector<int> maped_index;
	int correct[2];

}Sorce;

/**
* @brief �ִʴ���֮����б�
*/
typedef struct cookData {

	int index; /*���е�����index*/
	int reflen; /*���б����˶��ٸ���*/
	map<vector<string>, int> refmaxcounts; /*�ִʺ�ÿ���ʳ��ֵ�������*/
	set<vector<string>> refset; /*refmaxcounts�е�key�ļ���*/

}CookData;

/**
* @brief �ı�������
*/
class Bleualign {
public:
	
	static const int bleu_ngrams = 2;

	/**
	* @brief ��ȡ�ļ�����
	* @param source_file	ԭ��A
	* @param target_file	ԭ��B
	* @param src2trans		ԭ��A�ķ����б�     
	* @param tar2trans		ԭ��B�ķ����б�
	*/
	void init_4files(string source_file, string target_file, vector<string> src2trans, vector<string> tar2trans);

	/**
	* @brief ��ȡ�ļ�����
	* @param source_data	ԭ��A
	* @param target_data	ԭ��B
	* @param source_trans_datas		ԭ��A�ķ����б�
	* @param target_trans_datas		ԭ��B�ķ����б�
	*/
	void init_4files(vector<string> source_data, vector<string> target_data, vector<vector<string>> source_trans_datas, vector<vector<string>> target_trans_datas);


	/**
	* @brief ��ȡ�ļ�
	* @param file_name	�ļ�����
	* @param content_list	�洢���б�
	*/
	void readFile(string file_name, vector<string>& content_list);

	/**
	* @brief ��ӡvoctor�б�
	* @param list	һάvector�б�
	*/
	static void showVector(vector<string> list);
	static void showVector(vector<int> list);

	/**
	* @brief ��ѭ����ʼ
	*
	* @return �Ƿ���϶���
	*/
	bool mainLoop();
	
	/*
	* @brief ��ȡƥ��ɹ��Ķ���
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
	* @brief �ָ��б�
	* @param original			�з�ǰ��ԭ��
	* @param original_split		�зֺ��ԭ���б�
	*/
	void splitLine(vector<string>original, vector<vector<string>> &original_split);

	/**
	* @brief �����м���bleu
	* @param original ԭ�ķִʺ�
	* @param translate ����ִʺ�
	* @param &map_list ����÷��ֵ�
	* @param logflag �Ƿ������־״̬
	*/
	void calculateBleu(vector<vector<string>> original,vector<vector<string>> translate, map<vector<int>, vector<Sorce>> &map_list,bool logflag = true);
	
	/**
	* @brief ���ִʺ���б����ͳ��
	* @param content �ִʺ������
	* @param cook_data ���صĴ����б�
	*/
	void cookSet(vector<vector<string>> content,vector<CookData> &cook_data);

	/**
	* @brief ��̬�滮Ѱ���������
	* @param tran_len ��������
	* @param ori_len ԭ������
	* @param map_list �����б�
	* @param bleualign_map ���ؽ����
	*/
	void pathFinder(const  int tran_len, const int ori_len, map<vector<int>, vector<Sorce>> map_list, vector<vector<int>> &bleualign_map);
	
	/**
	* @brief Ѱ�ҷ�϶
	* @param original �ִʺ��ԭ��
	* @param translate �ִʺ�ķ���
	* @param bleualign_map �÷��б�
	* @param result ���ս����
	*/
	void gapFinder(vector<vector<string>> original, vector<vector<string>> translate, vector<vector<int>> bleualign_map,set<vector<vector<int>>> &result);

	/**
	* @brief ���϶
	* @param sourcegap �����ĵķ�϶�б�
	* @param targetgap	ԭ�ĵķ�϶�б�
	* @param lastpair	��һ�Զ�Ӧ��ϵ
	* @param src		��ǰ�����Ӧindex
	* @param target		��ǰԭ�Ķ�Ӧindex
	* @param translate	�зֺõķ���
	* @param original	�зֺõ�ԭ��
	* @param return_pair	���ص��¶�Ӧ����
	* @param result		�������
	*/
	void gapFiller(vector<int> sourcegap, vector<int> targetgap, vector<vector<int>> lastpair, vector<int> src, vector<int> target, vector<vector<string>> translate, vector<vector<string>> original, vector<vector<int>> &return_pair, set<vector<vector<int>>> &result);

	/**
	* @brief д���ļ�
	*/
	void writeFile();

};

/**
* @brief ������
*/
class Bleu_util {
public:
	/**
	* @brief wstringת��string
	* @param ws	��ת��wstring�ַ���
	*
	* @return string�ַ���
	*/
	static std::string ws2s(const std::wstring& ws);

	/**
	* @brief stringת��wstring
	* @param s	��ת��string�ַ���
	*
	* @return wstring�ַ���
	*/
	static std::wstring s2ws(const std::string& s);

	/**
	* @brief �з��ַ���
	* @param sv	���ص��ַ�������
	* @param s Ҫ�зֵ��ַ���
	* @param flag �зֹ���
	*/
	static void split(vector<string>& sv, const string& s,  const char flag);




};