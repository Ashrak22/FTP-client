/** 
 * @file CFolder.cpp
 * @brief Implementation of CFolder Class
 * @author Jakub J Simon
 *
 */


#include "CFolder.h"
/*!
\param name Filename
\param path Path to file on FTP server
\brief stes the filename and path
*/
CFolder::CFolder(const string & name, const string & path)
{
    mName = name;
    mPath = path;
}
/*!
\return Name of file
\brief getter for filename
*/
string CFolder::getText() const
{
    return mName;
}
/*!
\return Path to file
\brief getter for path
*/
string CFolder::getPath() const
{ 
    return mPath;
}
/*!
\param text Filename
\brief setter for filename
*/
void CFolder::setText(const string & text)
{
    mName = text;
}
/*!
\param text Path to file
\brief setter for path
*/
void CFolder::setPath(const string & text)
{
    mPath = text;
}
/*!
\brief Destructor
*/
CFolder::~CFolder()
{
    
}
/*!
\return Files size
\brief virtual getter for size
*/
int CFolder::getSize() const
{
    ///It is for use in child classes, as we don't care about size for folders, we return 0
    return 0;
}
/*!
\param text Filesize
\brief virtual setter for filesize
*/
void CFolder::setSize(int size)
{
    ///We do not need to set the size for Folder, therefore blank
}
