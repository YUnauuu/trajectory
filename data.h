﻿#pragma once
#include"tinyxml2.h"
#include<string>
#include<vector>

using namespace std;
using namespace tinyxml2;
//轨迹数据处理
class Data
{
public:
	Data(){}
	Data(string p) : path(p) {}
	
	vector<vector<double>> data; //我用 double类型存储维度lat 、经度lon 和海拔ele，时间数据还没存，不知道用long int还是string
	void readData();//读取数据
	void readData(string p);//读取数据
	void trajectoryCompression();//轨迹压缩-基于路网压缩，未实现
private:
	string path;

};

void Data::readData()
{
	XMLDocument doc;
	if (doc.LoadFile(path.c_str()) != XML_SUCCESS)
	{
		cout << "File read error!" << endl;
		exit(-1);
	}

	XMLElement* rootNode = doc.RootElement();
	if (!rootNode)
	{
		cout << "File read error!" << endl;
		exit(-1);
	}

	XMLElement* subNode = rootNode->FirstChildElement();
	subNode = subNode->NextSiblingElement();
	XMLElement* subsubNode = subNode->FirstChildElement();
	subsubNode = subsubNode->NextSiblingElement();

	XMLElement* trajNode = subsubNode->FirstChildElement();
	vector<double> tempVec;
	int i = 0;
	XMLElement* subtrajNode;
	while (trajNode)
	{

		string str = trajNode->Attribute("lat");
		tempVec.clear();
		tempVec.emplace_back(stod(str));
		str = trajNode->Attribute("lon");
		tempVec.emplace_back(stod(str));
		subtrajNode = trajNode->FirstChildElement();
		str = subtrajNode->GetText();
		tempVec.emplace_back(stod(str));
		subtrajNode = subtrajNode->NextSiblingElement();
		str = subtrajNode->GetText();
		//tempVec.emplace_back(str);//时间如 2024-02-21T14:36:52Z

		data.emplace_back(tempVec);
		trajNode = trajNode->NextSiblingElement();
	}
}
void Data::readData(string p)
{
	path = p;
	readData();
}
void Data::trajectoryCompression()
{

}