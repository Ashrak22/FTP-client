/** 
 * @file CNetwork.cpp
 * @brief Implementation of CNetwork Class
 * @author Jakub J Simon
 *
 */

#include "CNetwork.h"
#include <iostream>
/*!
\brief Set socket and port to default
*/
CNetwork::CNetwork() {
    sock = 0;
    port_number = 0;
}
/*!
\brief Inits connection to remote socket
\param port Port number
\param adress Remote address either as IP address or DNS name
\return Success
\exception "Wrong server" Thrown if the user types in incorrect server adress
*/
int CNetwork::init(short port, const char * address) {
    ///Create socket as filedescriptor and specify it as TCP socket. 
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("Can't create socket'");
        exit(1);
    }
    memset((void *) &sin, 0, sizeof (sin));
    ///Translate DNS into IP
    if (address[0] >= '1' && address[0] <= '9') sin.sin_addr.s_addr = inet_addr(address);
    else {
        hp = gethostbyname(address);
        if(hp == NULL)
        {
            throw "Wrong server";
        }
        memcpy(&sin.sin_addr.s_addr, hp -> h_addr_list[0],
                hp -> h_length);
    }
    port_number = port;
    
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    ///Connect to Remote server
    if (connect(sock, (struct sockaddr *) &sin, sizeof (sin)) == -1) {
        throw "Wrong server name.";
    }
    return 0;
}
/*!
\brief Sends data to Remote server
\param len The length of data in Bytes
\param pBuf Text data to be sent
\return Success
*/
int CNetwork::send_data(int len, string & pBuf) {
    if (write(sock, pBuf.c_str(), len) != len) {
        perror("Error while writing into socket");
        return -1;
    } else return len;
}
/*!
\brief Sends data to Remote server
\param len The length of data in Bytes
\param pBuf Binary data to be sent
\return length of data really sent
*/
int CNetwork::send_data(int len, unsigned char * pBuf) {
    if (write(sock, pBuf, len) != len) {
        perror("Error while writing into socket");
        return -1;
    } else return len;
}

/*!
\brief Receives data from Remote server
\param len The length of data we can maximaly receive
\param pBuf Text data to be received
\return Number of Bytes received
*/
int CNetwork::receive_data(int len, string & pBuf) {
    int length;
    unsigned char * buf;
    buf = new unsigned char[len+1];
    memset(buf, 0, len+1);
    if ((length = read(sock, buf, len)) <= 0) {
        delete [] buf;
        return -1;
    }
    
    pBuf = (const char *)buf;
    delete [] buf;
    return length;
}
/*!
\brief Receives data from Remote server
\param len The length of data we can maximaly receive
\param pBuf Binary data to be received
\return Number of Bytes received
*/
int CNetwork::receive_data(int len, unsigned char *& pBuf) {
    int length;
    pBuf = new unsigned char[len];
    memset(pBuf, 0, len);
    if ((length = read(sock, pBuf, len)) <= 0) {
        perror("Error while reading from socket");
        return -1;
    }
    return length;
}
/*!
\brief Close the socket
*/
void CNetwork::end() {
    close(sock);
}
