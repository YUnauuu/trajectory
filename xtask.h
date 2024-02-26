#pragma once
#include<functional>
#include<future>
#include<string>
#include<mutex>
#include"data.h"

extern std::vector<double> result;
static std::mutex mux_;
//�������
class XTask
{
public:

	//���ݴ���ŷʽ������Ҫ������ȵĵ�
	//virtual void dataProcessing() = 0;

	//ʵ�м���
	virtual int compute() = 0;

	std::function<bool()> is_exit = nullptr;
	auto GetResult()
	{
		//�����ȴ� set_value
		return p_.get_future().get();
	}
	void SetValue(double v)
	{
		p_.set_value(v);
		std::unique_lock<std::mutex> lock(mux_);
		result.emplace_back(v);
	}
	
private:
	//������ֵ
	std::promise<double> p_;
};