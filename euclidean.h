#pragma once
#include<iostream>
#include"measure.h"

//ŷ����þ����������
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
	if (data1.data.size() != data2.data.size())//���ݸ�����Ҫ���
	{
		cout << "���ݸ�������ȣ�" << endl;
		exit(-2);
	}

	ThreadPool pool(4);//���� 4 ���߳�
	const unsigned int NUM = 1000;
	int n = (data1.data.size() + NUM - 1) / NUM;//���ݷָ� 1000��Ϊһ�飬�� n ��,һ���̼߳���һ��
	vector<double> distance;//�ݴ� n ��ļ�����
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