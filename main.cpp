#include<iostream>
#include"data.h"
#include "xthread_pool.h"
#include"euclidean.h"

XThreadPool pool;//�̳߳�
double result = 0.0;//�������

int main()
{
	/*string trajectoryPath = ".//data//11291991.gpx";
	Data trajectoryData(trajectoryPath);
	trajectoryData.readData();*/
	//ʹ��ŷʽ����������ƶ�
	//Euclidean my(".//data//11291991.gpx", ".//data//11291991����.gpx");
	pool.Init(16);
	Data da;
	da.readData(".//data//11291991.gpx", ".//data//11291991����.gpx");


	std::cout << "result = " << result << std::endl;
	return 0;
}
