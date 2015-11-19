/** 
 * @file CButtonBar.cpp
 * @brief Implementation of CButtonBar Class
 * @author Jakub J Simon
 *
 */


#include "CButtonBar.h"

/*! \brief The constructor sets the coordinates to last line of window.


*/

CButtonBar::CButtonBar() : CGUIItem(0, LINES-1, COLS, 1)
{
    ///The constructor sets the coordinates fo the button bar, which are always all the same. It is the last Line of the window (the whole width). The it calls CButtonBar::redraw().
    redraw();
}

/*! \brief Draw the buttons.

*/
///Redraw draws the buttons on the coordinates whch were previously set by constructor. Each button is roughly 1/6 of the width
void CButtonBar::redraw()
{
    wattron(mWindow, A_BOLD);
    mvwprintw(mWindow, 0, 0, " 1");
    wattron(mWindow, COLOR_PAIR(4));
    mvwprintw(mWindow, 0, 2, "Help");
    for(int i = 6; i < COLS/6; i++)mvwprintw(mWindow, 0, i, " ");
    wattroff(mWindow, COLOR_PAIR(4));
    mvwprintw(mWindow, 0, COLS/6, " 3");
    wattron(mWindow, COLOR_PAIR(4));
    mvwprintw(mWindow, 0, 2+COLS/6, "ChangeWorkDir");
    for(int i = COLS/6+16; i < COLS/3; i++)mvwprintw(mWindow, 0, i, " ");
    wattroff(mWindow, COLOR_PAIR(4));
    mvwprintw(mWindow, 0, COLS/6+15, " 5");
    wattron(mWindow, COLOR_PAIR(4));
    mvwprintw(mWindow, 0, 17+COLS/6, "RecurDown");
    for(int i = COLS/6+26; i < COLS/2; i++)mvwprintw(mWindow, 0, i, " ");
    wattroff(mWindow, COLOR_PAIR(4));
    mvwprintw(mWindow, 0, COLS/2, " 7");
    wattron(mWindow, COLOR_PAIR(4));
    mvwprintw(mWindow, 0, COLS/2+2, "Upload");
    for(int i = COLS/2 + 8; i < 4*COLS/6; i++)mvwprintw(mWindow, 0, i, " ");
    wattroff(mWindow, COLOR_PAIR(4));
    mvwprintw(mWindow, 0, 4*COLS/6, " 8");
    wattron(mWindow, COLOR_PAIR(4));
    mvwprintw(mWindow, 0, 4*COLS/6+2, "MkDir");
    for(int i = 4*COLS/6 + 7; i < 5*COLS/6; i++)mvwprintw(mWindow, 0, i, " ");
    wattroff(mWindow, COLOR_PAIR(4));
    mvwprintw(mWindow, 0, 5*COLS/6, " 9");
    wattron(mWindow, COLOR_PAIR(4));
    mvwprintw(mWindow, 0, 5*COLS/6+2, "DelFile");
    for(int i = 5*COLS/6 + 9; i <= COLS; i++)mvwprintw(mWindow, 0, i, " ");
    
    wrefresh(mWindow);
    
}

/*! \brief Empty destructor just to call the parent destructor.
*/
CButtonBar::~CButtonBar()
{
    
}
