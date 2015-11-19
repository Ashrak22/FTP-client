/** 
 * @file CFile.h
 * @brief Definiton of CFile Class
 * @author Jakub J Simon
 *
 */


#ifndef CFILE_H
#define	CFILE_H
#include "CFolder.h"

/*!
\class CFile
\brief Data class to hold information about remote files
*/
class CFile:public CFolder
{
    public:
        CFile(string name, string path, int size);
        void setSize(int size);
        int getSize() const;
    private:
	///File size
        int mSize;
};


#endif	/* CFILE_H */

