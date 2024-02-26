#pragma once
#include"tinyxml2.h"
#include<string>
#include<vector>
#include<tuple>

//轨迹数据
class Data
{
public:
	Data(){}
	Data(std::string p) : path(p) {}
	Data(std::string p, std::string p2) : path(p), path2(p2){}
	
	//我用 double类型存储维度lat 、经度lon 和海拔ele，时间数据暂时用string存
	std::vector<std::tuple<double, double, double, std::string>> data;

	//读取数据
	void readData(std::string p, std::string p2);

	//轨迹压缩-基于路网压缩，未实现
	void trajectoryCompression();
private:
	std::string path;
	std::string path2;
};

