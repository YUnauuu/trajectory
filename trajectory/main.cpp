#include<iostream>
#include"data.h"

int main()
{
	string trajectoryPath = ".//data//11291991.gpx";
	Data trajectoryData(trajectoryPath);
	if (!trajectoryData.readData())
	{
		cout << "FIle read error!" << endl;
		return -1;
	}

}