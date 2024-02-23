#pragma once
#include<iostream>
#include"measure.h"

//欧几里得距离度量方法
class Euclidean : public Measure
{
public:
	Euclidean(string path1, string path2) : Measure(path1, path2) 
	{
		measure();
	}
	void measure() override;
};

void Euclidean::measure()
{
	if (data1.data.size() != data2.data.size())//数据个数需要相等
	{
		cout << "数据个数不相等！" << endl;
		exit(-2);
	}

	ThreadPool pool(4);//开启 4 个线程
	const unsigned int NUM = 1000;
	int n = (data1.data.size() + NUM - 1) / NUM;//数据分割 1000个为一组，共 n 组,一个线程计算一组
	vector<double> distance;//暂存 n 组的计算结果
	distance.resize(n);
	for (int i = 0; i < n; ++i)
	{
		pool.enqueue([&] {
			double sumTemp = 0.0;
			for (int j = i * NUM; j < data1.data.size() && j < (i + 1) * NUM; ++j)
			{
				sumTemp += (data1.data[j][0] - data2.data[j][0]) * (data1.data[j][0] - data2.data[j][0]) + (data1.data[j][1] - data2.data[j][1]) * (data1.data[j][1] - data2.data[j][1]);
			}
			distance[i] = sumTemp;
			
			});
	}
	double allSum = 0.0;
	for (double t : distance)
	{
		allSum += t;
	}
	distance.clear();
	cout << allSum << endl;
}