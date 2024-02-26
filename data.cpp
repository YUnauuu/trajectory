#pragma once
#include"data.h"
#include<iostream>
#include<functional>
#include"xthread_pool.h"
#include"euclidean.h"

using namespace tinyxml2;
using namespace std;

extern XThreadPool pool;;

void Data::readData(string path1, string path2)
{
	XMLElement* trajNode1;
	XMLElement* trajNode2;
	
	XMLDocument doc;
	if (doc.LoadFile(path1.c_str()) != XML_SUCCESS)
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
	//跳过无效字段
	XMLElement* subNode = rootNode->FirstChildElement();
	subNode = subNode->NextSiblingElement();
	XMLElement* subsubNode = subNode->FirstChildElement();
	subsubNode = subsubNode->NextSiblingElement();
	trajNode1 = subsubNode->FirstChildElement();
	
	
	XMLDocument doc2;
	if (doc2.LoadFile(path2.c_str()) != XML_SUCCESS)
	{
		cout << "File read error!" << endl;
		exit(-1);
	}

	rootNode = doc2.RootElement();
	if (!rootNode)
	{
		cout << "File read error!" << endl;
		exit(-1);
	}
	//跳过无效字段
	subNode = rootNode->FirstChildElement();
	subNode = subNode->NextSiblingElement();

	subsubNode = subNode->FirstChildElement();
	subsubNode = subsubNode->NextSiblingElement();
	trajNode2 = subsubNode->FirstChildElement();

	//从下面开始
	tuple<double, double, double, string> tuple_a, tuple_b;

	vector<tuple<double, double, double, string>> vec_a, vec_b;
	int count = 0;
	const int unitNum = 1000;//每组的个数
	XMLElement* subtrajNode1, * subtrajNode2;
	while (trajNode1 && trajNode2)
	{
		//记录第一个路径path1下的数据
		string str = trajNode1->Attribute("lat");
		get<0>(tuple_a) = stod(str);//维度lat
		str = trajNode1->Attribute("lon");
		get<1>(tuple_a) = stod(str);//经度lon
		subtrajNode1 = trajNode1->FirstChildElement();
		str = subtrajNode1->GetText();
		get<2>(tuple_a) = stod(str);//海拔ele
		subtrajNode1 = subtrajNode1->NextSiblingElement();
		str = subtrajNode1->GetText();
		get<3>(tuple_a) = str;//时间

		vec_a.emplace_back(tuple_a);
		trajNode1 = trajNode1->NextSiblingElement();
		 
		 
		//记录第二个路径path2下的数据
		str = trajNode2->Attribute("lat");
		get<0>(tuple_b) = stod(str);//维度lat
		str = trajNode2->Attribute("lon");
		get<1>(tuple_b) = stod(str);//经度lon
		subtrajNode2 = trajNode2->FirstChildElement();
		str = subtrajNode2->GetText();
		get<2>(tuple_b) = stod(str);//海拔ele
		subtrajNode2 = subtrajNode2->NextSiblingElement();
		str = subtrajNode2->GetText();
		get<3>(tuple_b) = str;//时间

		vec_b.emplace_back(tuple_b);
		trajNode2 = trajNode2->NextSiblingElement();
		++count;
		if (count > unitNum)
		{
			count = 0;
			//以 unitNum 个数据分组，开启线程计算
			auto task = make_shared<Euclidean>(vec_a, vec_b);
			pool.AddTask(task);
			vec_a.clear();
			vec_b.clear();
		}
	}
	auto task = make_shared<Euclidean>(vec_a, vec_b);

	pool.AddTask(task);
	vec_a.clear();
	vec_b.clear();
	
}
void Data::trajectoryCompression()
{

}