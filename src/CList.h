/** 
 * @file CList.h
 * @brief Definiton of CList Class
 * @author Jakub J Simon
 *
 */

#ifndef CLIST_H
#define	CLIST_H
#include <string>
#include "CFolder.h"
using namespace std;

/*!
\class CList
\brief Holds the list of Files/Folders
*/

class CList
{
///Due to polymorphism we can define one class for both Files and Folders. They are stored linearly in a field.
public:
    CList();
    ~CList();
    int add(const string & name, const int & size, const string & path);
    void clear();
    int getSize() const;
    CFolder * find(const string & name) const;
    CFolder * find(const int & index) const;
    CList & operator= (const CList & remote);
private:
    void resize();
     ///Size of the field
    int mSize;
    ///First empty position
    int mLast;
    ///The field itself
    CFolder ** mPole;
};



#endif	/* CLIST_H */

