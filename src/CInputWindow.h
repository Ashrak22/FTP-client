/** 
 * @file CInputWindow.h
 * @brief Definiton of CInputWindow Class
 * @author Jakub J Simon
 *
 */

#ifndef CINPUTWINDOW_H
#define	CINPUTWINDOW_H

#include "CGUIItem.h"

/*!
\class CInputWindow
\brief Specyfing and handling of user inputs
*/
class CInputWindow : public CGUIItem
{
    public:
    CInputWindow(string caption);
    void redraw(string caption);
    string getResponse(bool isPass);
    ~CInputWindow();
};


#endif

