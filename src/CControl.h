/** 
 * @file CControl.h
 * @brief Definition for the main Controlling class.
 * @author Jakub J Simon
 *
 */

#ifndef CCONTROL_H
#define	CCONTROL_H
#include "CFTP.h"
#include "CList.h"
#include "CPanel.h"
#include "CQueue.h"
#include <pthread.h>
#include <semaphore.h>
#include "CButtonBar.h"
#include "Cerror.h"
using namespace std;

/*! \brief TPASS are the arguments passed to worker thread
*/
typedef struct TPass
{
    ///The Queue for getting request.
    CQeue * Queue;
    ///Dequeue mutex so that there is no race condition between GUI and Worker thread.
    pthread_mutex_t * deqeue_mutex;
    ///Semaphore for blocking the Worker thread when there is no work to be done.
    sem_t *jobs_in_q;
}TPASS;

/*! \clas CControl.
    \brief This is the main Controlling clas which does the main loop.

*/
///CControl does handle errors and overall interaction with user through the ncurses gui. It handles login, parsing the FTP output and interaction with the layout, as well as passing requests to worker thread.
class CControl
{
public:
    void init();
    void splash();
    int login();
    void run();
    void testParse(string listing);
    void recursiveDownload(string dir, int index);
    void prepareLayout();
    ~CControl();
    
private:
    ///The main Window
    CPanel *left;
    ///List of Files
    CList rFiles;
    ///Member FTP connection
    CFTP ftp;
    ///List of Folders
    CList rFolders;
    ///The queue for saving request to be passed to worker thread
    CQeue * mQ;
    ///Arguments for worker
    TPASS * args;
    ///Strings that hold Server Adress, Username and Password
    string mSname, mUname, mPwd;
    ///Button bar
    CButtonBar * bar;
    ///Error window
    CError * err;
};


#endif	/* CCONTROL_H */

