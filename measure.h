#pragma once
#include"data.h"
#include"thread_pool.h"
//度量方法
class Measure
{
public:
	Measure(string path1, string path2)
	{
		data1.readData(path1);
		data2.readData(path2);
	}
	Data data1, data2;
	virtual void dataProcessing(){}//数据处理，欧式距离需要数量相等的点
	virtual void measure() {}//实现度量方法
};
