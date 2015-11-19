/** 
 * @file CProgress.h
 * @brief Definiton of CProgress Class
 * @author Jakub J Simon
 *
 */
#ifndef CPROGRESS_H
#define	CPROGRESS_H
#include "CGUIItem.h"
/*!
\class CProgress
\brief CProgress hold the definition of the Progress bar
*/
class CProgress : public CGUIItem
{
public:
    CProgress(int x, int y, int width, int height);
    void redraw(string name, int speed, int percent);
    ~CProgress();
};


#endif
