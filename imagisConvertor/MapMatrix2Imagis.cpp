#include "stdafx.h"
#include "MapMatrix2Imagis.h"
#include <string>
#include <deque>
#include <regex>
#include <map>
#include "tinyxml2.h"
#include "tinyxml2.cpp"
#include <sstream>


using namespace std;

template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

deque<string> split(string str, string s)
{
	regex reg(s.c_str());
	deque<string> de;
	sregex_token_iterator it(str.begin(), str.end(), reg, -1);
	sregex_token_iterator end;

	while (it != end)
	{
		de.push_back(*it++);
	}


	if (de.size() > 0 && de[0] == "")
	{
		de.pop_front();
	}

	return de;
}


MapMatrix2Imagis::MapMatrix2Imagis()
{
}


MapMatrix2Imagis::~MapMatrix2Imagis()
{
}

bool MapMatrix2Imagis::convert(string xmlFileName, string outFileName)
{
	tinyxml2::XMLDocument oriDoc;

	tinyxml2::XMLError errXml = oriDoc.LoadFile(xmlFileName.c_str());

	if (tinyxml2::XML_SUCCESS != errXml)
	{
		return false;
	}

	tinyxml2::XMLElement* head = oriDoc.RootElement();

	if (!head)
	{
		return false;
	}

	tinyxml2::XMLElement *layer = head->FirstChildElement("Layer");
	vector<shared_ptr<CBaseElement>> vecBaseElement;

	while (layer)
	{
		tinyxml2::XMLElement *layerName = layer->FirstChildElement("LayerName");

		if (layerName)
		{
			tinyxml2::XMLElement *featureList = layer->FirstChildElement("FeatureList");

			if (featureList)
			{
				tinyxml2::XMLElement *feature = featureList->FirstChildElement("Feature");

				while (feature)
				{
					tinyxml2::XMLElement *classId = feature->FirstChildElement("CLASSID");
					tinyxml2::XMLElement *verts = feature->FirstChildElement("VERTEXES");

					if (verts && classId)
					{
						string strVerts = verts->GetText();
						string strClassId = classId->GetText();

						shared_ptr<CBaseElement> elem;
						bool bFlag = parse(strVerts, strClassId, elem);

						if (bFlag)
						{
							vecBaseElement.push_back(elem);
						}

					}

					feature = feature->NextSiblingElement();
				}
			}
		}


		layer = layer->NextSiblingElement();
	}

	outputImagis(vecBaseElement, outFileName);
	return true;
}

bool MapMatrix2Imagis::outputImagis(vector<shared_ptr<CBaseElement>> vecBaseElement, string outFileName)
{
	ofstream out(outFileName);
	out << "IMAGIS3D2.0" << endl;
	out << "           0" << endl;
	out << "Meter" << endl;
	out << "#\\/#" << endl;
	out << "             0.0             0.0             0.0             0.0             0.0             0.0"<< endl;
	out << "       1" << endl;
	out << "自由层" << endl;
	out << "           0" << endl;
	out << "           " << vecBaseElement.size() << endl;
	out << "           0" << endl;
	out << "           0" << endl;
	out << "           0" << endl;
	out << "           0" << endl;
	
	int id = 0;

	for (auto baseElem : vecBaseElement)
	{
		id++;
		shared_ptr<CPrism> prism = dynamic_pointer_cast<CPrism> (baseElem);

		if (prism)
		{
			outPrism(prism, out, id);
			continue;
		}

		shared_ptr<CHumanTop> humanTop = dynamic_pointer_cast<CHumanTop>(baseElem);

		if (humanTop)
		{
			outHumanTop(humanTop, out, id);
			continue;
		}

		shared_ptr<CSteeple> steeple = dynamic_pointer_cast<CSteeple>(baseElem);

		if (steeple)
		{
			outSteeple(steeple, out, id);
			continue;
		}

		shared_ptr<CCylinder> cylinder = dynamic_pointer_cast<CCylinder>(baseElem);

		if (cylinder)
		{
			outCylinder(cylinder, out, id);
			continue;
		}

		shared_ptr<CPolygon> cpolygon = dynamic_pointer_cast<CPolygon>(baseElem);

		if (cpolygon)
		{
			outPolygon(cpolygon, out, id);
			continue;
		}
	}

	out.close();

	return true;
}

