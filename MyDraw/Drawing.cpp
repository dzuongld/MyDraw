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
#include "Drawing.h"
#include "Line.h"
#include "MyRectangle.h"
#include "MyOval.h"
#include "MyGroup.h"

IMPLEMENT_SERIAL( Drawing, CObject, 1 )

// Constructor/Destructor
Drawing::Drawing(void)
{
	// Initial mode is selectMode
	this->editMode = SelectMode;

	// Initial state for mouse button is not pressed.
	this->previousMouseMode = Drawing::NoButtonPressed;

	//set default color to black
	this->currentColor = RGB( 0, 0, 0 );
}

Drawing::~Drawing(void)
{
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = this->figures.at(i);
		figures.erase(figures.begin()+i);
		delete f;
		i--;
		
	}
	
}

//pick color
void 
Drawing::pickColor(CView * cview)
{
	CColorDialog dlg; 
	 if (dlg.DoModal() == IDOK) 
	 { 
		  this->currentColor = dlg.GetColor(); 
	 } 
	 else 
	 {
		 this->currentColor =  RGB( 0, 0, 0 );
	 }
	 for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		if (f->isSelected()) {
			f->changeColor(this->currentColor);			
		}
	}
	cview->RedrawWindow();
}


// Draw the figures in the Drawing.
// Called by the draw method in MyDrawView
void 
Drawing::draw(CDC* pDC)
{
	// For each figure in vector "figures" draw the figure with control points.
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		f->drawWithControlPoints(pDC);
		
	}

	// Draw the selection rectangle if enabled.
	this->drawSelectionRectangle(pDC);
}

// Set the edit mode for the editor.
void 
Drawing::setEditMode(EditMode mode)
{
	this->editMode = mode;
}

void
Drawing::deleteSelected(CView * cview)
{
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		if (f->isSelected()) {
		
			figures.erase(figures.begin()+i);
			delete f;
			i--;
		}
	}
	//this->editMode = SelectMode;
    this->selectAll(false);
    cview->RedrawWindow();
            

}

void 
Drawing::copySelected() 
{
	if (!this->tmpFigures.empty()) {
		this->tmpFigures.clear();
	}
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);

		if (f->isSelected()) {
			Figure * f2 = f->cloneFigure();
			this->tmpFigures.push_back(f2);
			
		}
	}
           
}

void
Drawing::paste(CView * cview) 
{
	for (unsigned i = 0; i < this->tmpFigures.size(); i++) {
		Figure * f = this->tmpFigures.at(i);
		Figure * f2 = f->cloneFigure();
		this->figures.push_back(f2);
									
	}
    
    cview->RedrawWindow();
         
}

void 
Drawing::sendBack(CView * cview) 
{
	vector<Figure *> tFigs;
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		
		if (!f->isSelected()) {
			this->figures.erase(figures.begin()+i);
			this->figures.push_back(f);

		}
	}
	
    cview->RedrawWindow();
}

void 
Drawing::bringFront(CView * cview)
{
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		
		if (f->isSelected()) {
			this->figures.erase(figures.begin()+i);
			this->figures.push_back(f);

		}
	}
	
    cview->RedrawWindow();
}

