#include "TextSimilarity.h"
#include <iostream>

using namespace std;

//void Test()
//{
//	TextSimilarity ts("dict");
//	TextSimilarity::wordFreq wf = ts.getWordFreq("test1.txt");
//	TextSimilarity::wordFreq wf2 = ts.getWordFreq("test2.txt");
//	//for (const auto& e : wf)
//	//{
//	//	cout << ts.UTF8ToGBK(e.first) << ":" << e.second << "    ";
//	//}
//	//cout << endl;
//	vector<pair<string, int>> wfvec = ts.sortByValueReverse(wf);
//	vector<pair<string, int>> wfvec2 = ts.sortByValueReverse(wf2);
//	cout << "wfvec:" << endl;
//	for (int i = 0; i < 10; i++)
//	{
//		cout << ts.UTF8ToGBK(wfvec[i].first)<< ":" << wfvec[i].second << " ";
//	}
//	cout << endl;
//	cout << "wfvec2:" << endl;
//	for (int i = 0; i < 10; i++)
//	{
//		cout << ts.UTF8ToGBK(wfvec2[i].first)<< ":" << wfvec2[i].second << " ";
//	}
//	cout << endl;
//	TextSimilarity::wordSet wset;
//	ts.selectAimWords(wfvec, wset);
//	ts.selectAimWords(wfvec2, wset);
//	cout << "wset:" << endl;
//	for (const auto& e : wset)
//	{
//		cout << ts.UTF8ToGBK(e) << " ";
//	}
//	cout << endl;
//	vector<double> oneHot1 = ts.getOneHot(wset, wf);
//	cout << "oneHot1:" << endl;
//	for (const auto& e : oneHot1)
//	{
//		cout << e << "  ";
//	}
//	cout << endl;
//	vector<double> oneHot2 = ts.getOneHot(wset, wf2);
//	cout << "oneHot2:" << endl;
//	for (const auto& e : oneHot2)
//	{
//		cout << e << "  ";
//	}
//	cout << endl;
//
//	cout << "cosine = " << ts.cosine(oneHot1, oneHot2) << endl;
//}

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		cout << "Usage" << argv[0] << "<dict> <file1> <file2>";
		return 1;
	}

	TextSimilarity ts(argv[1]);
	cout << ts.getTextSimilarity(argv[2], argv[3]) << endl;

	//Test();

	system("pause");
	return 0;
}