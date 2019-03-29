#include "TextSimilarity.h"
#include <windows.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <assert.h>
using namespace std;

TextSimilarity::TextSimilarity(string dict)
	: DICT(dict)
	, DICT_PATH(dict+ "/jieba.dict.utf8")
	, HMM_PATH(dict + "/hmm_model.utf8")
	, USER_DICT_PATH(dict + "/user.dict.utf8")
	, IDF_PATH(dict + "/idf.utf8")
	, STOP_WORD_PATH(dict + "/stop_words.utf8")
	, _jieba(DICT_PATH,
		HMM_PATH,
		USER_DICT_PATH,
		IDF_PATH,
		STOP_WORD_PATH)
	,_maxWordNumber(100)
{
	getStopWordTable(STOP_WORD_PATH.c_str());
}

void TextSimilarity::getStopWordTable(const char* stopWordFile)
{
	ifstream fin(stopWordFile);
	if (!fin.is_open())
	{
		cout << "open file:" << stopWordFile << "failed" << endl;
		return;
	}
	string line;
	while (!fin.eof())
	{
		getline(fin, line);
		_stopWordSet.insert(line);
	}
	fin.close();
}
 

TextSimilarity::wordFreq TextSimilarity::getWordFreq(const char* file)
{
	ifstream fin(file);
	if (!fin.is_open())
	{
		cout << "open file:" << file << "failed" << endl;
		return wordFreq();
	}
	string line;
	wordFreq wf;
	while (!fin.eof())
	{
		getline(fin, line);
		line = GBKToUTF8(line);
		vector<string> words;
		_jieba.Cut(line, words, true);
		for (const auto& e : words)
		{
			if (_stopWordSet.count(e) > 0) 
			{
				continue;
			}
			else
			{
				if (wf.count(e) > 0)
					wf[e]++;
				else
					wf[e] = 1;
			}
		}
	}
	fin.close();
	return wf;
}

string TextSimilarity::GBKToUTF8(string str)
{
	//GBK-->UTF16
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, len);
	//UTF16-->UTF8
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* utf8char = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8char, len, NULL, NULL);
	string out(utf8char);
	if (wstr)
	{
		delete[] wstr;
		wstr = NULL;
	}
	if (utf8char)
	{
		delete[] utf8char;
		utf8char = NULL;
	}
	return out;
}

string TextSimilarity::UTF8ToGBK(string str)
{
	//UTF8-->UTF16
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, len);
	//UTF16-->GBK
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* gbkchar = new char[len];
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, gbkchar, len, NULL, NULL);
	string out(gbkchar);
	if (wstr)
	{
		delete[] wstr;
		wstr = NULL;
	}
	if (gbkchar)
	{
		delete[] gbkchar;
		gbkchar = NULL;
	}
	return out;
}

bool cmpReverse(pair<string, int> lp, pair<string, int> rp)
{
	return lp.second > rp.second;
}

vector<pair<string, int>> TextSimilarity::sortByValueReverse(TextSimilarity::wordFreq& wf)
{
	vector<pair<string, int>> wfvector(wf.begin(), wf.end());
	sort(wfvector.begin(), wfvector.end(), cmpReverse);
	return wfvector;
}

void TextSimilarity::selectAimWords(vector<pair<string, int>>& wfvec, TextSimilarity::wordSet& wset) 
{
	int len = wfvec.size();
	int sz = len > _maxWordNumber ? _maxWordNumber : len;
	for (int i = 0; i < sz; i++)
	{
		wset.insert(wfvec[i].first);
	}
}

vector<double> TextSimilarity::getOneHot(TextSimilarity::wordSet& wset, TextSimilarity::wordFreq& wf)
{
	vector<double> oneHot;
	for (const auto& e : wset)
	{
		if (wf.count(e) > 0)
			oneHot.push_back(wf[e]);
		else
			oneHot.push_back(0);
	}
	return oneHot;
}

double TextSimilarity::cosine(vector<double> oneHot1, vector<double> oneHot2) 
{
	double modular1 = 0, modular2 = 0;
	double products = 0;
	assert(oneHot1.size() == oneHot2.size());
	for (size_t i = 0; i < oneHot1.size(); i++)
	{
		products += oneHot1[i] * oneHot2[i];
	}
	for (size_t i = 0; i < oneHot1.size(); i++)
	{
		modular1 += pow(oneHot1[i], 2);
	}
	modular1 = pow(modular1, 0.5);
	for (size_t i = 0; i < oneHot2.size(); i++)
	{
		modular2 += pow(oneHot2[i], 2);
	}
	modular2 = pow(modular2, 0.5);
	return products / (modular1*modular2);
}

double TextSimilarity::getTextSimilarity(const char* file1, const char* file2)
{
	TextSimilarity::wordFreq wf = getWordFreq(file1);
	TextSimilarity::wordFreq wf2 = getWordFreq(file2);
	vector<pair<string, int>> wfvec = sortByValueReverse(wf);
	vector<pair<string, int>> wfvec2 = sortByValueReverse(wf2);
	cout << "wfvec:" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << UTF8ToGBK(wfvec[i].first) << ":" << wfvec[i].second;
	}
	cout << endl;
	cout << "wfvec2:" << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << UTF8ToGBK(wfvec2[i].first) << ":" << wfvec2[i].second;
	}
	cout << endl;
	TextSimilarity::wordSet wset;
	selectAimWords(wfvec, wset);
	selectAimWords(wfvec2, wset);
	cout << "wset:" << endl;
	for (const auto& e : wset)
	{
		cout << UTF8ToGBK(e) << " ";
	}
	cout << endl;
	vector<double> oneHot1 = getOneHot(wset, wf);
	cout << "oneHot1:" << endl;
	for (const auto& e : oneHot1)
	{
		cout << e << "  ";
	}
	cout << endl;
	vector<double> oneHot2 = getOneHot(wset, wf2);
	cout << "oneHot2:" << endl;
	for (const auto& e : oneHot2)
	{
		cout << e << "  ";
	}
	cout << endl;

	cout << "cosine = ";
	return cosine(oneHot1, oneHot2);
}