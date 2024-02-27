#pragma once
#include<functional>
#include<future>
#include<string>
#include<mutex>
#include"data.h"

extern double result;

//任务基类
class XTask
{
public:

	//数据处理，欧式距离需要数量相等的点
	//virtual void dataProcessing() = 0;

	//实行计算
	//virtual int Run() = 0;
	virtual int compute() = 0;

	std::function<bool()> is_exit = nullptr;
	auto GetResult()
	{
		//阻塞等待 set_value
		return p_.get_future().get();
	}
	void SetValue(double v)
	{
		p_.set_value(v);
	}
	
private:
	//计算结果值
	std::promise<double> p_;
};