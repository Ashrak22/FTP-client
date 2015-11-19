/** 
 * @file CButtonBar.h
 * @brief Header for CButtonBar class.
 * @author Jakub J Simon
 *
 * @date 15/5/2012
 */


#ifndef CBUTTONBAR_H
#define	CBUTTONBAR_H
#include "CGUIItem.h"

/*!\class CButtonBar.
   \brief CButtonBar hold the specification for teh Button Bar at the bottom of the screen.
*/
/// This clas basically does only the drawing, as the buttons aren't clickable and Key Input is handled by CControl class.
class CButtonBar : public CGUIItem {

public:

    CButtonBar();
    void redraw();
    ~CButtonBar();
};



#endif
