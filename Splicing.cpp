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


int Splicing(string localpath, string remotepath)          //ƴ�Ӻ���
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
	int TotalLen1 = ftell(file1);                   //��ȡ�����ļ�����
	int TotalLen2 = ftell(file2);                   //��ȡ�������ļ�����

	int ZeroLen = TotalLen2 - 1024;                 //��ȡ�����ļ��У�����ռλ����ĳ���
	int RealLen1 = TotalLen1 - 1024 - ZeroLen;      //��ȡ�����ļ��У���Ҫƴ�ӵ��������ļ��ϵ����ݳ���
	int SingleLen = RealLen1 / 10;
	int RestLen = RealLen1 % 10 + SingleLen;

	unsigned char* const data1 = (unsigned char*)malloc(sizeof(unsigned char)*SingleLen);
	unsigned char* const data2 = (unsigned char*)malloc(sizeof(unsigned char)*RestLen);

	fseek(file1, int(ZeroLen + 1024), SEEK_SET);
	for (int i = 0; i < 9; i++)
	{
		fread(data1, sizeof(unsigned char), SingleLen, file1);                //��Ҫƴ�ӵ����ݶ���data1������
		fwrite(data1, sizeof(unsigned char), SingleLen, file2);             //��data1�д��������ƴ�ӵ��������ļ���
	}

	fread(data2, sizeof(unsigned char), RestLen, file1);                //�����µ�Ҫƴ�ӵ����ݶ���data2������
	fwrite(data2, sizeof(unsigned char), RestLen, file2);             //��data2�д��������ƴ�ӵ��������ļ���

	fclose(file1);
	fclose(file2);

	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	//cout << "cost time: " << duration << "s" << endl;

	return duration;
}





