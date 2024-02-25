#include<iostream>
#include"data.h"
#include"measure.h"
#include "xthread_pool.h"
#include"euclidean.h"

XThreadPool pool;


int main()
{
	/*string trajectoryPath = ".//data//11291991.gpx";
	Data trajectoryData(trajectoryPath);
	trajectoryData.readData();*/
	//使用欧式距离计算相似度
	//Euclidean my(".//data//11291991.gpx", ".//data//11291991副本.gpx");
	pool.Init(16);
	pool.Start();
	Data da;
	da.readData(".//data//11291991.gpx", ".//data//11291991副本.gpx");


	return 0;
}