void
Drawing::groupFigures(CView * cview) 
{
	vector<Figure *> gFigures;
	int minX = 65535;
	int minY = 65535;
	int maxX = -65536;
	int maxY = -65536;

	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		
		if ((f->isSelected())) {
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

			gFigures.push_back(f);
			this->figures.erase(figures.begin()+i);
			
			i--;

		}
	}
	
	MyGroup * group = new MyGroup(minX, minY, maxX, maxY, gFigures);
	
	group->select(true);
	this->figures.push_back(group);

	/*vector<double *> xRatios = group->getXratios();
	vector<double *> yRatios = group->getYratios();

	for (unsigned i = 0; i < gFigures.size(); i++) {
		Figure * f = gFigures.at(i);
			
		vector<ControlPoint *> cp = f->getControlPoints();
		ControlPoint * p0 = cp.at(0);
		ControlPoint * p1 = cp.at(1);

		int x0 = p0->getX();
		int y0 = p0->getY();
		int x1 = p1->getX();
		int y1 = p1->getY();

		double* d1 = xRatios.at(i);
		double* d2 = yRatios.at(i);

		if (x0 == minX) {
			d1[0] = -1;
		} else if (x0 == maxX) {
			d1[0] = 1;
		} else {
			d1[0] = ((double)(x0-minX))/((double)(maxX-minX));
		}

		if (x1 == minX) {
			d1[1] = -1;
		} else if (x1 == maxX) {
			d1[1] = 1;
		} else {
			d1[1] = ((double)(x1-minX))/((double)(maxX-minX));
		}

		if (y0 == minY) {
			d2[0] = -1;
		} else if (y0 == maxY) {
			d2[0] = 1;
		} else {
			d2[0] = ((double)(y0-minY))/((double)(maxY-minY));
		}

		if (y1 == minY) {
			d2[0] = -1;
		} else if (y1 == maxY) {
			d2[0] = 1;
		} else {
			d2[0] = ((double)(y1-minY))/((double)(maxY-minY));
		}
	}*/
	
    cview->RedrawWindow();
}

	
void
Drawing::ungroupFigures(CView * cview) 
{
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = this->figures.at(i);
		if ((f->isSelected()) && (f->getFigureType() == Figure::Group)) {
			MyGroup * g = dynamic_cast<MyGroup*>(f);
			vector<Figure *> v = g->getGroup();
			for (unsigned j = 0; j < v.size(); j++) {
				Figure * fig = v.at(j);
				Figure * f2 = fig->cloneFigure();
				this->figures.push_back(f2);
				
			}
			v.clear();
			this->figures.erase(figures.begin()+i);
			delete g;
			i--;
			
		}
	}
	
    cview->RedrawWindow();
}

