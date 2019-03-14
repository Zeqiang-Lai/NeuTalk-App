#include "client_api.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    ClientAPI client;

    cout << "Try to connect server..." << endl;
    int status = client.ConnectToServer("172.20.10.8", 12306);
    cout << "Finish, status" << status << endl;

    cout << "Try to login..." << endl;
    unsigned long self_id = strtoul(argv[1],NULL,0);
    status = client.Login(self_id, argv[2]);
    cout << "Finish, status" << status << endl;
    
    // cout << "Try to Register..." << endl;
    // status = client.Register("Zeqiang", "123456");
    // cout << "Finish, status" << status << endl;
    
    // client.RequestSelfIfo();
    // cout << "Request Online Users" << endl;
    // client.RequestOnlineUsers();
    // client.RequestOnlineUsers();
    // cout << "Finsih" << endl;
    cout << "Send Msg" << endl;
  
    string content;
    unsigned long recv_id;
    time_t date;
    while(1){
        cout << ":";
        cin >> content;
        cout << "Send to(id):";
        cin >> recv_id;
        client.SendMsg(false, content, "2018-9-3", self_id, recv_id);
    }
    return 0;
}
