#include<iostream>
#include"data.h"
#include"measure.h"
#include"thread_pool.h"
#include"euclidean.h"

int main()
{
	/*string trajectoryPath = ".//data//11291991.gpx";
	Data trajectoryData(trajectoryPath);
	trajectoryData.readData();*/
	//ʹ��ŷʽ����������ƶ�
	Euclidean my(".//data//11291991.gpx", ".//data//11291991����.gpx");
	
	return 0;
}