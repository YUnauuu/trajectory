#pragma once
#include"data.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include"xthread_pool.h"
#include"euclidean.h"

//using namespace tinyxml2;
using namespace std;

//void Data::readData(string path1, string path2)
//{
//	XMLElement* trajNode1;
//	XMLElement* trajNode2;
//	
//	XMLDocument doc;
//	if (doc.LoadFile(path1.c_str()) != XML_SUCCESS)
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
//	//������Ч�ֶ�
//	XMLElement* subNode = rootNode->FirstChildElement();
//	subNode = subNode->NextSiblingElement();
//	XMLElement* subsubNode = subNode->FirstChildElement();
//	subsubNode = subsubNode->NextSiblingElement();
//	trajNode1 = subsubNode->FirstChildElement();
//	
//	
//	XMLDocument doc2;
//	if (doc2.LoadFile(path2.c_str()) != XML_SUCCESS)
//	{
//		cout << "File read error!" << endl;
//		exit(-1);
//	}
//
//	rootNode = doc2.RootElement();
//	if (!rootNode)
//	{
//		cout << "File read error!" << endl;
//		exit(-1);
//	}
//	//������Ч�ֶ�
//	subNode = rootNode->FirstChildElement();
//	subNode = subNode->NextSiblingElement();
//
//	subsubNode = subNode->FirstChildElement();
//	subsubNode = subsubNode->NextSiblingElement();
//	trajNode2 = subsubNode->FirstChildElement();
//
//	//�����濪ʼ
//	tuple<double, double, double, string> tuple_a, tuple_b;
//
//	vector<tuple<double, double, double, string>> vec_a, vec_b;
//	const int unitNum = 1000;//ÿ��ĸ���
//	vec_a.reserve(unitNum);
//	vec_b.reserve(unitNum);
//	int count = 0;
//	XMLElement* subtrajNode1, * subtrajNode2;
//	while (trajNode1 && trajNode2)
//	{
//		//��¼��һ��·��path1�µ�����
//		string str = trajNode1->Attribute("lat");
//		get<0>(tuple_a) = stod(str);//ά��lat
//		str = trajNode1->Attribute("lon");
//		get<1>(tuple_a) = stod(str);//����lon
//		subtrajNode1 = trajNode1->FirstChildElement();
//		str = subtrajNode1->GetText();
//		get<2>(tuple_a) = stod(str);//����ele
//		subtrajNode1 = subtrajNode1->NextSiblingElement();
//		str = subtrajNode1->GetText();
//		get<3>(tuple_a) = str;//ʱ��
//
//		vec_a.emplace_back(tuple_a);
//		trajNode1 = trajNode1->NextSiblingElement();
//		 
//		 
//		//��¼�ڶ���·��path2�µ�����
//		str = trajNode2->Attribute("lat");
//		get<0>(tuple_b) = stod(str);//ά��lat
//		str = trajNode2->Attribute("lon");
//		get<1>(tuple_b) = stod(str);//����lon
//		subtrajNode2 = trajNode2->FirstChildElement();
//		str = subtrajNode2->GetText();
//		get<2>(tuple_b) = stod(str);//����ele
//		subtrajNode2 = subtrajNode2->NextSiblingElement();
//		str = subtrajNode2->GetText();
//		get<3>(tuple_b) = str;//ʱ��
//
//		vec_b.emplace_back(tuple_b);
//		trajNode2 = trajNode2->NextSiblingElement();
//		++count;
//		if (count > unitNum)
//		{
//			count = 0;
//			//�� unitNum �����ݷ��飬�����̼߳���
//			auto task = make_shared<Euclidean>(vec_a, vec_b);
//			pool.AddTask(task);
//			vec_a.clear();
//			vec_b.clear();
//		}
//	}
//	auto task = make_shared<Euclidean>(vec_a, vec_b);
//
//	pool.AddTask(task);
//	vec_a.clear();
//	vec_b.clear();
//	
//}

//��path·����ȡ���ݲ��浽data
void Data::ReadData(std::string path)
{
	this->path = path;
	ifstream file(path);
	if (!file.is_open())//���·����Ч
	{
		cout << "no file: " << path << endl;
		return;
	}
	std::array<double, 2> temp_arr;
	string line_str;
	string lat_str;
	stringstream ss;
	while (getline(file, line_str))//ֱ�������ļ�
	{
		lat_str = R"( lat=)";
		auto pos = line_str.find(lat_str);
		if (pos != string::npos)
		{
			ss.str("");
			pos += 6;
			while ((line_str[pos] <= '9' && line_str[pos] >= '0') || line_str[pos] == '.' || line_str[pos] == '-')
			{
				ss << line_str[pos];
				++pos;
			}
			temp_arr[0] = stod(ss.str());

			ss.str("");
			pos += 7;
			while ((line_str[pos] <= '9' && line_str[pos] >= '0') || line_str[pos] == '.' || line_str[pos] == '-')
			{
				ss << line_str[pos];
				++pos;
			}
			temp_arr[1] = stod(ss.str());
			data.emplace_back(temp_arr);
		}
	}
	file.close();
}

//��ȡ����data
std::vector<std::array<double, 2>> Data::GetData()
{
	return data;
}
//��ȡ�ļ���
std::string Data::GetName()
{
	std::string name = path.substr(7);
	return name;
}
void Data::trajectoryCompression()
{
	
}