// Call back when the mouse is pressed, moved, or released.
// It is called by the mouse call backs in MyDrawView
void 
Drawing::OnMouse(CView * cview, int nFlags, CPoint point) {

	// Check if the mouse butto is pressed now
	bool mousePressedNow = ((nFlags & MK_LBUTTON)!=0);

	if ( mousePressedNow ) {
		// Mouse left button is currently pressed

		if (this->previousMouseMode == Drawing::NoButtonPressed) {
			// Button has just been pressed

			// Update previous mouse mode
			this->previousMouseMode = Drawing::ButtonPressed;

			// Check edit mode

			if (this->editMode == Drawing::NewLineMode) {
				// Edit mode is NewLineMode. 
				// This is because the user just selected the Figure->Line menu

				// Create a new line.
				Line * line = new Line(point.x, point.y, point.x, point.y, this->currentColor);

				// Add to the list of figures
				this->figures.push_back(line);

				// Now switch to select mode
				this->editMode = SelectMode;

				// Select only the last control point of the line 
				// so dragging will modify this control point.
				this->selectAll(false);
				line->selectLast(true);

				// Update previous mouse coordinates
				this->previousX = point.x;
				this->previousY = point.y;

				// Redraw window. This will clal the draw method.
				cview->RedrawWindow();
			}
			else if (this->editMode == Drawing::NewRectangleMode ) {

                // Edit mode is NewRectangleMode.
                // This is because the user just selected the Figure->Rectangle menu

                // Create a new rectangle.
                MyRectangle * rectangle = new MyRectangle(point.x, point.y, point.x, point.y, this->currentColor);

                // Add to the list of figures
                this->figures.push_back(rectangle);

                // Now switch to select mode
                this->editMode = SelectMode;

                // Select only the last control point of the line
                // so dragging will modify this control point.
                this->selectAll(false);
                rectangle->selectLast(true);

                // Update previous mouse coordinates
                this->previousX = point.x;
                this->previousY = point.y;

                // Redraw window. This will call the draw method.
                cview->RedrawWindow();
            }
			else if (this->editMode == Drawing::NewOvalMode ) {

                // Edit mode is NewRectangleMode.
                // This is because the user just selected the Figure->Rectangle menu

                // Create a new rectangle.
                MyOval * oval = new MyOval(point.x, point.y, point.x, point.y, this->currentColor);

                // Add to the list of figures
                this->figures.push_back(oval);

                // Now switch to select mode
                this->editMode = SelectMode;

                // Select only the last control point of the line
                // so dragging will modify this control point.
                this->selectAll(false);
                oval->selectLast(true);

                // Update previous mouse coordinates
                this->previousX = point.x;
                this->previousY = point.y;

                // Redraw window. This will call the draw method.
                cview->RedrawWindow();
            }
			
			else if (this->editMode == Drawing::SelectMode) {

				// The edit mode is selection.

				// Check first if there is a selected control point in 
				// mouse coordinates point.x,point.y
				ControlPoint * controlPoint = findControlPoint(point.x, point.y);
				if (controlPoint != NULL && controlPoint->isSelected()) {
					// Found a control point selected at this x, y

					// Deselect all control points
					this->selectAll(false);

					// Select the control point that was found.
					/*Figure * fg = controlPoint->ofFigure();
					if (fg->getFigureType() == Figure::Group) {
						MyGroup * g = dynamic_cast<MyGroup*>(fg);
						vector<Figure *> v = g->getGroup();
						for (unsigned j = 0; j < v.size(); j++) {
						
							Figure * fig = v.at(j);
							vector<ControlPoint *> cp = fig->getControlPoints();
							ControlPoint * p0 = cp.at(0);
							ControlPoint * p1 = cp.at(1);
							ControlPoint * c = controlPoint->closerTo(p0,p1);
							c->select(true);
			
						}
						controlPoint->select(true);
					} else {*/

						controlPoint->select(true);
					//}

					// Update previous mouse coordinates
					this->previousX = point.x;
					this->previousY = point.y;
				}
				else {
					// No selected control point was found.

					Figure * f;

					// Check if any of the figures selected is close to this coordinate
					if (isAnySelectedFigureCloseTo(point.x, point.y)) {
						// Yes. Update previous mouse coordinates so all the control points selected can be dragged.
						this->previousX = point.x;
						this->previousY = point.y;
					}

					// Check if there is a figure close to point.x,point.y even if it is 
					// not selected.
					else if ((f=isAnyFigureCloseTo(point.x, point.y))!=NULL) {
						// Yes. Deselect anything selected before and select this figure.
						this->selectAll(false);

						// Select this figure
						
						/*if (f->getFigureType() == Figure::Group) {
							MyGroup * g = dynamic_cast<MyGroup*>(f);
							g->select(true);
						} else {*/
						f->select(true);
						//}

						// Update previous mouse coordinates
						this->previousX = point.x;
						this->previousY = point.y;

						// Redraw window
						cview->RedrawWindow();
					}
					else {
						// No figure selected. Deselect all control points.
						this->selectAll(false);

						// Redraw window
						cview->RedrawWindow();

						// Store previous mouse coordinates
						this->previousX = point.x;
						this->previousY = point.y;
					}
				}
			}
		}
		else {
			// Mouse button was already pressed.

			// Check if any of the control points is selected.
			if (isSelected()) {
				// There are control points selected.

				// Mouse is being dragged. Drag control points
				dragSelectedControlPoints(point.x - this->previousX, point.y - this->previousY);

				// Update previous mouse coordinates
				this->previousX = point.x;
				this->previousY = point.y;
			}
			else {
				// There are no control points selected and mouse is being dragged.

				// Update selection rectangle
				updateSelectionRectangle(this->previousX, this->previousY, point.x, point.y);
			}		

			cview->RedrawWindow();
		}
	}
	else {
		// Button is not pressed.

		if (this->previousMouseMode == Drawing::ButtonPressed) {
			// Mouse button has just been released.

			if (this->selectionRectangleEnabled) {
				// Selection rectangle was enabled

				// Select figures inside the selection area.
				this->selectFiguresInArea(this->previousX, this->previousY, point.x, point.y);

				// Erase selection rectangle
				this->disableSelectionRectangle();
			}
			cview->RedrawWindow();
		}

		this->previousMouseMode = Drawing::NoButtonPressed;
	}
}

