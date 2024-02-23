#include<iostream>
#include"data.h"
#include"measure.h"
#include"thread_pool.h"

int main()
{
	string trajectoryPath = ".//data//11291991.gpx";
	Data trajectoryData(trajectoryPath);
	trajectoryData.readData();
	return 0;
}