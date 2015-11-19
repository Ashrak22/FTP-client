/** 
 * @file CError.cpp
 * @brief Implementation of CError Class
 * @author Jakub J Simon
 *
 */
#include "Cerror.h"

/*!
\brief Prepares new Window.

*/
///Prepares new Window which is exactly half the screen wide and 5 Lines high for future displaying of Errors in CControl
CError::CError() : CGUIItem(COLS / 4, 5, COLS / 2, 5) {

}
/*!
\brief Displays error message.
\param caption The text of the error message.

*/
///Displays the error text received in caption parameter and wait for the user to press any key.
void CError::redraw(const char * caption)
{
    wattron(mWindow, COLOR_PAIR(7));
    fillWindow();
    mvwprintw(mWindow, 2, 5, "%s", caption);
    mvwprintw(mWindow, 3, mXSize/2-13, "Press any key to continue");
    wgetch(mWindow);
}
/*!
\brief Destructor.

*/
CError::~CError()
{
 ///Empty destructor to call parent destructor.   
}
