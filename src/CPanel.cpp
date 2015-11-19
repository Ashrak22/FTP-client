/** 
 * @file CPanel.cpp
 * @brief Implementation of CPanel Class
 * @author Jakub J Simon
 *
 */

#include "CPanel.h"
#include "CFile.h"
#include <ncurses.h>
#include <iostream>
using namespace std;
/*!
\param x x Coordinate
\param y y Coordinate
\param width Panel width
\param height Panel height
*/
CPanel::CPanel(int x, int y, int width, int height) : CGUIItem(x,y,width,height)
{
    highlighted = 0;
    offset = 0;
    //Allow catching special keys for this Window
    keypad(mWindow, TRUE);
    redraw();
}
/*!
\brief Redraw window without files and Folders
*/
void CPanel::redraw()
{
    wattron(mWindow,COLOR_PAIR(1));
    fillWindow();
    mvwprintw(mWindow, 1, ((2*mXSize)/3)/2 - 2, "Name");
    mvwprintw(mWindow, 1, ((2*mXSize)/3)+1, "Size in MiB");
    mvwvline(mWindow, 1, (2*mXSize)/3, 0, mYSize-2);
    mvwhline(mWindow,2,1, 0, mXSize-2);
    mvwprintw(mWindow, mYSize, mXSize, " ");
    wrefresh(mWindow);
    wattroff(mWindow,COLOR_PAIR(1));
}
/*!
\brief Redraw window with Files and Folders
*/
void CPanel::redraw(const CList& folders, const CList& files)
{
    CFolder * temp;
    wattron(mWindow,COLOR_PAIR(1));
    fillWindow();
    mvwprintw(mWindow, 1, ((2*mXSize)/3)/2 - 2, "Name");
    mvwprintw(mWindow, 1, ((2*mXSize)/3)+1, "Size in MiB");
    
    mvwvline(mWindow, 1, (2*mXSize)/3, 0, mYSize-2);
    mvwhline(mWindow,2,1, 0, mXSize-2);
    for(int i = 0; i < mYSize-4; i++)
    {
        if(i+offset< folders.getSize()-2)
        {
            wattron(mWindow, A_BOLD);
            if(highlighted == i)
            {
                wattroff(mWindow, A_BOLD);
                wattron(mWindow, COLOR_PAIR(2));
                for(int j = 1; j < mXSize-2; j++) mvwprintw(mWindow, i+3, j, " ");
                wattron(mWindow, COLOR_PAIR(4));
                mvwvline(mWindow, i+3, (2*mXSize)/3, 0, 1);
                wattron(mWindow, COLOR_PAIR(2));
                wattron(mWindow, A_BOLD);
            }
            temp = folders.find(i+offset+1);
            mvwprintw(mWindow, i+3, 1, "%s", temp->getText().c_str());
            mvwprintw(mWindow, i+3, ((2*mXSize)/3)+1, "<DIR>");
            if(highlighted == i)
            {
                wattroff(mWindow, COLOR_PAIR(2));
                wattron(mWindow, COLOR_PAIR(1));
            }
            
        }
        else if(i+offset == folders.getSize()-2)
        {
            if(highlighted == i)
            {
                wattroff(mWindow, A_BOLD);
                wattron(mWindow, COLOR_PAIR(2));
                for(int j = 1; j < mXSize-2; j++) mvwprintw(mWindow, i+3, j, " ");
                wattron(mWindow, COLOR_PAIR(4));
                mvwvline(mWindow, i+3, (2*mXSize)/3, 0, 1);
                wattron(mWindow, COLOR_PAIR(2));
                wattron(mWindow, A_BOLD);
            }
            temp = folders.find(i+offset+1);
            mvwprintw(mWindow, i+3, 1, "%s", temp->getText().c_str());
            mvwprintw(mWindow, i+3, ((2*mXSize)/3)+1, "<DIR>");
            if(highlighted == i)
            {
                wattroff(mWindow, COLOR_PAIR(2));
                wattron(mWindow, COLOR_PAIR(1));
            }
            wattroff(mWindow, A_BOLD);
        }
        else
        {
           if(highlighted == i)
            {

                wattron(mWindow, COLOR_PAIR(2));
                for(int j = 1; j < mXSize-2; j++) mvwprintw(mWindow, i+3, j, " ");
                wattron(mWindow, COLOR_PAIR(4));
                mvwvline(mWindow, i+3, (2*mXSize)/3, 0, 1);
                wattron(mWindow, COLOR_PAIR(2));

            }
            temp = files.find(i-(folders.getSize()-1)+offset);
            if(temp == NULL) break;
            mvwprintw(mWindow, i+3, 1, "%s", temp->getText().c_str());
            mvwprintw(mWindow, i+3, ((2*mXSize)/3)+1, "%f", (float)(temp->getSize())/(1024*1024));
            if(highlighted == i)
            {
                wattroff(mWindow, COLOR_PAIR(2));
                wattron(mWindow, COLOR_PAIR(1));
            } 
        }
    }
    wattroff(mWindow, A_BOLD);
    wrefresh(mWindow);
}
/*!
\brief Move highlighting one position down
*/
void CPanel::keyDown(const CList& folders, const CList& files)
{
    if(highlighted == mYSize - 5)
    {
        if(folders.getSize() + files.getSize()-1 > mYSize -4 + offset) offset++;
    }
    else if(highlighted + offset == folders.getSize() + files.getSize()-1);
    else
    {
        highlighted++;
    }
    redraw(folders, files);
}
/*!
\brief Move highlighting one position UP
*/
void CPanel::keyUp(const CList& folders, const CList& files)
{
 
    if(highlighted == 0)
    {
        if(offset > 0) offset--;
    }
    else
    {
        highlighted--;
    }
    redraw(folders, files);
}
/*!
\brief Get ncurses representation of Window
\return Ncurses representation of Window
*/
WINDOW * CPanel::getWindow() const
{
    return mWindow;
}
/*!
\brief Get Highlighted position
\return Highlighted position
*/
int CPanel::getPosition() const
{
    return highlighted+offset+1;
}
/*!
\brief Clear highlighting, e.g. for change of folder
*/
void CPanel::clearPosition()
{
    offset = 0;
    highlighted = 0;
}
