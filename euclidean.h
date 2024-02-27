#pragma once
#include<iostream>
#include"xtask.h"

//欧几里得距离度量方法
static std::mutex mux_;
class Euclidean : public XTask
{
public:
	Euclidean(){}
	Euclidean(std::vector<std::tuple<double, double, double, std::string>> vec_a, std::vector<std::tuple<double, double, double, std::string>> vec_b)
	{
		for (int i = 0; i < vec_a.size(); ++i)
		{
			this->vec_a.emplace_back(vec_a[i]);
			this->vec_b.emplace_back(vec_b[i]);
		}
	}
	int compute() override;
	std::vector<std::tuple<double, double, double, std::string>> vec_a;
	std::vector<std::tuple<double, double, double, std::string>> vec_b;
};
