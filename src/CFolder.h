/** 
 * @file CFolder.h
 * @brief Definiton of CFolder Class
 * @author Jakub J Simon
 *
 */


#ifndef CFOLDER_H
#define	CFOLDER_H
#include <string>
using namespace std;

/*!
\class CFolder
\brief Data class to hold information about remote folders
*/
class CFolder
{
public:
    CFolder();
    CFolder(const string & name, const string & path);
    void setText(const string & text);
    void setPath(const string & text);
    virtual void setSize(int size);
    virtual int getSize() const;
    string getText() const;
    string getPath() const;
    virtual ~CFolder();
protected:
    ///Filename
    string mName;
    ///Path to File on Remote server
    string mPath;
};


#endif	/* CFILE_H */

