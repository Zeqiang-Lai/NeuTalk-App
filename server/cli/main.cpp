#include "server.h"
#include <signal.h>
#include <iostream>
using namespace std;

int main()
{
    signal(SIGPIPE, SIG_IGN);
    Server server;
    if(server.get_status() == 0){
        cout << "server lanuch successfully" << endl;
        server.Run();
    }
    while(1);
}