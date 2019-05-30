#include<regex>
#include<iostream>
#include<string>
#include<string.h>
#include "Splicing.h"
using namespace std;



vector<string> CommandLineDeal(string input)
{
	string p1("-p1\\s\\S*");
	string p2("-p2\\s\\S*");
	string c("-c\\s\\S*");
	string path1, path2, cpuSet;
	vector<string> inputDeal;
	regex pattern1(p1);
	regex pattern2(p2);
	regex pattern3(c);
	smatch result;
	string str = input;

	if (regex_search(str, result, pattern1)) {
		string s = result[0];
		path1 = s.substr(4);
		inputDeal.push_back(path1);
	}
	else
	{
		cout << "Error: Your local-file path is wrong!" << endl;
		cout << "Please pay attention to your input path should be similar to:'E:\TEST'  instead of:'E:\TEST\*.cal'" << endl;
	}

	if (regex_search(str, result, pattern2)) {
		string s = result[0];
		path2 = s.substr(4);
		inputDeal.push_back(path2);
	}
	else
	{
		cout << "Error: Your remote-file path is wrong!" << endl;
		cout << "Please pay attention to your input path should be similar to:'E:\TEST'  instead of:'E:\TEST\*.cal'" << endl;
	}

	if (regex_search(str, result, pattern3)) {
		string s = result[0];
		cpuSet = s.substr(3);
		inputDeal.push_back(cpuSet);
	}
	else
	{
		cpuSet = "0";
		inputDeal.push_back(cpuSet);
	}

	return inputDeal;
}
