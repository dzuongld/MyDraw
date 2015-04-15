#pragma once
#include "figure.h"

class MyOval :
	public Figure
{
public:
	DECLARE_SERIAL( MyOval )

	MyOval(int x0, int y0, int x1, int y1, COLORREF crColor);
    ~MyOval(void);
	MyOval(void);

	//overwirte Serialize method
	void Serialize( CArchive& archive );

    // Draw an oval using graphic context pDC
    void draw(CDC* pDC);

    // Return true if oval is close to coordinates (x,y)
    bool isCloseTo(int x, int y);

	//clone oval
	//MyOval * cloneOval();
	Figure* cloneFigure();
};
