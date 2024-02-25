#pragma once
#include"data.h"
#include<iostream>
#include<functional>
#include"xthread_pool.h"
#include"euclidean.h"

using namespace tinyxml2;
using namespace std;

extern XThreadPool pool;;
//void Data::readData()
//{
//	XMLDocument doc;
//	if (doc.LoadFile(path.c_str()) != XML_SUCCESS)
//	{
//		cout << "File read error!" << endl;
//		exit(-1);
//	}
//
//	XMLElement* rootNode = doc.RootElement();
//	if (!rootNode)
//	{
//		cout << "File read error!" << endl;
//		exit(-1);
//	}
//	//跳过无效字段
//	XMLElement* subNode = rootNode->FirstChildElement();
//	subNode = subNode->NextSiblingElement();
//	XMLElement* subsubNode = subNode->FirstChildElement();
//	subsubNode = subsubNode->NextSiblingElement();
//
//	//从下面开始
//	XMLElement* trajNode = subsubNode->FirstChildElement();
//	vector<double> tempVec;
//	int i = 0;
//	XMLElement* subtrajNode;
//	while (trajNode)
//	{
//
//		string str = trajNode->Attribute("lat");
//		tempVec.clear();
//		tempVec.emplace_back(stod(str));
//		str = trajNode->Attribute("lon");
//		tempVec.emplace_back(stod(str));
//		subtrajNode = trajNode->FirstChildElement();
//		str = subtrajNode->GetText();
//		tempVec.emplace_back(stod(str));
//		subtrajNode = subtrajNode->NextSiblingElement();
//		str = subtrajNode->GetText();
//		//tempVec.emplace_back(str);//时间如 2024-02-21T14:36:52Z
//
//		data.emplace_back(tempVec);
//		trajNode = trajNode->NextSiblingElement();
//	}
//}
//void Data::readData(string p)
//{
//	path = p;
//	readData();
//}
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

	XMLElement* rootNode2 = doc2.RootElement();
	if (!rootNode)
	{
		cout << "File read error!" << endl;
		exit(-1);
	}
	//跳过无效字段
	XMLElement* subNode2 = rootNode2->FirstChildElement();
	subNode2 = subNode2->NextSiblingElement();

	XMLElement* subsubNode2 = subNode2->FirstChildElement();
	subsubNode2 = subsubNode2->NextSiblingElement();
	trajNode2 = subsubNode2->FirstChildElement();

	//从下面开始
	vector<double> tvec_a, tvec_b;
	vector<vector<double>> vec_a, vec_b;
	int count = 0;
	const int unitNum = 1000;//每组的个数
	XMLElement* subtrajNode1, * subtrajNode2;
	while (trajNode1 && trajNode2)
	{
		//记录第一个路径path1下的数据
		string str = trajNode1->Attribute("lat");
		
		tvec_a.clear();
		tvec_a.emplace_back(stod(str));
		str = trajNode1->Attribute("lon");
		tvec_a.emplace_back(stod(str));
		subtrajNode1 = trajNode1->FirstChildElement();
		str = subtrajNode1->GetText();
		tvec_a.emplace_back(stod(str));
		subtrajNode1 = subtrajNode1->NextSiblingElement();
		str = subtrajNode1->GetText();
		//tempVec.emplace_back(str);//时间如 2024-02-21T14:36:52Z

		vec_a.emplace_back(tvec_a);
		trajNode1 = trajNode1->NextSiblingElement();
		//cout << tvec_a[0] << ", " << tvec_a[1] << ", " << tvec_a[2] << endl;
		 
		 
		//记录第二个路径path2下的数据
		str = trajNode2->Attribute("lat");
		tvec_b.clear();
		tvec_b.emplace_back(stod(str));
		str = trajNode2->Attribute("lon");
		tvec_b.emplace_back(stod(str));
		subtrajNode2 = trajNode2->FirstChildElement();
		str = subtrajNode2->GetText();
		tvec_b.emplace_back(stod(str));
		subtrajNode2 = subtrajNode2->NextSiblingElement();
		str = subtrajNode2->GetText();
		//tempVec.emplace_back(str);//时间如 2024-02-21T14:36:52Z

		vec_b.emplace_back(tvec_b);
		trajNode2 = trajNode2->NextSiblingElement();
		++count;
		if (count > unitNum)
		{
			//cout << count << endl;
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