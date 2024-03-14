#include "chacha20.cpp"
#include "udpsender.h"
#include "micromessage.h"
#include "dhcpmessage.h"

#include <iostream>
#include <string>
#include <fstream>
#include <time.h>

using namespace std;

void showHelp(string commandName);

string VERSION = "v0.1.0";

int main(int argc, char *argv[]){

    int port = 9000;
    string address = "172.20.0.255";
    string password = "123456";

    Chacha20 chacha;
    udpSender sender(port, address);
    bool test = false;
    bool help = false;
    bool multicastTest = false;
    bool messageTest = false;

    for(int i = 1; i < argc; i++){
        if(string(argv[i]).compare("--testVectors") == 0){
            test = true;
        }
        else if(string(argv[i]).compare("--help") == 0){
            help = true;
        }
        else if(string(argv[i]).compare("--testMulticast") == 0){
            multicastTest = true;
        }
        else if(string(argv[i]).compare("--testMessage") == 0){
            messageTest = true;
        }
    }


    if(test){
        chacha.test();
    }
    else if(help){
        showHelp(argv[0]);
    }
    else if(multicastTest){
        MicroMessage mm = MicroMessage::getTestMessage();
        int cTextSize;
        uint8_t pTextSize;
        uint8_t *pText = mm.getBytes(&pTextSize);
        uint8_t *cText = chacha.encrypt(password, pText, pTextSize, &cTextSize);
        sender.send(cText, cTextSize);
        delete[] pText;
        delete[] cText;
    }
    else if(messageTest){
        MicroMessage::test();
    }
    else if(argc == 4 || argc == 5){
        string cmd = argv[1];
        string mac = argv[2];
        string ip = argv[3];
        string host = "";
        if(argc == 5){
            host = argv[4];
        }
        uint8_t uid[6] = {0xde, 0xad, 0xbe, 0xef, 0xca, 0xfe};

        uint8_t mid = time(NULL) % 0xff;

        DHCPMessage mm(uid, mid);

        mm.init(cmd, mac, ip, host);

        const char* iface = getenv("DNSMASQ_INTERFACE");
        const char* remain = getenv("DNSMASQ_TIME_REMAINING");

        if(iface != nullptr){
            mm.setInterface(iface);
            string macAddress;
            string sysFSFile = "/sys/class/net/" + string(iface) + "/address";
            ifstream macStream(sysFSFile);
            if(macStream.is_open()){
                if(getline(macStream, macAddress)){
                    uint8_t *mac = mm.stringToMac(macAddress);
                    ByteTools::arrayCopy(mac, 0, (uint8_t*)&uid, 0, 6);
                    delete[] mac;
                }
                macStream.close();
            }
        }
        if(remain != nullptr){
            mm.setRemaining(remain);
        }


        int cTextSize;
        uint8_t pTextSize;
        uint8_t *pText = mm.getBytes(&pTextSize);
        uint8_t *cText = chacha.encrypt(password, pText, pTextSize, &cTextSize);
        sender.send(cText, cTextSize);


        delete[] pText;
        delete[] cText;

    }

    return 0;
}

void showHelp(string commandName){
    cout << "ChaCha20 DCHP Message multicast sender " << VERSION << endl;
    cout << endl;
    cout << "Useage:" << endl;
    cout << commandName << " <operation> <mac> <ip> <hostname>" << endl;
    cout << endl;
    cout << "Command Line options:" << endl;
    cout << "--help show this help text" << endl;
    cout << "--testMulticast send a multicast test message to configured address:port" << endl;
    cout << "--testVectors Run a test with standard vectors & display the internals" << endl;
    cout << "--testMessage Generate a test message" << endl;
}