bool MapMatrix2Imagis::outPrism(shared_ptr<CPrism> prism, ofstream &out, int id)
{
	auto basePt = prism->basePt;
	auto roof = prism->roof;

	double bottomHeight = basePt.z;	
	auto bottom = roof;

	for (int i = 0; i < bottom.size(); i++)
	{
		bottom[i].z = basePt.z;
	}

	vector<CPt> verts;
	verts.assign(bottom.begin(), bottom.end());
	verts.insert(verts.end(), roof.begin(), roof.end());

	int totalSize = verts.size();

	vector<int> primitive;

	{
		//构建上下两个面
		int bottomSize = bottom.size();
		primitive.push_back(bottomSize);

		//构建底面
		for (int i = 0; i < bottomSize; i++)
		{
			primitive.push_back(i);
		}

		//构建侧面
		for (int i = 0; i < bottomSize - 1; i++)
		{
			primitive.push_back(4);
			primitive.push_back(i);
			primitive.push_back(i + 1);
			primitive.push_back(i + 1 + bottomSize);
			primitive.push_back(i + bottomSize);
		}

		//构建顶部
		int roofSize = roof.size();
		primitive.push_back(roofSize);

		for (int i = 0; i < roofSize; i++)
		{
			primitive.push_back(i + bottomSize);
		}
	}
	

	out <<"           " << totalSize << "             " << to_string(id) << "\t" << "255" << "\t" << "255" << "\t" << "255" << "\t" << "0" << "\t" << "1.0" << "\t" <<
		primitive.size() << "\t" << "0" << "\t" << "0" << "\t" << "0" << "\t" << "255" << "\t" << "2" << "\t" << "0" << "\t" << "1" << 
		"\t" << "0" << "\t" << "1" << "\t" << "0" << "\t" << "1" << "\t" << "0" << "\t" << "" << endl;

	out << "\t\t\t";

	for (auto index : primitive)
	{
		out << index << "\t";
	}

	out << endl;

	for (auto vert : verts)
	{
		out << "\t\t\t   " << vert.x << "\t\t" << vert.y << "\t\t" << vert.z << endl;
	}

}

bool MapMatrix2Imagis::outHumanTop(shared_ptr<CHumanTop> humanTop, ofstream &out, int id)
{
	auto basePt = humanTop->basePt;
	auto roof = humanTop->roof;
	auto roofTop = humanTop->roofTop;

	double bottomHeight = basePt.z;
	auto bottom = roof;

	for (int i = 0; i < bottom.size(); i++)
	{
		bottom[i].z = basePt.z;
	}

	vector<CPt> verts;
	verts.assign(bottom.begin(), bottom.end());
	verts.insert(verts.end(), roof.begin(), roof.end());
	verts.insert(verts.end(), roofTop.begin(), roofTop.end());

	int totalSize = verts.size();

	vector<int> primitive;

	{
		//构建底面
		int bottomSize = bottom.size();
		primitive.push_back(bottomSize);

		for (int i = 0; i < bottomSize; i++)
		{
			primitive.push_back(i);
		}

		//构建侧面
		for (int i = 0; i < bottomSize - 1; i++)
		{
			primitive.push_back(4);
			primitive.push_back(i);
			primitive.push_back(i + 1);
			primitive.push_back(i + 1 + bottomSize);
			primitive.push_back(i + bottomSize);
		}

		//构建顶部
		int roofSize = roof.size();
		int roofTopSize = roofTop.size();

		for (int i = 0, j = 0; i < roofSize / 3.0; i += 3, j += 1)
		{
			primitive.push_back(4);
			primitive.push_back(i);
			primitive.push_back(i + 1);
			primitive.push_back(i + 2);
			primitive.push_back(j + bottomSize + roofSize);
		}

		for (int i = 0, j = 0; i < roofSize / 3.0 - 1; i += 3, j += 1)
		{
			primitive.push_back(4);
			primitive.push_back(i + 2 + bottomSize);
			primitive.push_back(j + bottomSize + roofSize);
			primitive.push_back(j + 1 + bottomSize + roofSize);
			primitive.push_back(i + 3 + bottomSize);
		}

		primitive.push_back(4);
		primitive.push_back(0 + bottomSize);
		primitive.push_back(bottomSize + roofSize -	1);
		primitive.push_back(bottomSize + roofSize + roofTopSize - 1);
		primitive.push_back(bottomSize + roofSize + roofTopSize - 2);

	}


	out << "\t\t\t" << totalSize << "\t" << to_string(id) << "\t" << "255" << "\t" << "255" << "\t" << "255" << "\t" << "0" << "\t" << "1.0" << "\t" <<
		primitive.size() << "\t" << "0" << "\t" << "0" << "\t" << "0" << "\t" << "255" << "\t" << "2" << "\t" << "0" << "\t" << "1" <<
		"\t" << "0" << "\t" << "1" << "\t" << "0" << "\t" << "1" << "\t" << "0" << "\t" << "" << endl;

	out << "\t\t\t";

	for (auto index : primitive)
	{
		out << index << "\t";
	}

	out << endl;

	for (auto vert : verts)
	{
		out << "\t\t\t   " << vert.x << "\t\t" << vert.y << "\t\t" << vert.z << endl;
	}
}

