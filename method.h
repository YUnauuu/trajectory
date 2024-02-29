#pragma once
#include<functional>
#include<future>
#include<string>
#include<mutex>
#include"data.h"

extern double result;
extern std::vector<Data> data_vector;

//���㷽������
class Method
{
public:

	//���ݴ���ŷʽ������Ҫ������ȵĵ�
	//virtual void dataProcessing() = 0;

	//ʵ�м���
	virtual void compute() {};

	std::function<bool()> is_exit = nullptr;
	auto GetResult()
	{
		//�����ȴ� set_value
		return result_.get_future().get();
	}
	void SetValue(double v)
	{
		result_.set_value(v);
	}
	
private:
	//������ֵ
	std::promise<double> result_;
};