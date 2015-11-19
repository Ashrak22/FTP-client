/** 
 * @file CGUIItem.cpp
 * @brief Implementation of CGUIItem Class
 * @author Jakub J Simon
 *
 */

#include "CGUIItem.h"
/*!
\brief Constructor
\param x The x Coordinate
\param y The y Coordinate
\param width The width of the window
\param height The height of the window
*/
CGUIItem::CGUIItem(int x, int y, int width, int height)
{
    ///
    mXCoord = x;
    mYCoord = y;
    mXSize = width;
    mYSize = height;
    mWindow = newwin(height, width, y, x);
    redraw(); 
    wrefresh(mWindow);
}
/*!
\brief Destructor
*/
CGUIItem::~CGUIItem()
{
    ///First the border must be destroyed (as is written in ncurses HOWTO)
    wborder(mWindow, ' ',' ',' ',' ',' ',' ',' ',' '); 
    wrefresh(mWindow);
    ///Then delete window itself
    delwin(mWindow);
}
/*!
\brief Fill window with blank background
*/
void CGUIItem::fillWindow()
{
    ///Ncurses has no direct function so we need to just fill every character place of the window with space. The color is specified by attribute of the window. After finish refresh the window, as all operations are buffered,
    for(int i = 0; i < mYSize; i++)
    {
        for(int j = 0; j < mXSize; j++) mvwprintw(mWindow, i, j," ");
    }
    
    box(mWindow, 0, 0);
    wrefresh(mWindow);
}
/*!
\brief Redraw window
*/
void CGUIItem::redraw()
{
    ///Empty overloaded function for specifying the layout of GUI element.
}
