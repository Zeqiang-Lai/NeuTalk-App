//
// server.h
// NeuTalk
//
// 作者: 赖泽强
// 创建日期: 2018-9-2
//

#include <unistd.h>

#include "server.h"
#include "config.h"

using std::cout;
using std::endl;

Server::Server() : persistence_manager()
{

    Config config;
    config.Load();
    cout << config.GetIP() << ":" << config.GetPort() << endl;
    m_status = 0;
    m_serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&m_serv_addr, 0, sizeof(m_serv_addr));  //每个字节都用0填充
    m_serv_addr.sin_family = AF_INET;  //使用IPv4地址
    m_serv_addr.sin_addr.s_addr = inet_addr(config.GetIP().c_str());  //具体的IP地址
    m_serv_addr.sin_port = htons(config.GetPort());  //端口

    std::cout << "Start binding port" << std::endl;
    m_status = bind(m_serv_sock, (struct sockaddr*)&m_serv_addr, sizeof(m_serv_addr));
    std::cout << "finish binding port, status=" << m_status << std::endl;
    //进入监听状态，等待用户发起请求
    if(m_status == 0) {
        std::cout << "Start listening port" << std::endl;
        m_status = listen(m_serv_sock, 20);
        std::cout << "Finish listening port, status=" << m_status << std::endl;
    }

    // 加载注册用户列表
    Load(m_registered_users);
}

Server::~Server()
{
    close(m_serv_sock);
    for(auto client : m_client_list)
        close(client.sock);
}

int Server::get_status()
{
    return m_status;
}

void Server::Run()
{
    std::thread t1(&Server::ListenUserConnect, this);
    t1.detach();
    std::thread t2(&Server::ListenUserRequest, this);
    t2.detach();
    // std::thread t3(&Server::ListenUserLeave, this);
    // t3.detach();
}

void Server::ListenUserConnect()
{
    std::cout << "Start listening user connect" << std::endl;
    while(1){
        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        int clnt_sock = accept(m_serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);  
        m_lock_client_list = true;
        ClientIfo ifo;
        ifo.sock = clnt_sock; ifo.addr = clnt_addr;
        m_client_list.push_back(ifo);
        std::cout << "User connected: " << clnt_sock << std::endl; 
        m_lock_client_list = false;
    }
    std::cout << "Stop listening user connect" << std::endl;
}

string Server::RecvString(int sock)
{
    unsigned int data_size;
    int recv_size;
    char *buffer;
    string s;

    recv_size = recv(sock, &data_size, sizeof(data_size), MSG_WAITALL);
    cout << "data_size = " << data_size << endl;
    buffer = new char[data_size];
    recv_size = recv(sock, buffer, data_size, MSG_WAITALL);
    s.append(buffer, buffer+data_size);
    delete[] buffer;

    return s;
}

