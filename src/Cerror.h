/** 
 * @file Cerror.h
 * @brief Definiton of CError Class
 * @author Jakub J Simon
 *
 */


#ifndef CERROR_H
#define	CERROR_H
#include "CGUIItem.h"
/*!\class CError
\brief Displays error messages.


*/
///CError extends CGUIItem and is only for displaying error messages
class CError : public CGUIItem {
public:
    CError();
    void redraw(const char* caption);
    ~CError();
};

#endif
