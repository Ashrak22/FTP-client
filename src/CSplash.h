/** 
 * @file CSplash.h
 * @brief Definiton of CSplash Class
 * @author Jakub J Simon
 *
 */
#ifndef CSPLASH_H
#define	CSPLASH_H
#include "CGUIItem.h"

/*!
\class CSplash
\brief Shows splashscreen
*/
class CSplash : public CGUIItem
{
    public:
        CSplash(int x, int y, int width, int height);
        void redraw();
        virtual ~CSplash();
};


#endif
