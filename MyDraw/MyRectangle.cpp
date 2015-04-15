#include "StdAfx.h"
#include "MyRectangle.h"

IMPLEMENT_SERIAL( MyRectangle, Figure, 1 )

MyRectangle::MyRectangle(int x0, int y0, int x1, int y1, COLORREF crColor)
:Figure(Figure::FigureType::Rectangle)
{
    controlPoints.push_back(new ControlPoint(this, x0,y0));
    controlPoints.push_back(new ControlPoint(this, x1,y1));
	this->crColor = crColor;
}

MyRectangle::MyRectangle(void)
{
}

MyRectangle::~MyRectangle(void)
{
}

// Draw a rectangle using graphic context pDC
void MyRectangle::draw(CDC* pDC)
{
    ControlPoint * p0 = controlPoints.at(0);
    ControlPoint * p1 = controlPoints.at(1);
   
    // Find minx, miny, maxx, maxy
    int minX = (p0->getX()<p1->getX())?p0->getX():p1->getX();
    int minY = (p0->getY()<p1->getY())?p0->getY():p1->getY();
    int maxX = (p0->getX()<p1->getX())?p1->getX():p0->getX();
    int maxY = (p0->getY()<p1->getY())?p1->getY():p0->getY();

    // Draw rectangle
    CPen pen( PS_SOLID, 0, crColor );
    CPen* pOldPen = pDC->SelectObject( &pen );
    pDC->MoveTo(minX, minY);
    pDC->LineTo(maxX, minY);
    pDC->LineTo(maxX, maxY);
    pDC->LineTo(minX, maxY);
    pDC->LineTo(minX, minY);
	
	CBrush brush( crColor );
	CBrush* pOldBrush = pDC->SelectObject( &brush );
	pDC->Rectangle(minX, minY, maxX, maxY);
}

// Return true if rectangle is close to coordinates (x,y)
bool MyRectangle::isCloseTo(int x, int y)
{
    ControlPoint * p0 = controlPoints.at(0);
    ControlPoint * p1 = controlPoints.at(1);
   
    // Find minx, miny, maxx, maxy
    int minX = (p0->getX()<p1->getX())?p0->getX():p1->getX();
    int minY = (p0->getY()<p1->getY())?p0->getY():p1->getY();
    int maxX = (p0->getX()<p1->getX())?p1->getX():p0->getX();
    int maxY = (p0->getY()<p1->getY())?p1->getY():p0->getY();

    // Check if (x,y) is close to any of the borders.
    if (Figure::distancePointToLine(x, y, minX, minY, maxX, minY) < Figure::smallDistance ||
        Figure::distancePointToLine(x, y, maxX, minY, maxX, maxY) < Figure::smallDistance ||
        Figure::distancePointToLine(x, y, minX, maxY, maxX, maxY) < Figure::smallDistance ||
        Figure::distancePointToLine(x, y, minX, minY, minX, maxY) < Figure::smallDistance) {
        return true;
    }

    return false;
}

Figure * MyRectangle::cloneFigure()
{
	ControlPoint * p0 = controlPoints.at(0);
    ControlPoint * p1 = controlPoints.at(1);
   
    int x0 = p0->getX();
    int x1 = p1->getX();
    int y0 = p0->getY();
    int y1 = p1->getY();

    Figure * rectangle = new MyRectangle(x0, y0, x1, y1, crColor);
	return rectangle;
}

void MyRectangle::Serialize(CArchive& ar)
{
	Figure::Serialize( ar );
}
