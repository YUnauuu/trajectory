#include<iostream>
#include"data.h"
#include "xthread_pool.h"
#include"euclidean.h"

XThreadPool pool;//线程池
double result = 0.0;//结果数组

int main()
{
	/*string trajectoryPath = ".//data//11291991.gpx";
	Data trajectoryData(trajectoryPath);
	trajectoryData.readData();*/
	//使用欧式距离计算相似度
	//Euclidean my(".//data//11291991.gpx", ".//data//11291991副本.gpx");
	pool.Init(16);
	Data da;
	da.readData(".//data//11291991.gpx", ".//data//11291991副本.gpx");


	std::cout << "result = " << result << std::endl;
	return 0;
}
