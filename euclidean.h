#pragma once
#include<iostream>
#include"xtask.h"

//欧几里得距离度量方法
class Euclidean : public XTask
{
public:
	Euclidean(){}
	Euclidean(std::vector<std::vector<double>> vec_a, std::vector<std::vector<double>> vec_b) 
	{
		for (int i = 0; i < vec_a.size(); ++i)
		{
			this->vec_a.emplace_back(vec_a[i]);
			this->vec_b.emplace_back(vec_b[i]);
		}
	}
	int compute() override;
	std::vector<std::vector<double>> vec_a;
	std::vector<std::vector<double>> vec_b;
};
