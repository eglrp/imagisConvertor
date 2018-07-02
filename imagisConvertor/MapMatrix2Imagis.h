#pragma once
#include "BaseElement.h"
#include <memory>
#include <map>
#include <fstream>

/*
** brief mapmatrix转imagis
*/

class MapMatrix2Imagis
{
public:
	MapMatrix2Imagis();
	virtual ~MapMatrix2Imagis();

	/*
	** brief convert 转换
	** param xmlFileName 输入mapmatrix 基本图元xml文本
	** param outFileName 输出imagis.3d格式文本
	*/
	virtual bool convert(std::string xmlFileName, std::string outFileName);

	/*
	** brief outputImagis 输出imagis .3d格式
	** param vecBaseElement 基本图元容器
	** param 
	*/
	virtual bool outputImagis(std::vector<std::shared_ptr<CBaseElement>> vecBaseElement, std::string outFileName);
	
	/*
	** brief parse 解析
	** param str 文本
	** param classID 类型id
	** param element 返回的基本图元
	*/
	virtual bool parse(std::string str, std::string classID, std::shared_ptr<CBaseElement> &element);

protected:

	bool makeCircle(CPt pt1, CPt pt2, CPt pt3, double &x, double &y, double &r);

	bool makeCircleLoop(double x, double y, double z, double r, double w, std::vector<CPt> &vecCirclePt);

	bool outPrism(std::shared_ptr<CPrism> prism, std::ofstream &out, int id);

	bool outHumanTop(std::shared_ptr<CHumanTop> humanTop, std::ofstream &out, int id);

	bool outSteeple(std::shared_ptr<CSteeple> steeple, std::ofstream &out, int id);

	bool outCylinder(std::shared_ptr<CCylinder> cylinder, std::ofstream &out, int id);

	bool outPolygon(std::shared_ptr<CPolygon> polygon, std::ofstream &out, int id);

	/*
	** brief isPoint 判断是否是原点类型
	** param classId id类型
	*/
	bool isPoint(std::string classId);

	/*
	** brief isPrism 判断是否是棱柱类型
	** param classId id类型
	*/
	bool isPrism(std::string classId);

	/*
	** brief isHumanTop判断是否是人字顶
	** param classId 点集合
	*/
	bool isHumanTop(std::string classId);

	/*
	** brief isSteeple判断是否是尖顶
	** param classId 点集合
	*/
	bool isSteeple(std::string classId);

	/*
	** brief isCylinder判断是否是圆柱
	** param classId id类型
	*/
	bool isCylinder(std::string classId);

	/*
	** brief isPolygon判断是否是平面多边形
	** param classId id类型
	*/
	bool isPolygon(std::string classId);

	/*
	** brief makePt创建点
	** param vecPt 点集合
	** param element 返回的基本图元
	*/
	void makePt(std::vector<CPt> vecPt, std::shared_ptr<CBaseElement> &element);

	/*
	** brief makePrism创建棱柱
	** param vecPt 点集合
	** param element 返回的基本图元
	*/
	void makePrism(std::vector<CPt> vecPt, std::shared_ptr<CBaseElement> &element);

	/*
	** brief makeHumanTop创建人字顶
	** param vecPt 点集合
	** param element 返回的基本图元
	*/
	void makeHumanTop(std::vector<CPt> vecPt, std::shared_ptr<CBaseElement> &element);

	/*
	** brief makeSteeple创建尖顶
	** param vecPt 点集合
	** param element 返回的基本图元
	*/
	void makeSteeple(std::vector<CPt> vecPt, std::shared_ptr<CBaseElement> &element);

	/*
	** brief makeCylinder创建圆柱
	** param vecPt 点集合
	** param element 返回的基本图元
	*/
	void makeCylinder(std::vector<CPt> vecPt, std::shared_ptr<CBaseElement> &element);

	/*
	** brief makePolygon创建平面多边形
	** param vecPt 点集合
	** param element 返回的基本图元
	*/
	void makePolygon(std::vector<CPt> vecPt, std::shared_ptr<CBaseElement> &element);

	/*
	** brief seperatePt 按照高度进行点的分层
	** param vecPt 点集合
	** param mapPt 返回的分层点集合
	*/
	void seperatePt(std::vector<CPt> vecPt, std::map<int, std::vector<CPt>> &mapPt);
};

