#pragma once
#include <vector>

class CBaseElement
{
public:
	CBaseElement();
	virtual ~CBaseElement();
};

/*
** @brief ԭ��
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
** @brief ����
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
** @brief ���ֶ�
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
** @brief �ⶥ
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
** @brief Բ��
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
** @brief ƽ������
*/
class CPolygon : public CBaseElement
{
public:
	CPolygon(std::vector<CPt> polygon);

	std::vector<CPt> polygon;
};
