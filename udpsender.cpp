#include "udpsender.h"


udpSender::udpSender(int port, string address)
{
    this->port = port;
    this->address = address;
    int trueflag = 1;

    this->sock = socket(AF_INET, SOCK_DGRAM, 0);


    if(this->sock < 0){
        cerr << "Unable to create UDP socket" << endl;
    }
    if(setsockopt(this->sock, SOL_SOCKET, SO_BROADCAST,
                       &trueflag, sizeof trueflag) < 0){
        cerr << "Unable to set broadcast socket" << endl;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(address.c_str());
    addr.sin_port = htons(port);

}

void udpSender::send(uint8_t *message, uint len)
{
    int nbytes = sendto(sock, message, len, 0, (struct sockaddr*) &addr, sizeof(addr) );
    if (nbytes < 0) {
        cerr << "Unable to send UDP message" << endl;
    }
}

void udpSender::send(MicroMessage *msg)
{
    uint8_t size;
    uint8_t *bytes = msg->getBytes(&size);
    send(bytes, size);
    delete[] bytes;
}

void udpSender::test()
{
    string message = "Hello World!!";
    cout << "Sending \"" << message << "\"" << endl;
    send((uint8_t *)message.c_str(), message.length());
    cout << "Done." << endl;
}
