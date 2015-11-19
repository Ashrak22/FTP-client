/** 
 * @file CGUIItem.h
 * @brief Definiton of CGUIItem Class
 * @author Jakub J Simon
 *
 */

#ifndef CGUIITEM_H
#define	CGUIITEM_H
#include <ncurses.h>
#include <string>
#include "CList.h"
/*!
\class CGUIItem
\brief Parent class for all GUI classes
*/
using namespace std;
///CGUIItem defines several methods that every child class uses as the constructor fillWindow od destructor
class CGUIItem
{

public:
    CGUIItem(int x, int y, int width, int height);
    void fillWindow();
    virtual void redraw();
    ~CGUIItem();
protected:
    ///Coordinates and size
    int mXCoord, mYCoord, mXSize, mYSize;
    ///WINDOW as defined by ncurses
    WINDOW * mWindow;
    ///Caption where aplicable
    string mCaption;
};


#endif	/* CGUIITEM_H */

