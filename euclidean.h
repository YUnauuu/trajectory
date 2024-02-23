#pragma once
#include<iostream>
#include"measure.h"

//欧几里得距离度量方法
class Euclidean : public Measure
{
public:
	Euclidean(string path1, string path2) : Measure(path1, path2){}
	void measure() override;
};

void Euclidean::measure()
{
	if (data1.data.size() != data2.data.size())
	{
		cout << "data error!" << endl;
		exit(-2);
	}

}