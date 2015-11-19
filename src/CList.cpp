/** 
 * @file CList.cpp
 * @brief Implementation of CList Class
 * @author Jakub J Simon
 *
 */

#include "CList.h"
#include "CFile.h"

/*!
\brief Constructor
*/
CList::CList()
{
    ///Sets initial values of size 100 and mLast to 0
    mSize = 100;
    mPole = new CFolder * [mSize];
    mLast = 0;
}
/*!
\brief Destructor deallocates all members of the field
*/
CList::~CList()
{
    for(int i = 0; i < mLast; i++)
    {
        delete mPole[i];
    }
    delete [] mPole;
}
/*!
\brief Adds new file/folder
\param name Filename
\param size Filesize
\param path Path to Remote file
\return success
*/
int CList::add(const string& name, const int& size, const string& path)
{
    if(mLast == mSize) resize();
    if(size > 0) mPole[mLast++] = new CFile(name, path, size);
    else mPole[mLast++] = new CFolder(name, path);
    return 0;
}
/*!
\brief If field is full double it's size and copy everything
*/
void CList::resize()
{
    CFolder ** temp;
    mSize *= 2;
    temp = new CFolder * [mSize];
    for(int i = 0; i < mLast; i++) temp[i] = mPole [i];
    delete [] mPole;
    mPole = temp;
}
/*!
\brief Find file/folder by index
\return pointer to CFilder which is the resulting File/Folder
*/
CFolder * CList::find(const int& index) const
{
    if(index < mLast) return mPole[index];
    else return NULL;
}
/*!
\brief Find file/folder by name
\return pointer to CFilder which is the resulting File/Folder
*/
CFolder * CList::find(const string& name) const
{
    for(int i = 0; i < mLast; i++)
    {
        if(name == mPole[i]->getText()) return mPole[i];
    }
    return NULL;
}
/*!
\brief Clean all the contents
*/
void CList::clear()
{
    for(int i = 0; i < mLast; i++)
    {
        delete mPole[i];
    }
    mLast = 0;
}
/*!
\brief Copy all File/Folder entries to new list
\return reference to left side CList
*/
CList & CList::operator =(const CList& remote)
{
    
    if (&remote == this) return *this;
    this->clear();
    for(int i = 0; i < remote.mLast; i++)
    {
        this->add(remote.mPole[i]->getText(), remote.mPole[i]->getSize(), remote.mPole[i]->getPath());
    }
    return *this;
}
/*!
\brief Gets number of entries
\return Number of entries
*/
int CList::getSize()const
{
    return mLast;
}