void Server::ListenUserRequest()
{
    long count = 1;
    while(1) {
        count++;
        while(m_lock_client_list == true);

        int recv_size;
        DataType type;
        unsigned int data_size;

        bool is_recv_data = false;

        for(auto &client : m_client_list) {

            // // Check user is online?
            if(count % 500000 == 0) {
                type = HeartBeat;
                recv_size = send(client.sock, &type, sizeof(type), MSG_WAITALL);
                cout << "hear beat from " << client.id <<": " << recv_size << endl;
                if(recv_size == -1) {
                    OnUserDisconnect(client);
                }
            }

            recv_size = recv(client.sock, &type, sizeof(DataType), MSG_DONTWAIT);

            DataLoginCheck data;
            DataRegisterCheck data2;
            DataMsg data3;

            if(recv_size > 0) {
                cout << "Type=" << type << endl;
                switch(type) {
                    case LoginCheck:
                        // recv_size = recv(client.sock, &data_size, sizeof(data_size), MSG_WAITALL);
                        // recv_size = recv(client.sock, &data, data_size, MSG_WAITALL);
                        cout << "Recving loginCheck" << endl;
                        recv_size = recv(client.sock, &data.id, sizeof(data.id), MSG_WAITALL);
                        cout << data.id << endl;
                        data.pwd = RecvString(client.sock);
                        cout << "pwd:" << data.pwd << endl;
                        OnLogin(data, client);
                        break;
                    case RegisterCheck:
                        // recv_size = recv(client.sock, &data_size, sizeof(data_size), MSG_WAITALL);
                        // recv_size = recv(client.sock, &data2, data_size, MSG_WAITALL);

                        data2.name = RecvString(client.sock);
                        data2.name = RecvString(client.sock);

                        OnRegister(data2, client.sock);
                        break;
                    case SelfIfoRequest:
                        OnUserIfo(client.id, client.sock);
                        break;
                    case UsersListRequest:
                        OnOnlineUsers(client.sock);
                        break;
                    case ChatMsg:
                        // recv_size = recv(client.sock, &data_size, sizeof(data_size), MSG_WAITALL);
                        // recv_size = recv(client.sock, &data3, data_size, MSG_WAITALL);

                        recv_size = recv(client.sock, &data3.sender_id, sizeof(data3.sender_id), MSG_WAITALL);
                        recv_size = recv(client.sock, &data3.recv_id, sizeof(data3.recv_id), MSG_WAITALL);

                        data3.content = RecvString(client.sock);
                        data3.date = RecvString(client.sock);

                        OnUserMsg(data3);
                        break;
                    case GroupChatMsg:
                        // recv_size = recv(client.sock, &data_size, sizeof(data_size), MSG_WAITALL);
                        // recv_size = recv(client.sock, &data3, data_size, MSG_WAITALL);

                        recv_size = recv(client.sock, &data3.sender_id, sizeof(data3.sender_id), MSG_WAITALL);
                        recv_size = recv(client.sock, &data3.recv_id, sizeof(data3.recv_id), MSG_WAITALL);

                        data3.content = RecvString(client.sock);
                        data3.date = RecvString(client.sock);

                        OnGroupMsg(data3);
                        break;
                }
            }
        } 
        if(count % 5000000 == 0) {
            count = 1;
        }   
    }
}

void Server::ListenUserLeave()
{

}

void Server::OnLogin(DataLoginCheck ifo, ClientIfo &client)
{
    cout << ifo.id << endl;
    cout << ifo.pwd << endl;
    cout << "loginCheck" << endl;

    DataType type = LoginCheckResult;
    DataLoginCheckResult data;

    // 校验
    UserIfo user_ifo;
    data.status = 1;
    
    // TODO(Ze): this process can be optimized 

    for(auto registered_user : m_registered_users) {
        if(registered_user.id == ifo.id && registered_user.pwd == ifo.pwd){
            data.status = 0;
            // 记录一下用户信息,便于后期推送等操作
            user_ifo.id = registered_user.id;
            user_ifo.name = registered_user.name;
            user_ifo.profile_picture = registered_user.profile_picture;
            // user_ifo.ip = inet_ntoa(client.addr.sin_addr);
        }
    }
    //  
    for(auto user : m_online_users) {
        if(user.id == ifo.id) 
            data.status = 1;
    }

    unsigned int data_size = sizeof(data);

    send(client.sock, &type, sizeof(type), MSG_WAITALL);
    send(client.sock, &data_size, sizeof(data_size), MSG_WAITALL);
    send(client.sock, &data, data_size, MSG_WAITALL);

    if(data.status == 0) {
        client.id = ifo.id;
        user_ifo.ip = inet_ntoa(client.addr.sin_addr);
        // user_ifo.port_num = ntohs(client.addr.sin_port);
        
        // 加入在线用户列表
        m_online_users.push_back(user_ifo);
        cout << "Add to online user list" << endl;
        // 向所有用户推送新的在线用户列表
        for(auto t_client : m_client_list) {
            if(t_client.sock != client.sock) {
                OnOnlineUsers(t_client.sock);
            }
        }
    }
}

