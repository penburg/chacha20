#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "micromessage.h"

using namespace std;

class udpSender
{
public:
    udpSender(int port, string address);
    void send(uint8_t * message, uint len);
    void send(MicroMessage *msg);
    void test();

private:
    int port;
    string address;
    int sock;
    struct sockaddr_in addr;
};

#endif // UDPSENDER_H