bool MapMatrix2Imagis::outSteeple(shared_ptr<CSteeple> steeple, ofstream &out, int id)
{
	auto basePt = steeple->basePt;
	auto roof = steeple->roof;
	auto roofTop = steeple->roofTop;

	double bottomHeight = basePt.z;
	auto bottom = roof;

	for (int i = 0; i < bottom.size(); i++)
	{
		bottom[i].z = basePt.z;
	}

	vector<CPt> verts;
	verts.assign(bottom.begin(), bottom.end());
	verts.insert(verts.end(), roof.begin(), roof.end());
	verts.push_back(roofTop);

	int totalSize = verts.size();

	vector<int> primitive;

	{
		//构建底面
		int bottomSize = bottom.size();
		primitive.push_back(bottomSize);

		for (int i = 0; i < bottomSize; i++)
		{
			primitive.push_back(i);
		}

		//构建侧面
		for (int i = 0; i < bottomSize - 1; i++)
		{
			primitive.push_back(4);
			primitive.push_back(i);
			primitive.push_back(i + 1);
			primitive.push_back(i + 1 + bottomSize);
			primitive.push_back(i + bottomSize);
		}

		//构建顶部
		int roofSize = roof.size();
		int roofTopSize = 1;

		for (int i = 0; i < roofSize - 1; i++)
		{
			primitive.push_back(3);
			primitive.push_back(i + bottomSize);
			primitive.push_back(i + bottomSize + 1);
			primitive.push_back(bottomSize + roofSize);
		}

		primitive.push_back(3);
		primitive.push_back(0 + bottomSize);
		primitive.push_back(bottomSize + roofSize - 1);
		primitive.push_back(bottomSize + roofSize);
	}


	out << "\t\t\t" << totalSize << "\t" << to_string(id) << "\t" << "255" << "\t" << "255" << "\t" << "255" << "\t" << "0" << "\t" << "1.0" << "\t" <<
		primitive.size() << "\t" << "0" << "\t" << "0" << "\t" << "0" << "\t" << "255" << "\t" << "2" << "\t" << "0" << "\t" << "1" <<
		"\t" << "0" << "\t" << "1" << "\t" << "0" << "\t" << "1" << "\t" << "0" << "\t" << "" << endl;

	out << "\t\t\t";

	for (auto index : primitive)
	{
		out << index << "\t";
	}

	out << endl;

	for (auto vert : verts)
	{
		out << "\t\t\t   " << vert.x << "\t\t" << vert.y << "\t\t" << vert.z << endl;
	}
}

bool MapMatrix2Imagis::outCylinder(shared_ptr<CCylinder> cylinder, ofstream &out, int id)
{
	auto basePt = cylinder->basePt;
	auto roof1 = cylinder->roof1;
	auto roof2 = cylinder->roof2;
	auto roof3 = cylinder->roof3;

	double r = 0.0;
	double centerX = 0.0;
	double centerY = 0.0;

	if (!makeCircle(roof1, roof2, roof3, centerX, centerY, r))
	{
		return false;
	}

	//制造底部点
	double bottomZ = basePt.z;
	double w = roof1.w;
	vector<CPt> bottom;
	makeCircleLoop(centerX, centerY, bottomZ, r, w, bottom);

	double topZ = roof1.z;
	vector<CPt> roof;
	makeCircleLoop(centerX, centerY, topZ, r, w, roof);

	vector<CPt> verts;
	verts.assign(bottom.begin(), bottom.end());
	verts.insert(verts.end(), roof.begin(), roof.end());

	int totalSize = verts.size();

	vector<int> primitive;

	{
		//构建底面
		int bottomSize = bottom.size();
		primitive.push_back(bottomSize);

		for (int i = 0; i < bottomSize; i++)
		{
			primitive.push_back(i);
		}

		//构建侧面
		for (int i = 0; i < bottomSize - 1; i++)
		{
			primitive.push_back(4);
			primitive.push_back(i);
			primitive.push_back(i + 1);
			primitive.push_back(i + 1 + bottomSize);
			primitive.push_back(i + bottomSize);
		}

		//构建顶部
		int roofSize = roof.size();
		for (int i = 0; i < roofSize; i++)
		{
			primitive.push_back(i + bottomSize);
		}
	}


	out << "\t\t\t" << totalSize << "\t" << to_string(id) << "\t" << "255" << "\t" << "255" << "\t" << "255" << "\t" << "0" << "\t" << "1.0" << "\t" <<
		primitive.size() << "\t" << "0" << "\t" << "0" << "\t" << "0" << "\t" << "255" << "\t" << "2" << "\t" << "0" << "\t" << "1" <<
		"\t" << "0" << "\t" << "1" << "\t" << "0" << "\t" << "1" << "\t" << "0" << "\t" << "" << endl;

	out << "\t\t\t";

	for (auto index : primitive)
	{
		out << index << "\t";
	}

	out << endl;

	for (auto vert : verts)
	{
		out << "\t\t\t   " << vert.x << "\t\t" << vert.y << "\t\t" << vert.z << endl;
	}
}

