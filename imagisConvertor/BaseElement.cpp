#include "stdafx.h"
#include "BaseElement.h"

using namespace std;

CBaseElement::CBaseElement()
{
}


CBaseElement::~CBaseElement()
{
}


CPt::CPt(double x, double y, double z, double w)
	:x(x), y(y), z(z), w(w)
{
}

CPt::~CPt() 
{
}

CPrism::CPrism(vector<CPt> roof, CPt basePt)
	: roof(roof), basePt(basePt)
{
}

CPrism::~CPrism() 
{
}


CHumanTop::CHumanTop(vector<CPt> roof, std::vector<CPt> roofTop, CPt basePt)
	: roof(roof), roofTop(roofTop), basePt(basePt)
{
}

CSteeple::CSteeple(vector<CPt> roof, CPt roofTop, CPt basePt)
	: roof(roof), roofTop(roofTop), basePt(basePt)
{
}

CCylinder::CCylinder(CPt roof1, CPt roof2, CPt roof3, CPt basePt)
	: roof1(roof1), roof2(roof2), roof3(roof3), basePt(basePt)
{
}

CPolygon::CPolygon(vector<CPt> polygon)
	: polygon(polygon)
{
}