#include "StdAfx.h"
#include "MyGroup.h"
#include <math.h>


IMPLEMENT_SERIAL( MyGroup, Figure, 1 )

MyGroup::MyGroup(int x0, int y0, int x1, int y1, vector<Figure *> groupFigures)
:Figure(Figure::FigureType::Group)
{
	controlPoints.push_back(new ControlPoint(this, x0,y0));
    controlPoints.push_back(new ControlPoint(this, x1,y1));
	vector<Figure *> g (groupFigures);
	this->groupFigures = g;
	/*for (unsigned i = 0; i < this->groupFigures.size(); i++) {
		double* d1 = new double[2];
		double* d2 = new double[2];
		xRatio.push_back(d1);
		yRatio.push_back(d2);
	}*/
}

MyGroup::MyGroup(void)
{
}

MyGroup::~MyGroup(void)
{
	for (unsigned i = 0; i < this->groupFigures.size(); i++) {
		Figure * f = this->groupFigures.at(i);
		groupFigures.erase(groupFigures.begin()+i);
		delete f;
		i--;
		
	}
}

void MyGroup::changeColor(COLORREF color)
{
	for (unsigned i = 0; i < this->groupFigures.size(); i++) {
		Figure * f = this->groupFigures.at(i);
		f->changeColor(color);
	}
}

//scale
void MyGroup::resize()
{	
	//ControlPoint * p0 = this->controlPoints.at(0);
	//ControlPoint * p1 = this->controlPoints.at(1);

	//int minX = p0->getX();
	//int minY = p0->getY();
	//int maxX = p1->getX();
	//int maxY = p1->getY();

	//int x0,y0,x1,y1;

	//for (unsigned i = 0; i < this->groupFigures.size(); i++) {
	//	Figure * f = this->groupFigures.at(i);
	//	vector<ControlPoint *> v = f->getControlPoints();
	//	ControlPoint * c0 = v.at(0);
	//	ControlPoint * c1 = v.at(1);
	//	//c->move(c->getX() + dx, c->getY() + dy);

	//	double* d1 = this->xRatio.at(i);
	//	double* d2 = this->yRatio.at(i);

	//	if (d1[0] == -1) {
	//		x0 = minX;
	//	} else if (d1[0] == 1) {
	//		x0 = maxX;
	//	} else {
	//		x0 = d1[0]*(maxX-minX) + minX;
	//	}

	//	if (d1[1] == -1) {
	//		x1 = minX;
	//	} else if (d1[1] == 1) {
	//		x1 = maxX;
	//	} else {
	//		x1 = d1[1]*(maxX-minX) + minX;
	//	}

	//	if (d2[0] == -1) {
	//		y0 = minY;
	//	} else if (d2[0] == 1) {
	//		y0 = maxY;
	//	} else {
	//		y0 = d2[0]*(maxY-minY) + minY;
	//	}

	//	if (d2[1] == -1) {
	//		y1 = minY;
	//	} else if (d2[1] == 1) {
	//		y1 = maxY;
	//	} else {
	//		y1 = d2[1]*(maxY-minY) + minY;
	//	}
	//	
	//	c0->move(x0,y0);
	//	c1->move(x1,y1);

	//}
}

 //Draw group using graphic context pDC
void MyGroup::draw(CDC* pDC)
{
    
	for (unsigned i = 0; i < this->groupFigures.size(); i++) {
		Figure * f = this->groupFigures.at(i);
		f->draw(pDC);
	}
}


// Return true if rectangle is close to coordinates (x,y)
bool MyGroup::isCloseTo(int x, int y)
{
    int minX = 65535;
	int minY = 65535;
	int maxX = -65536;
	int maxY = -65536;

	for (unsigned i = 0; i < this->groupFigures.size(); i++) {
		Figure * f = this->groupFigures.at(i);
			
		vector<ControlPoint *> cp = f->getControlPoints();
		ControlPoint * p0 = cp.at(0);
		ControlPoint * p1 = cp.at(1);
   
			// Find minx, miny, maxx, maxy
		int tminX = (p0->getX()<p1->getX())?p0->getX():p1->getX();
		int tminY = (p0->getY()<p1->getY())?p0->getY():p1->getY();
		int tmaxX = (p0->getX()<p1->getX())?p1->getX():p0->getX();
		int tmaxY = (p0->getY()<p1->getY())?p1->getY():p0->getY();
		minX = (minX<tminX)?minX:tminX;
		minY = (minY<tminY)?minY:tminY;
		maxX = (maxX<tmaxX)?tmaxX:maxX;
		maxY = (maxY<tmaxY)?tmaxY:maxY;
		
		// Check if (x,y) is close to any of the borders.
		if (Figure::distancePointToLine(x, y, minX, minY, maxX, minY) < Figure::smallDistance ||
			Figure::distancePointToLine(x, y, maxX, minY, maxX, maxY) < Figure::smallDistance ||
			Figure::distancePointToLine(x, y, minX, maxY, maxX, maxY) < Figure::smallDistance ||
			Figure::distancePointToLine(x, y, minX, minY, minX, maxY) < Figure::smallDistance) {
			return true;
		}

	}
	    
    return false;
}

