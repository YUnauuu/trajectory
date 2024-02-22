#pragma once
#include"tinyxml2.h"
#include<string>
#include<vector>

using namespace std;
using namespace tinyxml2;
//读取数据集，保存在data中
class Data
{
public:
	Data(string p) : path(p) {}
	string path;
	vector<vector<string>> data;
	bool readData();

};
bool Data::readData()
{
	XMLDocument doc;
	if (doc.LoadFile(path.c_str()) != XML_SUCCESS)
		return false;

	XMLElement* rootNode = doc.RootElement();
	if (!rootNode)
		return false;

	XMLElement* subNode = rootNode->FirstChildElement();
	subNode = subNode->NextSiblingElement();
	XMLElement* subsubNode = subNode->FirstChildElement();
	subsubNode = subsubNode->NextSiblingElement();

	XMLElement* trajNode = subsubNode->FirstChildElement();
	vector<string> tempVec;
	int i = 0;
	XMLElement* subtrajNode;
	while (trajNode)
	{

		string str = trajNode->Attribute("lat");
		tempVec.clear();
		tempVec.emplace_back(str);
		str = trajNode->Attribute("lon");
		tempVec.emplace_back(str);
		subtrajNode = trajNode->FirstChildElement();
		str = subtrajNode->GetText();
		tempVec.emplace_back(str);
		subtrajNode = subtrajNode->NextSiblingElement();
		str = subtrajNode->GetText();
		tempVec.emplace_back(str);

		data.emplace_back(tempVec);
		trajNode = trajNode->NextSiblingElement();
	}
	return true;
}