bool MapMatrix2Imagis::outPolygon(shared_ptr<CPolygon> polygon, ofstream &out, int id)
{
	auto verts = polygon->polygon;
	vector<int> primitive;

	//构建面
	int vertSize = verts.size();

	for (int i = 0; i < verts.size(); i++)
	{
		primitive.push_back(i);
	}

	out << "\t\t\t" << vertSize << "\t" << to_string(id) << "\t" << "255" << "\t" << "255" << "\t" << "255" << "\t" << "0" << "\t" << "1.0" << "\t" <<
		primitive.size() << "\t" << "0" << "\t" << "0" << "\t" << "0" << "\t" << "255" << "\t" << "2" << "\t" << "0" << "\t" << "1" <<
		"\t" << "0" << "\t" << "1" << "\t" << "0" << "\t" << "1" << "\t" << "0" << "\t" << "" << endl;

	out << "\t\t\t";

	for (auto index : primitive)
	{
		out << index << "\t";
	}

	out << endl;

	for (auto vert : verts)
	{
		out << "\t\t\t   " << vert.x << "\t\t" << vert.y << "\t\t" << vert.z << endl;
	}
}

bool MapMatrix2Imagis::makeCircle(CPt pt1, CPt pt2, CPt pt3, double &x, double &y, double &r)
{
	double x1 = -2;
	double y1 = 0;

	double x2 = 6.0;
	double y2 = 0;

	double x3 = 0.0;
	double y3 = -6.0;

	double a = 2 * (x2 - x1);
	double b = 2 * (y2 - y1);
	double c = x2 * x2 + y2 * y2 - x1 * x1 - y1 * y1;
	double d = 2 * (x3 - x2);
	double e = 2 * (y3 - y2);
	double f = x3 * x3 + y3 * y3 - x2 * x2 - y2 * y2;

	if (b * d - e * a == 0)
	{
		return false;
	}

	x = (b * f - e * c) / (b * d - e * a);
	y = (d * c - a * f) / (b * d - e * a);
	r = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));

	return true;
}

bool MapMatrix2Imagis::makeCircleLoop(double x, double y, double z, double r, double w, vector<CPt> &vecCirclePt)
{
	double angle = 0;
	double PI = 3.14159f;

	for (int i = 0; i < 72; i++)
	{
		double circle[72][2] = { 0 };
		circle[i][0] = r * cos(angle) + x;
		circle[i][1] = r * sin(angle) + y;
		angle += 2.0 * PI / 72.0f;
		vecCirclePt.push_back(CPt(circle[i][0], circle[i][1], z, w));
	}

	return true;
}


bool MapMatrix2Imagis::parse(string str, string classID, shared_ptr<CBaseElement> &element)
{
	deque<string> vecWord = split(str, "\\s+");
	int totalNum = vecWord.size();
	vector<CPt> vecPt;

	for (int i = 0; i < vecWord.size(); i = i + 4)
	{
		double x = stringToNum<double>(vecWord[i]);
		double y = stringToNum<double>(vecWord[i + 1]);
		double z = stringToNum<double>(vecWord[i + 2]);
		double w = stringToNum<double>(vecWord[i + 3]);

		vecPt.emplace_back(x, y, z, w);
	}

	if (isPoint(classID))
	{
		makePt(vecPt, element);

		return true;
	}

	if (isPrism(classID))
	{
		makePrism(vecPt, element);
		return true;
	}

	if (isHumanTop(classID))
	{
		makeHumanTop(vecPt, element);
		return true;
	}

	if (isSteeple(classID))
	{
		makeSteeple(vecPt, element);
		return true;
	}

	if (isCylinder(classID))
	{
		makeCylinder(vecPt, element);
		return true;
	}

	if (isPolygon(classID))
	{
		makePolygon(vecPt, element);
	}

	return false;
}


