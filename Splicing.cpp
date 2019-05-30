#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include "stdlib.h"
#include "time.h"
#include "Splicing.h"
#include "windows.h"
#include <thread>
using namespace std;


int Splicing(string localpath, string remotepath)          //拼接函数
{

	clock_t start, finish;
	double  duration;
	start = clock();


	FILE *file1;
	FILE *file2;
	file1 = fopen(localpath.c_str(), "rb+");
	file2 = fopen(remotepath.c_str(), "rb+");

	if (file1 == NULL)
	{
		cout << "Error: Cannot open the local file to read!" << endl;
		return -1;
	}

	if (file2 == NULL)
	{
		cout << "Error: Cannot open the remote file to read!" << endl;
		return -1;
	}

	fseek(file1, 0, SEEK_END);
	fseek(file2, 0, SEEK_END);
	int TotalLen1 = ftell(file1);                   //获取本地文件长度
	int TotalLen2 = ftell(file2);                   //获取服务器文件长度

	int ZeroLen = TotalLen2 - 1024;                 //获取本地文件中，用于占位的零的长度
	int RealLen1 = TotalLen1 - 1024 - ZeroLen;      //获取本地文件中，需要拼接到服务器文件上的数据长度
	int SingleLen = RealLen1 / 10;
	int RestLen = RealLen1 % 10 + SingleLen;

	unsigned char* const data1 = (unsigned char*)malloc(sizeof(unsigned char)*SingleLen);
	unsigned char* const data2 = (unsigned char*)malloc(sizeof(unsigned char)*RestLen);

	fseek(file1, int(ZeroLen + 1024), SEEK_SET);
	for (int i = 0; i < 9; i++)
	{
		fread(data1, sizeof(unsigned char), SingleLen, file1);                //将要拼接的数据读入data1数组中
		fwrite(data1, sizeof(unsigned char), SingleLen, file2);             //将data1中储存的数据拼接到服务器文件上
	}

	fread(data2, sizeof(unsigned char), RestLen, file1);                //将余下的要拼接的数据读入data2数组中
	fwrite(data2, sizeof(unsigned char), RestLen, file2);             //将data2中储存的数据拼接到服务器文件上

	fclose(file1);
	fclose(file2);

	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	//cout << "cost time: " << duration << "s" << endl;

	return duration;
}





