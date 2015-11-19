/** 
 * @file CFTP.h
 * @brief Definiton of CFTP Class
 * @author Jakub J Simon
 *
 */
#ifndef CFTP_H
#define	CFTP_H
#include "CNetwork.h"
#include <fstream>
#include "CProgress.h"

/*!
\class CFTP
\brief Class to shield all FTP stuff
*/
///CFTP provides interface to all relevant FTP functions, as connecting to server, logging in, Passive Mode, Listing Files, Download/Upload/Delete file, or Create new dir.
class CFTP
{
public:
    bool connect(string address);
    bool login(string username, string password);
    string listDir();
    void downloadFile(string dile, string dst, int size, CProgress * progressWin);
    void uploadFile(string dile, string dst, CProgress * progressWin);
    void changeDirectory(string dirName);
    string currentDir();
    int createDir(string name);
    void deleteFile(string name);
    
private:
    int passiveMode();
    int getPort(string buffer);
    string getAddress(string buffer);
    ///Holds network connection for so called active connection, also called the command.
    CNetwork active;
    ///Holds network connection for passive connection, more on Passive Mode at the respective method.
    CNetwork passive;
    
};


#endif	/* CFTP_H */

