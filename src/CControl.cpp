#include "CControl.h"
#include <ctime>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CSplash.h"
#include "CProgress.h"
#include "CInputWindow.h"
/** 
 * @file CControl.cpp
 * @brief Implementation of CControl Class
 * @author Jakub J Simon
 *
 */


/*!
\brief The worker thread.
\param args arguments for the Worker thread.

*/
///This thread handles everything around downloading and uploading files, ocasionally even creating folders. This function also holds and updates the Progress bar.
void * backroundWorker(TPASS * args) {
    CProgress * progressWin;
    TItem * temp;
    CFTP * ftp;
    progressWin = new CProgress(0, LINES - 4, COLS, 3);
    do {
        sem_wait(args->jobs_in_q);
        pthread_mutex_lock(args->deqeue_mutex);
        temp = args->Queue->deqeue();
        pthread_mutex_unlock(args->deqeue_mutex);
        if (temp->type == -1) break;
        ftp = new CFTP;
        ftp->connect(temp->serverName);
        ftp->login(temp->uname, temp->pwd);
	///Request type 0 -> download file fileName
        if (temp->type == 0) {
            ftp->changeDirectory(temp->path);
            ftp->downloadFile(temp->fileName, temp->fileName, temp->size, progressWin);
            delete temp;
            delete ftp;
	///Request type 2 -> Create dir fileName and change working dir to that dir
        } else if (temp->type == 2) {

            mkdir(temp->fileName.c_str(), S_IRWXU | S_IRWXG);
            chdir(temp->fileName.c_str());
            delete temp;
            delete ftp;
	///Request type 3 -> Change working dir to parent dir
        } else if (temp->type == 3)
        {
            chdir("..");
            delete temp;
            delete ftp;
        }
	///Request type 6 -> Upload file
        else if(temp->type == 6)
        {
            ftp->changeDirectory(temp->currPath);
            ftp->uploadFile(temp->path, temp->fileName, progressWin);
            delete temp;
            delete ftp;
        }
        progressWin->redraw("No download in progress", 0, 0);
    } while (1);
    delete progressWin;
    delete temp;
    return (NULL);
}
/*!
\brief Init the GUI.

*/
///Inits the GUI, first starts the ncurses subsystem, then disable line buffering, disables the echoing of typed characters. Afterwards changes GUI to color mode and inits color pairs. In the end it creates the GUI elements such as the main panel and Button Bar and starts the Splashscreen.
void CControl::init() {
    initscr(); /*Start curses*/
    raw(); /*Disable Line buffering*/
    noecho(); /*Do not echo characters*/
    keypad(stdscr, TRUE); /*Use F1, F2, ....*/
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal doesn't support colors\n");
        exit(1);
    }
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_YELLOW, COLOR_CYAN);
    init_pair(3, COLOR_BLACK, COLOR_RED);
    init_pair(4, COLOR_WHITE, COLOR_CYAN);
    init_pair(5, COLOR_WHITE, COLOR_YELLOW);
    init_pair(6, COLOR_BLACK, COLOR_WHITE);
    init_pair(7, COLOR_RED, COLOR_WHITE);
    left = new CPanel(0, 0, COLS, LINES - 4);
    splash(); /*Start with splashscreen*/
    prepareLayout();
    bar = new CButtonBar;
    err = new CError;

}
/*!
\brief Draw the screen layout
*/
void CControl::prepareLayout() {
    left->redraw();
}
/*!
\brief Draw Splaxh screen and wait 3 seconds
*/
void CControl::splash() {
    CSplash splash((COLS - 49) / 2, (LINES - 14) / 2, 49, 14);
    sleep(3);
}
/*!
\brief The main program loop.

*/
///This method handles the overall interaction with user and dispatches calls to other clases and evaluates exceptions and return values
void CControl::run() {
    int ch;
    CFolder * temp;

    pthread_t thrID;
    pthread_attr_t attr;
    ///Creates the Worker thread
    TItem * qItem;
    mQ = new CQeue();
    args = new TPASS();
    args->deqeue_mutex = new pthread_mutex_t;
    pthread_mutex_init(args->deqeue_mutex, NULL);
    args->Queue = mQ;
    args->jobs_in_q = new sem_t();
    sem_init(args->jobs_in_q, 0, 0);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    if (pthread_create(&thrID, &attr, (void*(*)(void*)) backroundWorker, (void*) args)) {
        perror("pthread_create");
    }
    testParse(ftp.listDir());
    left->redraw(rFolders, rFiles);
    while (1) {
        ch = wgetch(left->getWindow());
        ///Up arrow moves the highlight one position up
        if (ch == KEY_UP) left->keyUp(rFolders, rFiles);
	///Down arrow moves the highlight one position down
        else if (ch == KEY_DOWN) left->keyDown(rFolders, rFiles);
	///For enter key either change directory or download file (depends on highlighted item)
        else if ((char) ch == '\n') {
            int pos = left->getPosition();
            if (pos <= rFolders.getSize() - 1) {
                temp = rFolders.find(pos);
                try {
                    ftp.changeDirectory(temp->getText());
                } catch (const char * param) {
                    err->redraw(param);
                    login();
                    ftp.changeDirectory(temp->getPath() + "/" + temp->getText());
                }

                left->clearPosition();
                rFolders.clear();
                rFiles.clear();
                testParse(ftp.listDir());
                left->redraw(rFolders, rFiles);

            } else {
                qItem = new TItem;
                temp = rFiles.find(pos - rFolders.getSize());
                qItem->pwd = mPwd;
                qItem->uname = mUname;
                qItem->serverName = mSname;
                qItem->type = 0;
                qItem->fileName = temp->getText();
                qItem->path = temp->getPath();
                qItem->size = temp->getSize();
                pthread_mutex_lock(args->deqeue_mutex);
                mQ->enqeue(qItem);
                pthread_mutex_unlock(args->deqeue_mutex);
                sem_post(args->jobs_in_q);
            }
	///F12 key ends the programme, by giving -1 request to worker thread and breaking the loop
        } else if (ch == KEY_F(12)) {
            qItem = new TItem;
            qItem->type = -1;
            pthread_mutex_lock(args->deqeue_mutex);
            mQ->enqeue(qItem);
            pthread_mutex_unlock(args->deqeue_mutex);
            sem_post(args->jobs_in_q);
            break;
	///F3 key changes the local working directory
        } else if (ch == KEY_F(3)) {
            CInputWindow * input;
            input = new CInputWindow("New working Directory (absolute path):");
            chdir(input->getResponse(false).c_str());
            delete input;
            left->redraw(rFolders, rFiles);
	///F5 key enqueues reursive download of current folder
        } else if (ch == KEY_F(5)) {
            recursiveDownload(ftp.currentDir(), 0);
            rFiles.clear();
            rFolders.clear();
            testParse(ftp.listDir());
            left->redraw();
	///F8 Creates new directory on FTP Server
        } else if (ch == KEY_F(8)) {
            CInputWindow * input;
            input = new CInputWindow("New remote directory: ");

            try {
                ftp.createDir(input->getResponse(false));
            } catch (const char * param) {
                err->redraw(param);
                ftp.createDir(input->getResponse(false));

            }
            rFiles.clear();
            rFolders.clear();
            testParse(ftp.listDir());
            delete input;
            left->redraw(rFolders, rFiles);
	///F9 key deletes remote File, it ignores folders
        } else if (ch == KEY_F(9)) {
            int pos = left->getPosition();
            if (pos < rFolders.getSize()) continue;
            temp = rFiles.find(pos - rFolders.getSize());
            try {
                ftp.deleteFile(temp->getText());
            } catch (const char * param) {
                err->redraw(param);
                login();
                ftp.deleteFile(temp->getText());
            }
            rFiles.clear();
            rFolders.clear();
            testParse(ftp.listDir());
            left->redraw(rFolders, rFiles);
	///F7 uploads a file, which is requested from a user as absolute path to that file.
        } else if(ch == KEY_F(7)) {
            CInputWindow input("Absolute path to file:");
            qItem = new TItem;
            qItem->path = input.getResponse(false);
            fstream m_file;
            m_file.open(qItem->path.c_str(), ios_base::in);
            if(m_file.tellg() == -1)
            {
                err->redraw("File does not exist!");
                
                m_file.close();
                delete qItem;
                left->redraw(rFolders, rFiles);
                continue;
            }
            m_file.close();
            while(1)
            {
                input.redraw("Upload as:");
                qItem->fileName = input.getResponse(false);
                if(rFiles.find(qItem->fileName) != NULL)
                {
                    err->redraw("File already exists on FTP!");
                    continue;
                }
                break;
            }
            qItem->serverName = mSname;
            qItem->pwd = mPwd;
            qItem->uname = mUname;
            qItem->type = 6;
            qItem->currPath = ftp.currentDir();
            pthread_mutex_lock(args->deqeue_mutex);
            mQ->enqeue(qItem);
            pthread_mutex_unlock(args->deqeue_mutex);
            sem_post(args->jobs_in_q);
            left->redraw(rFolders, rFiles);
        }
    }
    pthread_join(thrID, NULL);
    pthread_attr_destroy(&attr);

    delete mQ;
    pthread_mutex_destroy(args->deqeue_mutex);
    delete args->deqeue_mutex;
    sem_destroy(args->jobs_in_q);
    delete args->jobs_in_q;
    delete args;
    delete left;
    delete bar;
    delete err;

}
/*!
\brief Parses the listing obtained from server.
\param listing the obtained listing.

*/
///It parses the listing by finding the appropriate column for name and size and determining the type by first character. File (-) or Folder(d).
void CControl::testParse(string listing) {
    string pole[500], name, path;
    unsigned int i = 0, start = 0, konec, size;
    while (1) {
        konec = listing.find('\r', start);
        pole[i++] = listing.substr(start, konec - start);
        start = konec + 2;
        if (start >= listing.length() - 1) break;
    }
    path = ftp.currentDir();
    rFolders.add(".", 0, "");
    for (unsigned int j = 1; j < i; j++) {
        int c = 0;
        size = 0;

        for (unsigned int k = 0; k < pole[j].length() - 1; k++) {
            if (pole[j][k] >= '0' && pole[j][k] <= '9' && c == 0) c++;
	   /// parse the size
            else if (pole[j][k] >= '0' && pole[j][k] <= '9' && c == 1) {
                size = (pole[j][k] - '0');
                c++;
            } else if (pole[j][k] >= '0' && pole[j][k] <= '9' && c == 2) {
                size = size * 10;
                size += (pole[j][k] - '0');
            } else if (pole[j][k] >= 'A' && pole[j][k] <= 'Z' && c == 2) {
                c++;
            } else if (pole[j][k] >= '0' && pole[j][k] <= '9' && c == 3) {
                c++;
            } else if ((c == 4 || c == 6) && pole[j][k] == ' ') {
                c++;
            } else if (pole[j][k] >= '0' && pole[j][k] <= '9' && c == 5) {
                c++;
	    ///parse name
            } else if (c == 7) {
                start = k;
                break;
            }
        }
        name = pole[j].substr(start);
        if (pole[j][0] != 'd') {
            rFiles.add(name, size, path);
        } else {
            rFolders.add(name, 0, path);
        }
    }
}
/*!
\brief Prepare recursive download into the queue.
\param dir The directory it should download files from.
\param index index to determine if this is the first function.

*/
///Downloads all files in the folder and the calls itself for all folders.
void CControl::recursiveDownload(string dir, int index) {
    CFolder * tmp;
    CList tmpFold;
    TItem * qItem;
    int i = 0;
    rFiles.clear();
    rFolders.clear();
    ftp.changeDirectory(dir);
    testParse(ftp.listDir());
    tmpFold = rFolders;
    tmp = rFiles.find(i++);
	///Enqueue all files
    while (tmp != NULL) {
        qItem = new TItem;
        qItem->pwd = mPwd;
        qItem->uname = mUname;
        qItem->serverName = mSname;
        qItem->type = 0;
        qItem->fileName = tmp->getText();
        qItem->path = tmp->getPath();
        qItem->size = tmp->getSize();
        pthread_mutex_lock(args->deqeue_mutex);
        mQ->enqeue(qItem);
        pthread_mutex_unlock(args->deqeue_mutex);
        sem_post(args->jobs_in_q);
        tmp = rFiles.find(i++);
    }
    i = 2;

    tmp = tmpFold.find(i++);
    while (tmp != NULL) {
	///Enqueue create local dir and change into it
        qItem = new TItem;
        qItem->pwd = mPwd;
        qItem->uname = mUname;
        qItem->serverName = mSname;
        qItem->type = 2;
        qItem->fileName = tmp->getText();
        pthread_mutex_lock(args->deqeue_mutex);
        mQ->enqeue(qItem);
        pthread_mutex_unlock(args->deqeue_mutex);
        sem_post(args->jobs_in_q);
        recursiveDownload(tmp->getText(), index+1);
	///After a directory is fully completed change to local parent directory
        qItem = new TItem;
        qItem->pwd = mPwd;
        qItem->uname = mUname;
        qItem->serverName = mSname;
        qItem->type = 3;
        pthread_mutex_lock(args->deqeue_mutex);
        mQ->enqeue(qItem);
        pthread_mutex_unlock(args->deqeue_mutex);
        sem_post(args->jobs_in_q);
        tmp = tmpFold.find(i++);
    }
    ///if not the first function call, change to parent remote directory
    if (index != 0) ftp.changeDirectory("..");
}
/*!
\brief Handles connection and login
*/
int CControl::login() {
    CInputWindow tmp("Server address:");
    ///Ask for adress until the connect succeds
    while (1) {
        tmp.redraw("Server Address: ");
        mSname = tmp.getResponse(false);
        try {
            ftp.connect(mSname);
        } catch (const char* e) {
            err->redraw(e);
            continue;
        }
        break;
    }
    ///Ask for username and password until logon succeds
    while (1) {
        tmp.redraw("Username:");
        mUname = tmp.getResponse(false);
        tmp.redraw("Password:");
        mPwd = tmp.getResponse(true);

        try {
            ftp.login(mUname, mPwd);
        } catch (const char * e) {
            err->redraw(e);
            continue;
        }
        break;
    }
    return 0;
}
/*!
\brief Destructor

*/
///End ncurses mode
CControl::~CControl() {
    endwin();
}
