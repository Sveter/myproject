#pragma once
#include <unordered_map>
#include <unordered_set>
#include <cppjieba/jieba.hpp>
#include <vector>
class TextSimilarity
{
public:
	typedef std::unordered_map<std::string, int> wordFreq;
	typedef std::unordered_set<std::string> wordSet;
	TextSimilarity(std::string dict);
	double getTextSimilarity(const char* file1, const char* file2);

private:

	void getStopWordTable(const char* stopWordFile); //Õ£”√¥ 
	wordFreq getWordFreq(const char* file); //¥ ∆µÕ≥º∆
	std::string UTF8ToGBK(std::string str);
	std::string GBKToUTF8(std::string str);
	std::vector<std::pair<std::string, int>> sortByValueReverse(wordFreq& wf);
	void selectAimWords(std::vector<std::pair<std::string, int>>& wfvec, wordSet& wset);
	std::vector<double> getOneHot(wordSet& wset, wordFreq& wf);
	double cosine(std::vector<double> oneHot1, std::vector<double> oneHot2);

	std::string DICT;
	std::string DICT_PATH;
	std::string HMM_PATH;
	std::string USER_DICT_PATH;
	std::string IDF_PATH;
	std::string STOP_WORD_PATH;
	cppjieba::Jieba _jieba;

	wordSet _stopWordSet;
	int _maxWordNumber;
};