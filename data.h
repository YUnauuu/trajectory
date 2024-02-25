#pragma once
#include"tinyxml2.h"
#include<string>
#include<vector>

//轨迹数据
class Data
{
public:
	Data(){}
	Data(std::string p) : path(p) {}
	Data(std::string p, std::string p2) : path(p), path2(p2){}
	
	std::vector<std::vector<double>> data; //我用 double类型存储维度lat 、经度lon 和海拔ele，时间数据还没存，不知道用long int还是string
	//void readData();//读取数据
	//void readData(std::string p);//读取数据
	void readData(std::string p, std::string p2);
	void trajectoryCompression();//轨迹压缩-基于路网压缩，未实现
private:
	std::string path;
	std::string path2;
};

