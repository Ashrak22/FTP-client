/** 
 * @file CNetwork.h
 * @brief Definiton of CNetwork Class
 * @author Jakub J Simon
 *
 */

#ifndef CNETWORK_H
#define	CNETWORK_H
#include <sys/socket.h>
#include <netinet/in.h>     // IPv4 and IPv6
#include <unistd.h>         // gethostname()
#include <netdb.h>          // DNS - gethostbyname()
#include <arpa/inet.h>      // všechny inet_* funckce
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>


using namespace std;
/*!
\class CNetwork
\brief Network layer class 
 */

class CNetwork
{
///CNetwork is meant to provide encapsulation to difficult C Socket Library. This class is derived from my Homework for the BI-PSI course.
public:
    CNetwork();
    int init(short port, const char * address);
    void end();
    int send_data(int len, string & pBuf);
    int send_data(int len, unsigned char * pBuf);
    int receive_data(int len, string & pBuf);
    int receive_data(int len, unsigned char *& pBuf);
private:
    ///The communication socket itself
    int sock;
    ///Structure holding information about remote adress, port, etc in network endianity
    struct sockaddr_in sin;
    ///Structure fro DNS Resolution
    struct hostent *hp;
    ///Number of port to conect in PC endianity
    short port_number;
};


#endif	/* CNETWORK_H */

