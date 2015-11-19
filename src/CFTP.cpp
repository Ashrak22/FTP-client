/** 
 * @file CFTP.cpp
 * @brief Implementation of CFTP Class
 * @author Jakub J Simon
 *
 */
#include "CFTP.h"
#include <iostream>
/*!
  \def BUFF_SIZE 5000
  Hold the size of the upload buffer
*/
#define BUFF_SIZE 5000

/*!
\brief Connects to server
\param address The address of the server, either IP or DNS name
\return connection succesfull
*/
bool CFTP::connect(string address) {
    string buffer;
    ///Calls active connection to inits itsefl to address and port 21
    active.init(21, address.c_str());
    ///The reads the Welcome message from server
    active.receive_data(500, buffer);
    return true;
}
/*!
\brief Logs the user in
\param username The username
\param password User's password
\exception "Wrong username/password"
\return connection succesfull
*/
bool CFTP::login(string username, string password) {
    string buffer;
    ///First we need to send USER username \r\n command
    buffer = "USER " + username + "\r\n";
    active.send_data(buffer.length(), buffer);
    active.receive_data(500, buffer);
    ///After we receive answer (prompt for password) we send PASS password \r\n command. FTP is plain-text therefore unsecure.
    buffer = "PASS " + password + "\r\n";
    active.send_data(buffer.length(), buffer);
    active.receive_data(500, buffer);
    ///If answer to PASS command has 530 (Bad password/username) throw exception which is handled in CControl::login()
    if (buffer.find("530") != buffer.npos) {
        throw "Wrong username/password";
    }
    return true;
}
/*!
\brief Gets the listing
\return listing obtained from server
*/
string CFTP::listDir() {
    int len = 0;
    ///First we need to open passive connection
    passiveMode();
    ///Then we send LIST\r\n command
    string buffer = "LIST\r\n", buf = "";
    active.send_data(buffer.length(), buffer);
    active.receive_data(500, buffer);
    ///The Listing itself arrives on passive port
    while (len != -1) {
        len = passive.receive_data(500, buffer);
        buf += buffer;
        buffer.clear();
    }
    buf.push_back('\n');
    active.receive_data(500, buffer);
    passive.end();
    return buf;
}
/*!
\brief Calculates the port number from PASV confirmation
\param buffer PASV confirmation
\return the port in binary form
*/
int CFTP::getPort(string buffer) {
    int res = 0, position = 27, i = 0;
    string number;
    ///Get rid of IP adress which we got earlier
    while (i != 4) {
        if (buffer[position++] == ',') i++;
    }
    ///Read first number which has to be the multiplied by 256
    while (buffer[position] != ',') {
        number += buffer[position++];
    }
    position++;
    res = 256 * atoi(number.c_str());
    number = "";
    ///Reading second number for port
    while (buffer[position] != ')') {
        number += buffer[position++];
    }
    ///Finally we need to add those two numbers to get the port server is listening on
    res += atoi(number.c_str());
    return res;
}
/*!
\brief Calculates the IP address from PASV confirmation
\param buffer PASV confirmation
\return IP address as string
*/
string CFTP::getAddress(string buffer) {
    string res;
    int position = 27;
    ///Reading all octets and exchange the comma for dot
    for (int j = 0; j < 3; j++) {
        while (buffer[position] != ',') {
            res += buffer[position++];
        }
        res += '.';
        position++;
    }
    while (buffer[position] != ',') {
        res += buffer[position++];
    }
    return res;
}
/*!
\brief Initiates passive connection
\return Succes of the operation
*/
int CFTP::passiveMode() {
    ///We need to ope passive connection due to NAT and private addressing. Usually the client would connect to server on port 21 and would bind itself on port 20 (data). But due to IPv4 address depletion and use of private addressing with NAT is the clients IP adress usually different from it's public IP. So server instead of connecting to client directly, starts to listen to a port on it's side which is the passed as PASV confirmation as follows PASV ready (a1, a2, a3, a4, p1, p2). a1 - a4 are octets of the servers IP, p1-p2 specify the the port on which the server listens, the port is calculated as follows: p1*256+p2
    string buffer = "PASV\r\n";
    active.send_data(buffer.length(), buffer);
    active.receive_data(500, buffer);
    passive.init(getPort(buffer), getAddress(buffer).c_str());
    return 0;
}
/*!
\brief Downloads a file
\param file Remote file name.
\param dst Local file name.
\param size The file size in Bytes
\param porogressWin The ProgressBar to be updated
*/
void CFTP::downloadFile(string file, string dst, int size, CProgress * progressWin) {
    fstream m_file;
    string buffer;
    int length = 0, curr_length, maxl = size, i = 0;
    float percent;
    double speed;
    time_t start, end;
    unsigned char * buf;
    ///First open and create destination file
    m_file.open(dst.c_str(), ios::out | ios::trunc);
    passiveMode();
    ///Open passive connection a send RETR filename\r\n command on active connection
    buffer = "RETR " + file + "\r\n";
    active.send_data(buffer.length(), buffer);
    ///Get answer code 150
    active.receive_data(500, buffer);
    ///Then start the clock to calculate speed
    time(&start);
    while (length < maxl) {
	///In while cycle read data from socket and write them to file
        curr_length = passive.receive_data(65536, buf);
        length += curr_length;
        m_file.write((const char *) buf, curr_length);
	///Every 100th recalculate speed and percentage and update progressbar
        if (i == 100) {
            i = 0;
            time(&end);
            speed = difftime(end, start);
            speed = (length / speed) / 1024;
            percent = ((float) (length) / size)*100;
            progressWin->redraw(file, speed, (int) percent);
        } else i++;
        delete [] buf;
    }
    m_file.close();
    passive.end();
    ///Receive Transfer finished message
    active.receive_data(500, buffer);  
}
/*!
\brief Gets the current remote working
\return path to and name of the current working dir
*/
string CFTP::currentDir() {
    string buffer;
    int start, end;
    ///FTP uses traditional PWD command to get the current working dir
    buffer = "PWD\r\n";
    active.send_data(buffer.length(), buffer);
    active.receive_data(200, buffer);
    ///The directory needs to be parsed out from the the answer, it starts and ends with a " character
    start = buffer.find('\"');
    end = buffer.find('\"', start + 1);
    buffer = buffer.substr(start + 1, end - (start + 1));
    return buffer;
}
/*!
\brief Changes remote working directory
\param dirNameabsolute or relative path or name of the folder
\exception "Not logged in" Throw when the remote server reaches timeout and loggs user out
*/
void CFTP::changeDirectory(string dirName) {
    string buffer;
    ///The command is CWD dirName\r\n
    buffer = "CWD " + dirName + "\r\n";
    active.send_data(buffer.length(), buffer);
    ///If receive_data returns -1, which means remote socket closed throw "Not logged in" exception
    if (active.receive_data(200, buffer) == -1) {
        throw "Not Logged in";
    }
}
/*!
\brief Creates new directory
\param name name of dir to be create
\exception "Not logged in" Throw when the remote server reaches timeout and loggs user out
\exception "Directory exists" Cannot create the folder as it exists already
\return Success
*/
int CFTP::createDir(string name) {
    string buffer;
    ///The command is CWD dirName\r\n
    buffer = "MKD " + name + "\r\n";
    active.send_data(buffer.length(), buffer);
    ///If receive_data returns -1, which means remote socket closed throw "Not logged in" exception
    if (active.receive_data(200, buffer) == -1) {
        throw "Not Logged in";

    }
    ///If server returns code 521, throw "Directory exists" exception
    if (buffer.find("521") != buffer.npos) throw "Directory exists";
    else return 0;
}
/*!
\brief Deletes remote file
\param name name of file to be deleted
\exception "Not logged in" Throw when the remote server reaches timeout and loggs user out
*/
void CFTP::deleteFile(string name) {
    string buffer;
    ///The command is DELE dirName\r\n
    buffer = "DELE " + name + "\r\n";
    active.send_data(buffer.length(), buffer);
    ///If receive_data returns -1, which means remote socket closed throw "Not logged in" exception
    if (active.receive_data(200, buffer) == -1) {
        throw "Not Logged in";

    }
}
/*!
\brief Uploads a file
\param file absolute path to local file.
\param dst Remote filename.
\param porogressWin The ProgressBar to be updated
*/
void CFTP::uploadFile(string dile, string dst, CProgress* progressWin) {
    fstream m_file;
    string buffer;
    int length = 0, curr_length, i = 0, size, maxl;

    float percent;
    double speed;
    time_t start, end;
    char * buf;
    buf = new char [BUFF_SIZE];

    ///First determine local file size
    m_file.open(dile.c_str(), ios::in | ios::binary);
    size = m_file.tellg();
    m_file.seekg(0, ios_base::end);
    maxl = m_file.tellg();
    size = maxl - size;
    m_file.seekg(ios_base::beg);
    passiveMode();
    ///Because of reasons above open passive connection and send STOR dstFileName\r\n command
    buffer = "STOR " + dst + "\r\n";
    active.send_data(buffer.length(), buffer);
    active.receive_data(500, buffer);
    time(&start);
    maxl = size;
    while (1) {
	///In while cycle read data from file and write them to socket
        memset(buf, 0, BUFF_SIZE);

        m_file.read(buf, BUFF_SIZE);

        curr_length = passive.send_data(size < BUFF_SIZE ? size : BUFF_SIZE, (unsigned char *) buf);
        size = size - BUFF_SIZE;
        length += curr_length;
	///Every 50th recalculate speed and percentage and update progressbar
        if (i == 50) {
            i = 0;
            time(&end);
            speed = difftime(end, start);
            speed = (length / speed) / 1024;
            percent = ((float) (length) / maxl)*100;
            progressWin->redraw(dst, speed, (int) percent);

        } else i++;
        if (size <= 0) break;
    }
    m_file.close();
    delete [] buf;
    passive.end();
    active.receive_data(500, buffer);
}