vector<Figure *> & MyGroup::getGroup()
{
	return this->groupFigures;
}

//vector<double *> & MyGroup::getXratios()
//{
//	return this->xRatio;
//}
//
//vector<double *> & MyGroup::getYratios()
//{
//	return this->yRatio;
//}

// Select/deselect all control points
void MyGroup::select(bool selected)
{
	for (unsigned i = 0; i < this->controlPoints.size(); i++) {
		ControlPoint * c = controlPoints.at(i);
		c->select(selected);
	}
	for (unsigned i = 0; i < this->groupFigures.size(); i++) {
		Figure * f = this->groupFigures.at(i);
		vector<ControlPoint *> cp = f->getControlPoints();
		for (unsigned j = 0; j < cp.size(); j++) {
			ControlPoint * c = cp.at(j);
			c->select(selected);
		}
		
	}
}

// Return true if any of the control points of this figure is selected
bool MyGroup::isSelected() {
	for (unsigned i = 0; i < this->controlPoints.size(); i++) {
		ControlPoint * c = controlPoints.at(i);
		if (c->isSelected()) {
			return true;
		}
	}
	for (unsigned i = 0; i < this->groupFigures.size(); i++) {
		Figure * f = this->groupFigures.at(i);
		if (f->isSelected()) {
			return true;
		}
		
	}
	return false;
}

void MyGroup::dragSelectedControlPoints( int dx, int dy )
{
	for (unsigned i = 0; i < this->controlPoints.size(); i++) {
		ControlPoint * c = controlPoints.at(i);
		if (c->isSelected()) {
			c->move(c->getX() + dx, c->getY() + dy);
		}
	}
	for (unsigned i = 0; i < this->groupFigures.size(); i++) {
		Figure * f = this->groupFigures.at(i);
		f->dragSelectedControlPoints(dx, dy);
		
	}
	//resize();
}

Figure * MyGroup::cloneFigure() {
	ControlPoint * p0 = this->controlPoints.at(0);
	int x0 = p0->getX();
	int y0 = p0->getY();
	ControlPoint * p1 = this->controlPoints.at(1);
	int x1 = p1->getX();
	int y1 = p1->getY();
	vector<Figure *> figs;
	for (unsigned i = 0; i < this->groupFigures.size(); i++) {
		Figure * f = this->groupFigures.at(i);
		Figure * f2 = f->cloneFigure();
		figs.push_back(f2);
		
	}
	Figure * group = new MyGroup(x0, y0, x1, y1, figs);
	return group;
}

void MyGroup::Serialize(CArchive& ar)
{
	//Figure::Serialize( ar );
	
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		//m_strfileName
		
		ar << this->controlPoints.size();

		for (unsigned i = 0; i < this->controlPoints.size(); i++) {
			ControlPoint *  cp = controlPoints.at(i);
			int x = cp->getX();
			int y = cp->getY();
			ar << x;
			ar << y;
		}

		ar << this->groupFigures.size();
		
		for (unsigned i = 0; i < this->groupFigures.size(); i++) {
			Figure * f = this->groupFigures.at(i);
			ar << f;
			//f->Serialize(ar);
		}

			
	}
	else
	{
		// TODO: add loading code here
		int count = 0; 
        
		ar >> count; 
		//ar >> count;

        for(unsigned i = 0; i < count; ++i) 
        { 
            
			int x, y;
			x = 0;
			y = 0;
            ar >> x;
			ar >> y; // Deserialize whatever kind of object comes next
            controlPoints.push_back(new ControlPoint(this, x,y)); 
        } 
		int count2 = 0; 
        ar >> count2; 

        for(unsigned i = 0; i < count2; ++i) 
        { 
            Figure *p;
            ar >> p; // Deserialize whatever kind of object comes next
            this->groupFigures.push_back(p); 
			
        } 
		
	}
}
