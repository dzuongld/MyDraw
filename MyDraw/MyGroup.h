#pragma once
#include "figure.h"
#include "Line.h"
#include "MyRectangle.h"
#include "MyOval.h"



class MyGroup :
	public Figure
{

protected:
	vector<Figure *> groupFigures;
	/*vector<double *> xRatio;
	vector<double *> yRatio;*/
	
public:
	DECLARE_SERIAL( MyGroup )

	MyGroup(void);
	MyGroup(int x0, int y0, int x1, int y1, vector<Figure *> groupFigures);
	~MyGroup(void);

	//overwirte Serialize method
	void Serialize( CArchive& archive );

	// Draw group using graphic context pDC
    void draw(CDC* pDC);

	//get figures in group
	vector<Figure *> & getGroup();

	vector<double *> & getXratios();

	vector<double *> & getYratios();

	//change color
	void changeColor(COLORREF color);

	//resize figures
	void resize();

    // Return true if group is close to coordinates (x,y)
    bool isCloseTo(int x, int y);

	//copy group
	Figure* cloneFigure();

	// Select/deselect all control points
	void select(bool selected);

	// Return true if any of the control points of this figure is selected
	bool isSelected();

	// Move the control points that are selected an increment dx,dy 
	void dragSelectedControlPoints( int dx, int dy );

	
	
};
