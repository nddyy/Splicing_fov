#include <iostream>
#include <stdio.h>
#include <fstream>
#include "Splicing.h"
#include <string>
#include "stdlib.h"
#include "threadpool.h"
#include "time.h"
#include "windows.h"
#include <thread>
using namespace std;
#ifdef SPLICING

int main() {

	clock_t start, finish;
	double  duration;


	std::thread sp;
	int cpuNumber = sp.hardware_concurrency();                                     //获取cpu个数
	int cpuAllNum;


	char input[200];
	cout << "Please input the command" << endl;
	cout << "The input should be similar to: '-p1 E:\\TEST\\local -p2 E:\\TEST\\remote -c 10'" << endl;
	cout << "'-p1' means local-file's path" << endl;
	cout << "'-p2' means remote-file's path" << endl;
	cout << "'-c' optional parameter: means the occupancy percentage of cpu, this number should be in 5-50, The default is 5" << endl;
	cin.getline(input, 200);
	string str = string(input);
	vector<string> inputDeal = CommandLineDeal(str);                     //对输入的命令行进行处理
	string path1 = inputDeal[0];
	string path2 = inputDeal[1];
	string cpuSet = inputDeal[2];
	int cpuset = std::stoi(cpuSet);

	string n_path1 = path1 + "\\" + "*.cal";
	string n_path2 = path2 + "\\" + "*.cal";

	cout << endl << "Verifying local-file's path..." << endl;
	vector<string> localfiles_name = getFiles((string)n_path1);            //在用户给定的路径下进行文件检索
	cout << "Local-file's retrieval completed!" << endl <<endl;
	cout << endl << "Verifying remote-file's path..." << endl;
	vector<string> remotefiles_name = getFiles((string)n_path2);
	cout << "Remote-file's retrieval completed!" << endl << endl;
	string *localpath = new string[localfiles_name.size()];
	string *remotepath = new string[remotefiles_name.size()];

	if (cpuset == 0) {
		cpuAllNum = cpuNumber * 0.05;                                       //设置cpu利用率默认值
	}
	else
	{
		cpuAllNum = cpuNumber * cpuset / 100;                                               //设置用户给定的cpu利用率
		if (cpuAllNum < 1) {
			cout << "Error: The number of use-cpu that you set is 0!" << endl;
			return -1;
		}
	}


	if (localfiles_name.size() != remotefiles_name.size())
	{
		cout << "Error: The number of local-files and remote-files are not equal!" << endl;
		return -1;
	}
	else
	{
		for (int i = 0; i<localfiles_name.size(); i++)
		{
			if (localfiles_name[i] != remotefiles_name[i])
			{
				cout << "Error: This group-file's name is different!       Group : " << i << endl;
				return -1;
			}
			else
			{
				localpath[i] = path1 + "\\" + localfiles_name[i];
				remotepath[i] = path2 + "\\" + remotefiles_name[i];
			}
		}
	}

	int	ThreadNum = localfiles_name.size();
	
	cout << "Total file: " << ThreadNum << endl;
	cout << "Start splice..." << endl;

	//start = clock();

	try                            //线程池处理拼接函数
	{
		std::threadpool executor{ unsigned short(cpuAllNum) };
		std::vector<std::future<int>> results;

		for (int i = 0; i < ThreadNum; i++)
		{
			results.emplace_back(executor.commit(Splicing, localpath[i], remotepath[i]));
		}
		int i = 1;
		float singlethreadtime = 0;
		float resttime = 0;
		for (auto && result : results) 
		{
			start = clock();
			result.get();
			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;
			if (i % cpuAllNum != 0) 
			{
				singlethreadtime = singlethreadtime + duration;
			}
			else
			{
				singlethreadtime = singlethreadtime + duration;
				resttime = ((ThreadNum-i) / cpuAllNum)*singlethreadtime;
				printf("\r");
				printf("Still need time: %6.2f s",resttime);
				singlethreadtime = 0;
			}
			i++;
		}
		
	}
	catch (std::exception& e) {
		std::cout << "Error: " << std::this_thread::get_id() << e.what() << std::endl;
	}

	cout << endl << "All files finished!" << endl;

	//finish = clock();
	
	system("pause");
	return 0;
}


#endif // SPLICING