bool MapMatrix2Imagis::isPoint(string classId)
{
	if (classId == "[INT] 77824")
	{
		return true;
	}

	return false;
}

bool MapMatrix2Imagis::isPrism(string classId)
{
	if (classId == "[INT] 65536")
	{
		return true;
	}

	return false;
}

bool MapMatrix2Imagis::isHumanTop(string classId)
{
	if (classId == "[INT] 69632")
	{
		return true;
	}

	return false;
}

bool MapMatrix2Imagis::isSteeple(string classId)
{
	if (classId == "[INT] 73728")
	{
		return true;
	}

	return false;
}

bool MapMatrix2Imagis::isCylinder(string classId)
{
	if (classId == "[INT] 81920")
	{
		return true;
	}

	return false;
}

bool MapMatrix2Imagis::isPolygon(std::string classId)
{
	if (classId == "[INT] 98304")
	{
		return true;
	}

	return false;
}

void MapMatrix2Imagis::makePt(vector<CPt> vecPt, shared_ptr<CBaseElement> &element)
{
	if (vecPt.empty())
	{
		return;
	}

	shared_ptr<CBaseElement> el(new CPt(vecPt[0].x, vecPt[0].y, vecPt[0].z, vecPt[0].w));
	element = el;
}

void MapMatrix2Imagis::makePrism(vector<CPt> vecPt, shared_ptr<CBaseElement> &element)
{
	map<int, vector<CPt>> mapPt;
	seperatePt(vecPt, mapPt);

	auto iter = mapPt.begin();
	auto vecFirstPt = iter->second;

	if (vecFirstPt.empty())
	{
		return;
	}

	auto basePt = vecFirstPt[0];

	iter++;
	auto roof = iter->second;

	shared_ptr<CBaseElement> pt(new CPrism(roof, basePt));
	element = pt;
}

void MapMatrix2Imagis::makeHumanTop(vector<CPt> vecPt, shared_ptr<CBaseElement> &element)
{
	map<int, vector<CPt>> mapPt;
	seperatePt(vecPt, mapPt);

	auto iter = mapPt.begin();
	auto vecFirstPt = iter->second;

	if (vecFirstPt.empty())
	{
		return;
	}

	auto basePt = vecFirstPt[0];

	iter++;
	auto roof = iter->second;

	iter++;
	auto roofTop = iter->second;

	shared_ptr<CHumanTop> pt(new CHumanTop(roof, roofTop, basePt));
	element = pt;
}

void MapMatrix2Imagis::makeSteeple(vector<CPt> vecPt, shared_ptr<CBaseElement> &element)
{
	map<int, vector<CPt>> mapPt;
	seperatePt(vecPt, mapPt);

	auto iter = mapPt.begin();
	auto vecFirstPt = iter->second;

	if (vecFirstPt.empty())
	{
		return;
	}

	auto basePt = vecFirstPt[0];

	iter++;
	auto roof = iter->second;

	iter++;
	auto roofTop = iter->second;

	if (roofTop.empty())
	{
		return;
	}

	shared_ptr<CSteeple> pt(new CSteeple(roof, roofTop[0], basePt));
	element = pt;
}

void MapMatrix2Imagis::makeCylinder(vector<CPt> vecPt, shared_ptr<CBaseElement> &element)
{
	map<int, vector<CPt>> mapPt;
	seperatePt(vecPt, mapPt);

	auto iter = mapPt.begin();
	auto vecFirstPt = iter->second;

	if (vecFirstPt.empty())
	{
		return;
	}

	auto basePt = vecFirstPt[0];

	iter++;
	auto roof = iter->second;

	if (roof.size() != 3)
	{
		return;
	}

	auto roof1 = roof[0];
	auto roof2 = roof[1];
	auto roof3 = roof[2];

	shared_ptr<CCylinder> pt(new CCylinder(roof1, roof2, roof3, basePt));
	element = pt;
}

void MapMatrix2Imagis::makePolygon(vector<CPt> vecPt, shared_ptr<CBaseElement> &element)
{
	shared_ptr<CPolygon> pt(new CPolygon(vecPt));
	element = pt;
}

void MapMatrix2Imagis::seperatePt(vector<CPt> vecPt, map<int, vector<CPt>> &mapPt)
{
	for (auto pt : vecPt)
	{
		auto it = mapPt.find(pt.z);

		if (it != mapPt.end())
		{
			it->second.push_back(pt);
		}
		else
		{
			vector<CPt> pts;
			pts.push_back(pt);
			mapPt.insert(std::pair<double, vector<CPt>>(pt.z, pts));
		}
	}
}

