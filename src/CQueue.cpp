/** 
 * @file CQueue.cpp
 * @brief Implementation of CQeue Class
 * @author Jakub J Simon
 *
 */
#include "CQueue.h"

/*!
\brief Initialize linked list
*/
CQeue::CQeue()
{
    mStart = NULL;
    mEnd = NULL;
}
/*!
\brief Add new job to end of the Queue
\param p New Job
*/
void CQeue::enqeue(TITEM * p)
{
    TMEMBER *temp;
    temp = new TMEMBER;
    temp->data = p;
    temp->mNext = NULL;
    if (mStart == NULL)
    {
        temp->mPrev = NULL;
        mStart = temp;
        mEnd = temp;
    }
    else
    {
        temp->mPrev = mEnd;
        mEnd->mNext = temp;
        mEnd = temp;
    }
}
/*!
\brief Get first job from Queue
\return New Job
*/
TITEM * CQeue::deqeue()
{
    TITEM * temp;
    temp = mStart->data;
    if(mStart == mEnd)
    {
        delete mStart;
        mStart = NULL;
        mEnd = NULL;
    }
    else
    {
        mStart = mStart->mNext;
        delete mStart->mPrev;
        mStart->mPrev = NULL;
    }
    return temp;
}
/*!
\brief Test if Queue is empty
\return true if is empty, false otherwise
*/
bool CQeue::isEmpty() const
{
    return mStart == NULL;
}
/*!
\brief Delete all pending jobs
\param p New Job
*/
CQeue::~CQeue()
{
    TItem * temp;
    while(!isEmpty())
    {
        temp = deqeue();
	delete temp;
    }
}
