#pragma once
#include<iostream>
#include"method.h"


//ŷ����þ����������
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
	//std::vector<double> result_vector;//����������� ֻ���� rank ��������� �ļ�����
	Data data;//һ�� Euclidean ����洢һ������(�켣)
	int rank;//�������� data ��data_queue �е���λ
	//void compute(int rank);//����data_vector �е�rankλ������
	void compute(int i);//�� data2 �������
};
