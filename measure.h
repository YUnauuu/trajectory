#pragma once
#include"data.h"
#include"thread_pool.h"
//��������
class Measure
{
public:
	Measure(string path1, string path2)
	{
		data1.readData(path1);
		data2.readData(path2);
	}
	Data data1, data2;
	virtual void dataProcessing(){}//���ݴ���ŷʽ������Ҫ������ȵĵ�
	virtual void measure() {}//ʵ�ֶ�������
};