// Move an increment dx, dy the selected control points.
void Drawing::dragSelectedControlPoints( int dx, int dy ) {
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		f->dragSelectedControlPoints(dx, dy);
		
	}
}

// Select/deselct all figures in the drawing
void Drawing::selectAll(bool selected)
{
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		f->select(selected);
		
	}
}

// Return true if there is a selected figure close to (x,y)
bool Drawing::isAnySelectedFigureCloseTo(int x, int y) 
{
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		if ( f->isSelected() && f->isCloseTo(x,y) ) {
			return true;
		}
		
		
	}

	return false;
}

// Return the figure selected or unselected that is close to the coordinate (x,y)
Figure * Drawing::isAnyFigureCloseTo(int x, int y) 
{
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		if ( f->isCloseTo(x,y) ) {
			return f;
		}
		
	}

	return NULL;
}

// Return true if any of the control points in the figures is selected
bool Drawing::isSelected() {
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		if ( f->isSelected() ) {
			return true;
		}
		
	}

	return false;
}

// Find the control point that encloses the coordinates (x,y)
ControlPoint * Drawing::findControlPoint(int x, int y)
{
	ControlPoint * c;
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		c = f->findControlPoint(x,y);
		if ( c != NULL ) {
			return c;
		}
		
	}

	return NULL;
}

// Select control points that are inside a rectangular area
void Drawing::selectFiguresInArea(int x0, int y0, int x1, int y1)
{
	for (unsigned i = 0; i < this->figures.size(); i++) {
		Figure * f = figures.at(i);
		if (f->isInsideArea(x0, y0, x1, y1)) {
			
			f->select(true);
		}
	}
}

// Enable and update coordinates of selection rectangle
void Drawing::updateSelectionRectangle(int x0, int y0, int x1, int y1)
{
	this->selectionRectangleX0 = x0;
	this->selectionRectangleY0 = y0;
	this->selectionRectangleX1 = x1;
	this->selectionRectangleY1 = y1;
	this->selectionRectangleEnabled = true;
}

// Disable selection rectangle
void Drawing::disableSelectionRectangle() 
{
	this->selectionRectangleEnabled = false;
}

// Draw selection rectangle if enabled
void Drawing::drawSelectionRectangle(CDC *pDC)
{
	if (!this->selectionRectangleEnabled) {
		return;
	}

	// Draw selection triangle
	CPen pen( PS_SOLID, 0, RGB( 255, 0, 0 ) );
	CPen* pOldPen = pDC->SelectObject( &pen );

	pDC->MoveTo(this->selectionRectangleX0, this->selectionRectangleY0);
	pDC->LineTo(this->selectionRectangleX1, this->selectionRectangleY0);
	pDC->LineTo(this->selectionRectangleX1, this->selectionRectangleY1);
	pDC->LineTo(this->selectionRectangleX0, this->selectionRectangleY1);
	pDC->LineTo(this->selectionRectangleX0, this->selectionRectangleY0);
}

void Drawing::Serialize(CArchive& ar)
{
	CObject::Serialize( ar );
	
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		//m_strfileName
		int c = this->figures.size(); 
		ar << c;
		
		for (unsigned i = 0; i < this->figures.size(); i++) {
			Figure * f = figures.at(i);
			ar << f;
			//f->Serialize(ar);
		}
	}
	else
	{
		// TODO: add loading code here
		//Clean(); 
        int count = 0; 
        ar >> count; 

        for(unsigned i = 0; i < count; ++i) 
        { 
            Figure *p;
            ar >> p; // Deserialize whatever kind of object comes next
            figures.push_back(p); 
			//p->Serialize(ar);
        } 
	}
}
