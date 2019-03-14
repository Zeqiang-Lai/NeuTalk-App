#include <iostream>
#include "client_api.h"

using std::cout;
using std::cin;
using std::endl;

class ConsoleTalk : public ClientDelegate
{
  public:
    ConsoleTalk() {
        ClientAPI::shared()->login_delegate = this;
        ClientAPI::shared()->delegate = this;
    }

    int Run(string ip_addr, unsigned int port_num) {
        return ClientAPI::shared()->ConnectToServer(ip_addr, port_num);
    }
    int Login(unsigned long id, string pwd) {
        return ClientAPI::shared()->Login(id, pwd);
    }

  private:
    void notify(NotificationType type)
    {
        switch(type) {
            case NotificationLogin:
                login_status = ClientAPI::shared()->GetLoginResult().status;
                
                break;
        }
    }

    int login_status;
};

int main()
{
    ConsoleTalk app;

    // ---- Try to connect to server ---- //
    int status = app.Run("127.0.0.1", 12306);
    if(status == -1) {
        cout << "Unable to connect to the server." << endl;
        return -1;
    } else {
        cout << "Connect to the server successfully." << endl;
    }

    // ---- Try to login ---- //
    unsigned long id;
    string pwd;
    cout << "Now, you have to login." << endl;
    cout << "Type in your id: ";
    cin >> id;
    cout << "Tpye in your password: ";
    cin >> pwd;
    app.Login(id, pwd);
    cout << "Waiting for the response from server." << endl;


    string command;
    while(1) {
        cout << "Type your command: " << endl;
        cout << ""
    }
}