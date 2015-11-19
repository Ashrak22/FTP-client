/** 
 * @file CProgress.cpp
 * @brief Implementation of CProgress Class
 * @author Jakub J Simon
 *
 */

#include "CProgress.h"

/*!
\param x x Coordinate
\param y y Coordinate
\param width Panel width
\param height Panel height
\brief It writes "No Download in Progress" by default
*/
CProgress::CProgress(int x, int y, int width, int height) : CGUIItem(x,y,width,height)
{
    redraw("No download in progress", 0, 0);
}
/*!
\param name Filename to be shown
\param speed Speed to be shown
\param percent Percentage to be drawn
\brief Redraw progress bar showing filename, speed and graphical percentage
*/
void CProgress::redraw(string name, int speed, int percent)
{
    wattron(mWindow, COLOR_PAIR(1));
    fillWindow();
    mvwprintw(mWindow, 0, 1, "%s", name.c_str());
    wattron(mWindow, COLOR_PAIR(5));
    for(int i = 0; i < (percent*(COLS - 15))/100; i++)mvwprintw(mWindow, 1, i+1, " ");
    wattron(mWindow, COLOR_PAIR(1));
    mvwprintw(mWindow, 1, COLS-15, "%d KiB/s", speed);
    wrefresh(mWindow);
}
/*!
\brief Empty destructor to call parent destructor
*/
CProgress::~CProgress()
{
    
}
