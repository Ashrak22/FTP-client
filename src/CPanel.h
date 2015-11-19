/** 
 * @file CPanel.h
 * @brief Definiton of CPanel Class
 * @author Jakub J Simon
 *
 */

#ifndef CPANEL_H
#define	CPANEL_H
#include "CGUIItem.h"
#include "CList.h"
/*!
\class CPanel
\brief Creates the main Panel for showing files
*/
class CPanel: public CGUIItem
{
public:
    CPanel(int x, int y, int width, int height);
    void redraw();
    void redraw(const CList & folders, const CList & files);
    void keyDown(const CList & folders, const CList & files);
    void keyUp(const CList & folders, const CList & files);
    WINDOW * getWindow() const;
    int getPosition() const;
    void clearPosition();
private:
    ///Index of currently higlighted item
    int highlighted;
    ///How many item on top are over the edge
    int offset;
};


#endif	/* CPANEL_H */

