#pragma once
#include<functional>
#include<future>
#include<string>
#include"data.h"

//�������
class XTask
{
public:

	//���ݴ���ŷʽ������Ҫ������ȵĵ�
	//virtual void dataProcessing() = 0;

	//ʵ�м���
	virtual int compute() = 0;

	//��ȡ����
	/*void Read(std::string path1, std::string path2)
	{
		data1.readData(path1);
		data2.readData(path2);
	}*/
	std::function<bool()> is_exit = nullptr;
	auto GetResult()
	{
		//�����ȴ� set_value
		return p_.get_future().get();
	}
	void SetValue(double v)
	{
		p_.set_value(v);
	}
private:
	//������ֵ
	std::promise<double> p_;
};