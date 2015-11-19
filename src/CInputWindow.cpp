/** 
 * @file CInputWindow.cpp
 * @brief Implementation of CInputWindow Class
 * @author Jakub J Simon
 *
 */

#include "CInputWindow.h"
#include <iostream>
using namespace std;
/*!
\brief Constructor
\param caption The prompt displayed to user
*/
CInputWindow::CInputWindow(string caption) : CGUIItem(COLS/6, 5, 2*COLS/3, 10)
{
    ///The coordinates are hardcoded 2/3 of screen wide and centered on X axis, and 10 lines high, starting 5 lines from top
    redraw(caption);
}
/*!
\brief Redraw window
\param caption The prompt displayed to user
*/
void CInputWindow::redraw(string caption)
{
    ///Writes the prompt to window and creates an "Input line"
    wattron(mWindow, COLOR_PAIR(6));
    fillWindow();
    mvwprintw(mWindow, 1, 1, "%s", caption.c_str());
    wattron(mWindow, COLOR_PAIR(2));
    for(int i = 0; i < mXSize-10; i++) mvwprintw(mWindow,3, i+5, " ");
    wmove(mWindow, 3, 5);
}
/*!
\brief Read response from user
\param isPass specifies if this is a password prompt
\return user input 
*/
string CInputWindow::getResponse(bool isPass)
{
    int i = 0;
    int ch;
    string result;
    while(1)
    {
        ch = wgetch(mWindow);
        if(ch == '\n')
        {
            break;
        }
	///Add typed characters to the string, if the character has value 127, delete last character, if the character is \n end the input. This functions echoes the characters to user, so he can see the typed text, if isPass is true, it echoes only * .
        if(ch == 127)
        {
            i--;
            if(result.size() > 1) result = result.substr(0, result.length()-1);
            mvwprintw(mWindow, 3, i+5, " ");
            wmove(mWindow, 3, i+5);
        }
        else
        {
            result += (char)ch;
            if(isPass)mvwprintw(mWindow, 3, i+5, "*");
            else mvwprintw(mWindow, 3, i+5, "%c", ch);
            i++;
        }
    }
    return result;
}
/*!
\brief Destructor
*/
CInputWindow::~CInputWindow()
{
    ///Empty destructor to call parent constructor.
}

