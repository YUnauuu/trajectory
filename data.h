#pragma once
//#include"tinyxml2.h"
#include<string>
#include<vector>
#include<array>

//轨迹数据
class Data
{
public:
	Data(){}

	//从path路径读取数据并存到data
	void ReadData(std::string path);
	//void readData(std::string path1, std::string path2);

	//轨迹压缩-基于路网压缩，未实现
	void trajectoryCompression();

	//获取数据
	std::vector<std::array<double, 2>> GetData();
	//获取文件名
	std::string GetName();
private:
	std::vector<std::array<double, 2>> data;
	std::string path;
	std::vector<double> distance;//保存与其他数据集间的距离，顺序与queue<Data> data_queue 一致
};

