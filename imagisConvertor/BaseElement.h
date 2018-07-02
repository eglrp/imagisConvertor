#pragma once
#include <vector>

class CBaseElement
{
public:
	CBaseElement();
	virtual ~CBaseElement();
};

/*
** @brief 原点
*/
class CPt : public CBaseElement
{
public:
	CPt(double x, double y, double z, double w);

	virtual ~CPt();

	double x;
	double y;
	double z;
	double w;
};

/*
** @brief 棱柱
*/
class CPrism : public CBaseElement
{
public:
	CPrism(std::vector<CPt> roof, CPt basePt);

	virtual ~CPrism();

	std::vector<CPt> roof;
	CPt basePt;
};

/*
** @brief 人字顶
*/
class CHumanTop : public CBaseElement
{
public:
	CHumanTop(std::vector<CPt> roof, std::vector<CPt> roofTop, CPt basePt);

	virtual ~CHumanTop() {}

	std::vector<CPt> roof;

	std::vector<CPt> roofTop;

	CPt basePt;
};

/*
** @brief 尖顶
*/
class CSteeple : public CBaseElement
{
public:
	CSteeple(std::vector<CPt> roof, CPt roofTop, CPt basePt);

	std::vector<CPt> roof;

	CPt roofTop;

	CPt basePt;
};

/*
** @brief 圆柱
*/
class CCylinder : public CBaseElement
{
public:
	CCylinder(CPt roof1, CPt roof2, CPt roof3, CPt basePt);

	CPt roof1;

	CPt roof2;

	CPt roof3;

	CPt basePt;
};

/*
** @brief 平面多边形
*/
class CPolygon : public CBaseElement
{
public:
	CPolygon(std::vector<CPt> polygon);

	std::vector<CPt> polygon;
};
