//
// client_api.h
// NeuTalk
//
// 作者: 赖泽强
// 创建日期: 2018-8-31
//

#ifndef CLIENT_API_H_
#define CLIENT_API_H_

#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <thread>
#include <mutex>

#include "neu_talk_protocol.h"
#include "talk.h"

using std::string;
using std::vector;

class ClientDelegate {
  public:
    virtual void Notify(NotificationType type)=0;
};

class ClientAPI
{
  public:
    static ClientAPI* shared();

    ~ClientAPI();

    //尝试连接至服务器。
    //  - server_ip: 服务器IP地址
    //  - port_num: 服务器端口号
    //Note: 函数不会检查IP和端口的有效性。
    int ConnectToServer(string server_ip, unsigned int port_num);

    // return 1
    int Login(unsigned long id, string pwd);

    int Register(string name, string pwd);

    void RequestSelfIfo();

    void RequestOnlineUsers(); 

    void SendMsg(bool is_group, string content, string date, unsigned long sender_id, unsigned long recv_id);

    // int TryToSendFile();

    // int SendFile(string file_path);

    int Disconnect();
    
    DataLoginCheckResult GetLoginResult();

    int GetStatus();

    UserIfo GetSelfIfo();
    vector<UserIfo> GetOnlineUsers();
    vector<Talk>& GetTalks();

    // Notify delegates if needed.
    ClientDelegate *login_delegate;
    
    ClientDelegate *delegate;

  private:
    void ListenServer();

    void OnLoginConfirm(DataLoginCheckResult ifo);

    void OnRegisterConfirm(DataRegisterCheckResult ifo);

    void OnSelfIfo(UserIfo self_ifo);

    void OnOnlineUsers(vector<UserIfo> online_users);

    void OnUserMsg(DataMsg msg);
    
    void OnGroupMsg(DataMsg msg);

    // void OnFileRequest();

    // void OnFileReceived();

    string RecvString(int sock);

  private:
    static ClientAPI *m_instance;

    int m_serv_sock;
    struct sockaddr_in m_serv_addr;

    // Synchronizes access to member data.
    // TODO(Ze): do we need 5 mutex locks for data?
    mutable std::mutex m_mutex;

    // Data used by both GUI thread and worker thread.
    DataLoginCheckResult m_login_check_result;
    DataRegisterCheckResult m_register_check_result;
    UserIfo m_self_ifo;
    vector<UserIfo> m_online_users;
    vector<Talk> m_talks;

    int status;
};

#endif // CLIENT_API_H_