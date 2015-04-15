/*
  Copyright (c) 2011
  Gustavo Rodriguez-Rivera
  All rights reserved.

This work was developed by the author(s) at Purdue University
during 2011.

Redistribution and use in source and binary forms are permitted provided that
this entire copyright notice is duplicated in all such copies.  No charge,
other than an "at-cost" distribution fee, may be charged for copies,
derivations, or distributions of this material without the express written
consent of the copyright holders. Neither the name of the University, nor the
name of the author may be used to endorse or promote products derived from
this material without specific prior written permission.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR ANY PARTICULAR PURPOSE.
*/
#include "StdAfx.h"
#include "Line.h"
#include <math.h>

IMPLEMENT_SERIAL( Line, Figure, 1 )

// Constructor/destructor for a line
Line::Line(int x0, int y0, int x1, int y1, COLORREF crColor)
:Figure(Figure::FigureType::Line)
{
	controlPoints.push_back(new ControlPoint(this, x0,y0));
	controlPoints.push_back(new ControlPoint(this, x1,y1));
	this->crColor = crColor;
}

Line::Line(void)
{
}

Line::~Line(void)
{
}

// Draw a line using graphic context pDC
void Line::draw(CDC* pDC) {
	ControlPoint * p0 = controlPoints.at(0);
	ControlPoint * p1 = controlPoints.at(1);

	/*CPen pen;
	if (crColor == RGB(255,255,255)) {
		pen = CPen( PS_SOLID, 0, RGB(255,255,255) );
	} else {
		pen = CPen( PS_SOLID, 0, crColor );
	}*/
	CPen pen( PS_SOLID, 0, crColor );
	CPen* pOldPen = pDC->SelectObject( &pen );
	pDC->MoveTo(p0->getX(), p0->getY());
	pDC->LineTo(p1->getX(), p1->getY());
}

// Return true if line is close to coordinates (x,y)
bool Line::isCloseTo(int x, int y)
{
	ControlPoint * p1 = controlPoints.at(0);
	ControlPoint * p2 = controlPoints.at(1);

	double d = distancePointToLine(x, y, p1->getX(), p1->getY(), p2->getX(), p2->getY() );

	if (d < Figure::smallDistance) {
		// Liine is close to this point
		return true;
	}

	return false;
}

Figure * Line::cloneFigure() {
	ControlPoint * p0 = controlPoints.at(0);
    ControlPoint * p1 = controlPoints.at(1);
   
    int x0 = p0->getX();
    int x1 = p1->getX();
    int y0 = p0->getY();
    int y1 = p1->getY();

    Figure * line = new Line(x0, y0, x1, y1, crColor);
	return line;
}

void Line::Serialize(CArchive& ar)
{
	Figure::Serialize( ar );
	
}