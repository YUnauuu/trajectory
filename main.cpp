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
	//ʹ��ŷʽ����������ƶ�
	//Euclidean my(".//data//11291991.gpx", ".//data//11291991����.gpx");
	pool.Init(16);
	pool.Start();
	Data da;
	da.readData(".//data//11291991.gpx", ".//data//11291991����.gpx");


	return 0;
}