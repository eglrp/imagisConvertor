#pragma once
#include "BaseElement.h"
#include <memory>
#include <map>
#include <fstream>

/*
** brief mapmatrixתimagis
*/

class MapMatrix2Imagis
{
public:
	MapMatrix2Imagis();
	virtual ~MapMatrix2Imagis();

	/*
	** brief convert ת��
	** param xmlFileName ����mapmatrix ����ͼԪxml�ı�
	** param outFileName ���imagis.3d��ʽ�ı�
	*/
	virtual bool convert(std::string xmlFileName, std::string outFileName);

	/*
	** brief outputImagis ���imagis .3d��ʽ
	** param vecBaseElement ����ͼԪ����
	** param 
	*/
	virtual bool outputImagis(std::vector<std::shared_ptr<CBaseElement>> vecBaseElement, std::string outFileName);
	
	/*
	** brief parse ����
	** param str �ı�
	** param classID ����id
	** param element ���صĻ���ͼԪ
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
	** brief isPoint �ж��Ƿ���ԭ������
	** param classId id����
	*/
	bool isPoint(std::string classId);

	/*
	** brief isPrism �ж��Ƿ�����������
	** param classId id����
	*/
	bool isPrism(std::string classId);

	/*
	** brief isHumanTop�ж��Ƿ������ֶ�
	** param classId �㼯��
	*/
	bool isHumanTop(std::string classId);

	/*
	** brief isSteeple�ж��Ƿ��Ǽⶥ
	** param classId �㼯��
	*/
	bool isSteeple(std::string classId);

	/*
	** brief isCylinder�ж��Ƿ���Բ��
	** param classId id����
	*/
	bool isCylinder(std::string classId);

	/*
	** brief isPolygon�ж��Ƿ���ƽ������
	** param classId id����
	*/
	bool isPolygon(std::string classId);

	/*
	** brief makePt������
	** param vecPt �㼯��
	** param element ���صĻ���ͼԪ
	*/
	void makePt(std::vector<CPt> vecPt, std::shared_ptr<CBaseElement> &element);

	/*
	** brief makePrism��������
	** param vecPt �㼯��
	** param element ���صĻ���ͼԪ
	*/
	void makePrism(std::vector<CPt> vecPt, std::shared_ptr<CBaseElement> &element);

	/*
	** brief makeHumanTop�������ֶ�
	** param vecPt �㼯��
	** param element ���صĻ���ͼԪ
	*/
	void makeHumanTop(std::vector<CPt> vecPt, std::shared_ptr<CBaseElement> &element);

	/*
	** brief makeSteeple�����ⶥ
	** param vecPt �㼯��
	** param element ���صĻ���ͼԪ
	*/
	void makeSteeple(std::vector<CPt> vecPt, std::shared_ptr<CBaseElement> &element);

	/*
	** brief makeCylinder����Բ��
	** param vecPt �㼯��
	** param element ���صĻ���ͼԪ
	*/
	void makeCylinder(std::vector<CPt> vecPt, std::shared_ptr<CBaseElement> &element);

	/*
	** brief makePolygon����ƽ������
	** param vecPt �㼯��
	** param element ���صĻ���ͼԪ
	*/
	void makePolygon(std::vector<CPt> vecPt, std::shared_ptr<CBaseElement> &element);

	/*
	** brief seperatePt ���ո߶Ƚ��е�ķֲ�
	** param vecPt �㼯��
	** param mapPt ���صķֲ�㼯��
	*/
	void seperatePt(std::vector<CPt> vecPt, std::map<int, std::vector<CPt>> &mapPt);
};

