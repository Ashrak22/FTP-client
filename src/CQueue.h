/** 
 * @file CQueue.h
 * @brief Definiton of CQeue Class
 * @author Jakub J Simon
 *
 */

#ifndef CQUEUE_H
#define	CQUEUE_H
#include <string>
#include "CList.h"
using namespace std;
///Item to be passed to worker as a job
typedef struct TItem
{
    ///Username
    string uname;
    ///Password
    string pwd;
    ///Path to file on remote server
    string path;
    ///Filename on remote server
    string fileName;
    ///FileSize
    int size;
    ///Server address
    string serverName;
    ///local path
    string currPath;
    ///Request type
    int type;
} TITEM;

/*!
\class CQeue
\brief Queue to hold jobs for worker thread
*/
class CQeue
{
public:
    CQeue();
    ~CQeue();
    void enqeue(TITEM * p);
    TITEM * deqeue();
    bool isEmpty()const;
    
    
private:
    ///One member of the linked list
    struct TMEMBER
    {
	///The job
        TITEM * data;
        ///Previous a next member
        TMEMBER* mPrev, *mNext;
    };
    ///Start and end of the linked list
    TMEMBER *mStart, *mEnd;
};


#endif
