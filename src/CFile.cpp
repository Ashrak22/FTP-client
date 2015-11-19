/** 
 * @file CFile.cpp
 * @brief Implementation of CFile Class
 * @author Jakub J Simon
 *
 */
#include "CFile.h"
/*!
\param name File name
\param path path to file on FTP server
\param size File size
\brief Calls the parent constructor and sets the file size
*/
CFile::CFile(string name, string path, int size):CFolder(name, path)
{
    mSize = size;
}
/*!
\return filesize
\brief Getter for filesize
*/
int CFile::getSize() const
{
    return mSize;
}
/*!
\param size Filesize
\brief setter for filesize
*/
void CFile::setSize(int size)
{
    mSize = size;
}
