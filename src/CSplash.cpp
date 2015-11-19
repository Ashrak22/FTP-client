/** 
 * @file CSplash.cpp
 * @brief Implementation of CSplash Class
 * @author Jakub J Simon
 *
 */

#include "CSplash.h"
/*!
\brief Shows splash screen
*/
void CSplash::redraw()
{
     wattron(mWindow, COLOR_PAIR(3));
    for(int i = 0; i < mYSize; i++)
    {
        for(int j = 0; j < mXSize; j++) mvwprintw(mWindow, i, j," ");
    }
    
    box(mWindow, 0, 0);
    
    mvwprintw(mWindow, 1,1,"     @@@@@@@@@@@    ###########    @@@@@@@@@@  ");
    mvwprintw(mWindow, 2,1,"     @@@@@@@@@@@    ###########    @@@      @@ ");
    mvwprintw(mWindow, 3,1,"     @@@                ###        @@@       @@");
    mvwprintw(mWindow, 4,1,"     @@@                ###        @@@      @@ ");
    mvwprintw(mWindow, 5,1,"     @@@@@@@@@@@        ###        @@@     @@  ");
    mvwprintw(mWindow, 6,1,"     @@@@@@@@@@@        ###        @@@@@@@@@   ");
    mvwprintw(mWindow, 7,1,"     @@@                ###        @@@         ");
    mvwprintw(mWindow, 8,1,"     @@@                ###        @@@         ");
    mvwprintw(mWindow, 9,1,"     @@@                ###        @@@         ");
    mvwprintw(mWindow, 10,1,"     @@@                ###        @@@         ");
    mvwprintw(mWindow, 12,1,"        Final programme for BI-PA2          ");
    move(255,255);
    wrefresh(mWindow);
    wattroff(mWindow, COLOR_PAIR(3));
}
/*!
\param x x Coordinate
\param y y Coordinate
\param width Splashscreen width
\param height Splaschreen height
*/
CSplash::CSplash(int x, int y, int width, int height) : CGUIItem(x, y, width,height)
{
    redraw();
}
/*!
\brief Empty destructor to call parent destructor 
*/
CSplash::~CSplash()
{
    
}
