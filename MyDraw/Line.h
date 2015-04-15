#pragma once

#include "Figure.h"

// Subclass for a Line. 
// Inherits most methods from Figure. 
class Line : public Figure
{
public:
	DECLARE_SERIAL( Line )

	// Constructor/destructor for a line
	Line(void);
	Line(int x0, int y0, int x1, int y1, COLORREF crColor);
	~Line(void);

	//overwirte Serialize method
	void Serialize( CArchive& archive );

	// Draw a line using graphic context pDC
	void draw(CDC* pDC);

	// Return true if line can be selected by clicking on coordinates (x,y)
	//bool selectFigureAt(int x, int y);

	// Return true if line is close to coordinates (x,y)
	bool isCloseTo(int x, int y);

	//clone line
	Figure* cloneFigure();
};