void Server::OnRegister(DataRegisterCheck ifo, int client_sock)
{
    cout << ifo.name << endl;
    cout << ifo.pwd << endl;
    cout << "register check" << endl;

    DataType type = RegisterCheckResult;
    DataRegisterCheckResult data;
    data.status = 1;
    data.id = 99999;
    unsigned int data_size = sizeof(data);

    send(client_sock, &type, sizeof(type), MSG_WAITALL);
    send(client_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    send(client_sock, &data, data_size, MSG_WAITALL);
}

void Server::OnUserIfo(unsigned long user_id, int client_sock)
{
    UserIfo user_ifo;
    for(auto ifo : m_registered_users){
        if(ifo.id == user_id) {
            user_ifo.id = ifo.id;
            user_ifo.name = ifo.name;
            user_ifo.profile_picture = ifo.profile_picture;
        }
    }

    DataType type = SelfIfoResult;

    // ---- Linux下不可用 ---- //
    // unsigned int data_size = sizeof(user_ifo);

    // ---- Linux下不可用 ---- //
    // send(client_sock, &type, sizeof(type), MSG_WAITALL);
    // send(client_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    // send(client_sock, &user_ifo, data_size, MSG_WAITALL);

    // TODO(Ze): 尝试使用更优雅的解决方案
    send(client_sock, &type, sizeof(type), MSG_WAITALL);
    send(client_sock, &user_ifo.id, sizeof(user_ifo.id), MSG_WAITALL);

    unsigned int data_size;

    data_size = user_ifo.name.size();
    send(client_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    send(client_sock, user_ifo.name.data(), user_ifo.name.size(), MSG_WAITALL);

    data_size = user_ifo.ip.size();
    send(client_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    send(client_sock, user_ifo.ip.data(), user_ifo.ip.size(), MSG_WAITALL);

    data_size = user_ifo.profile_picture.size();
    send(client_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    send(client_sock, user_ifo.profile_picture.data(), user_ifo.profile_picture.size(), MSG_WAITALL);
    // 

}

void Server::OnOnlineUsers(int client_sock)
{
    cout << "Send Online User list" << endl;
    DataType type = UsersListResult;
    unsigned int data_size = sizeof(m_online_users);
    unsigned int num_of_users = m_online_users.size();

    cout << "Number of users = " << num_of_users << endl;

    send(client_sock, &type, sizeof(type), MSG_WAITALL);
    // send(client_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    send(client_sock, &num_of_users, sizeof(num_of_users), MSG_WAITALL);
    for(UserIfo ifo : m_online_users) {
        // ---- Linux下不可用 ---- //
        // data_size = sizeof(ifo);
        // send(client_sock, &data_size, sizeof(data_size), MSG_WAITALL);
        // send(client_sock, &ifo, data_size, MSG_WAITALL);
        //

        // TODO(Ze): 尝试使用更优雅的解决方案
        // send(client_sock, &type, sizeof(type), MSG_WAITALL);
        send(client_sock, &ifo.id, sizeof(ifo.id), MSG_WAITALL);

        data_size = ifo.name.size();
        send(client_sock, &data_size, sizeof(data_size), MSG_WAITALL);
        send(client_sock, ifo.name.data(), ifo.name.size(), MSG_WAITALL);

        data_size = ifo.ip.size();
        send(client_sock, &data_size, sizeof(data_size), MSG_WAITALL);
        send(client_sock, ifo.ip.data(), ifo.ip.size(), MSG_WAITALL);

        data_size = ifo.profile_picture.size();
        send(client_sock, &data_size, sizeof(data_size), MSG_WAITALL);
        send(client_sock, ifo.profile_picture.data(), ifo.profile_picture.size(), MSG_WAITALL);
        // 
    }
    cout << "Done" << endl;
}

void Server::OnUserMsg(DataMsg msg)
{
    cout << "Transfer messsage" << endl;
    int recv_sock;
    for(auto client : m_client_list) {
        cout << "ID = " << client.id << endl;
        if(client.id == msg.recv_id) {
            cout << "find: " << client.id << endl;
            recv_sock = client.sock;
            DataType type = ChatMsg;
            unsigned int data_size = sizeof(msg);

            // ---- Linux下不可用 ---- //
            // send(recv_sock, &type, sizeof(type), MSG_WAITALL);
            // send(recv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
            // send(recv_sock, &msg, data_size, MSG_WAITALL);

            // TODO(Ze): 尝试使用更优雅的解决方案
            send(recv_sock, &type, sizeof(type), MSG_WAITALL);
            send(recv_sock, &msg.sender_id, sizeof(msg.sender_id), MSG_WAITALL);
            send(recv_sock, &msg.recv_id, sizeof(msg.recv_id), MSG_WAITALL);

            data_size = msg.content.size();
            send(recv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
            send(recv_sock, msg.content.data(), msg.content.size(), MSG_WAITALL);

            data_size = msg.date.size();
            send(recv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
            send(recv_sock, msg.date.data(), msg.date.size(), MSG_WAITALL);
            //
        }
    }
}

void Server::OnGroupMsg(DataMsg msg)
{
    cout << "Transfer messsage" << endl;
    DataType type = GroupChatMsg;
    unsigned int data_size = sizeof(msg);
    
    for(auto client : m_client_list) {
        // ---- Linux下不可用 ---- //
        // send(client.sock, &type, sizeof(type), MSG_WAITALL);
        // send(client.sock, &data_size, sizeof(data_size), MSG_WAITALL);
        // send(client.sock, &msg, data_size, MSG_WAITALL);

        // TODO(Ze): 尝试使用更优雅的解决方案
        send(client.sock, &type, sizeof(type), MSG_WAITALL);
        send(client.sock, &msg.sender_id, sizeof(msg.sender_id), MSG_WAITALL);
        send(client.sock, &msg.recv_id, sizeof(msg.recv_id), MSG_WAITALL);

        data_size = msg.content.size();
        send(client.sock, &data_size, sizeof(data_size), MSG_WAITALL);
        send(client.sock, msg.content.data(), msg.content.size(), MSG_WAITALL);

        data_size = msg.date.size();
        send(client.sock, &data_size, sizeof(data_size), MSG_WAITALL);
        send(client.sock, msg.date.data(), msg.date.size(), MSG_WAITALL);
        //
    }
}

void Server::OnUserDisconnect(ClientIfo &client)
{
    close(client.sock);

    cout << "User disconnect: id=" << client.id << endl;
    
    std::vector<UserIfo>::iterator iter;
    for(iter = m_online_users.begin(); iter != m_online_users.end();) {
        if((*iter).id == client.id) {
            m_online_users.erase(iter);
        } else {
            iter++;
        }
    }
    cout << "Delete done" << endl;
    for(auto user : m_online_users) {
        cout << user.id << endl;
    }
    std::vector<ClientIfo>::iterator iter2;
    for(iter2 = m_client_list.begin(); iter2 != m_client_list.end();) {
        if((*iter2).id == client.id) {
            m_client_list.erase(iter2);
        } else {
            iter2++;
        }
    }
    // 向所有用户推送新的在线用户列表
    for(auto t_client : m_client_list) {
        // if(t_client.sock != client.sock) {
            OnOnlineUsers(t_client.sock);
        // }
    }
}

int Server::Load(std::vector<ProtectedUserIfo> &registered_users)
{
    return persistence_manager.Load(registered_users);
}
 
int Server::Write(std::vector<ProtectedUserIfo> &registered_users)
{
    return persistence_manager.Write(registered_users);
}
