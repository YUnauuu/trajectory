#pragma once
#include<iostream>
#include"method.h"


//欧几里得距离度量方法
class Euclidean : public Method
{
public:
	Euclidean(){}
	Euclidean(int ran);
	//Euclidean(std::vector<std::tuple<double, double, double, std::string>> vec_a, std::vector<std::tuple<double, double, double, std::string>> vec_b)
	/*Euclidean()
	{
		for (int i = 0; i < data1.GetData().size(); ++i)
		{
			this->vec_a.emplace_back(data1.GetData()[0]);
			this->vec_b.emplace_back(vec_b[i]);
		}
	}*/
	//std::vector<double> result_vector;//保存计算结果， 只有与 rank 后面的数据 的计算结果
	Data data;//一个 Euclidean 对象存储一个数据(轨迹)
	int rank;//这个对象的 data 在data_queue 中的排位
	//void compute(int rank);//计算data_vector 中第rank位的数据
	void compute(int i);//与 data2 计算距离
};
