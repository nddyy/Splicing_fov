#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <direct.h>
#include "Splicing.h"
#include <io.h>
#include <iostream>
#include <direct.h>
#include <algorithm>
#include <vector>
using namespace std;

vector<string> getFiles(string cate_dir)
{
	vector<string> files;//存放文件名

	_finddata_t file;
	long long lf;
	//输入文件夹路径
	if ((lf = _findfirst(cate_dir.c_str(), &file)) == -1) {
		cout << "Error: " << cate_dir << " not found!" << endl;
		cout << "Please pay attention to your input path should be similar to:'E:\TEST'  instead of:'E:\TEST\*.cal'" << endl;
	}
	else {
		cout << "Correct input!" << endl;
		cout << "Document retrievaling..." << endl << "...  ...  ..." << endl;
		files.push_back(file.name);
		while (_findnext(lf, &file) == 0) {
			//输出文件名
			//cout<<file.name<<endl;
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			files.push_back(file.name);
		}
	}
	_findclose(lf);
	sort(files.begin(), files.end());

	return files;
}
