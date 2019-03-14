//
// server.h
// NeuTalk
//
// 作者: 赖泽强
// 创建日期: 2018-9-1
//

#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include <thread>
#include <string>
#include <vector>

#include "neu_talk_protocol.h"
#include "protected_user_ifo.h"
#include "server_persistence.h"

struct ClientIfo{
    int sock;
    struct sockaddr_in addr;
    unsigned long id;
};

class Server
{
  public:
    Server();
    ~Server();
    void Run();
    int get_status();

  private:
    void ListenUserConnect();
    void ListenUserRequest();
    void ListenUserLeave();
    
    string RecvString(int sock);

    void OnLogin(DataLoginCheck ifo, ClientIfo &client);
    void OnRegister(DataRegisterCheck ifo, int client_sock);
    void OnUserIfo(unsigned long user_id, int client_sock);
    void OnOnlineUsers(int client_sock);
    void OnUserMsg(DataMsg msg);
    void OnGroupMsg(DataMsg msg);
    void OnUserDisconnect(ClientIfo &client);

    int Load(std::vector<ProtectedUserIfo> &registered_users);
    int Write(std::vector<ProtectedUserIfo> &registered_users);

    //** 成员变量 **//
    int m_serv_sock;
    struct sockaddr_in m_serv_addr;

    bool m_lock_client_list;
    std::vector<ClientIfo> m_client_list;

    std::vector<ProtectedUserIfo> m_registered_users;
    std::vector<UserIfo> m_online_users;

    int m_status;

    ServerPersistenceManager persistence_manager;
};

#endif // SERVER_H_