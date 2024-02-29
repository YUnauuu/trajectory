#pragma once
#include<functional>
#include<future>
#include<string>
#include<mutex>
#include"data.h"

extern double result;
extern std::vector<Data> data_vector;

//计算方法基类
class Method
{
public:

	//数据处理，欧式距离需要数量相等的点
	//virtual void dataProcessing() = 0;

	//实行计算
	virtual void compute() {};

	std::function<bool()> is_exit = nullptr;
	auto GetResult()
	{
		//阻塞等待 set_value
		return result_.get_future().get();
	}
	void SetValue(double v)
	{
		result_.set_value(v);
	}
	
private:
	//计算结果值
	std::promise<double> result_;
};