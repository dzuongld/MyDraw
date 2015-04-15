#pragma once
#include "figure.h"

class MyRectangle :
	public Figure
{
public:
	DECLARE_SERIAL( MyRectangle )

	MyRectangle(int x0, int y0, int x1, int y1, COLORREF crColor);
	MyRectangle(void);
    ~MyRectangle(void);

	//overwirte Serialize method
	void Serialize( CArchive& archive );

    // Draw a rectangle using graphic context pDC
    void draw(CDC* pDC);

    // Return true if rectangle is close to coordinates (x,y)
    bool isCloseTo(int x, int y);

	//clone rectangle
	Figure* cloneFigure();
};
