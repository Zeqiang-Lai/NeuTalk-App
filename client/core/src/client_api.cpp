//
// client_api.h
// NeuTalk
//
// 作者: 赖泽强
// 创建日期: 2018-9-2
//

#include "client_api.h"
#include "mlog.h"

using std::cout;
using std::endl;

ClientAPI* ClientAPI::m_instance = NULL;

// TODO(Ze): complete the whole singleton code
// Warning: may cause memory leak
ClientAPI* ClientAPI::shared()
{
    if(m_instance == NULL)
        m_instance = new ClientAPI();
 
    return m_instance;
}

ClientAPI::~ClientAPI()
{
    close(m_serv_sock);
}

int ClientAPI::ConnectToServer(string server_ip, unsigned int port_num)
{
    cout << "Try to connect " << server_ip << ":" << port_num << endl;
    // 向服务器（特定的IP和端口）发起请求
    memset(&m_serv_addr, 0, sizeof(m_serv_addr));  // 每个字节都用0填充
    m_serv_addr.sin_family = AF_INET;  // 使用IPv4地址
    m_serv_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());  // 具体的IP地址
    m_serv_addr.sin_port = htons(port_num);  //端口

    // 创建套接字
    m_serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    std::lock_guard<std::mutex> lock(m_mutex);
    status = connect(m_serv_sock, (struct sockaddr*)&m_serv_addr, sizeof(m_serv_addr));

    cout << "Status=" << status << endl;

    // 开始监听服务器传回的信息
    if(status == 0) {
        std::thread recv_thread(&ClientAPI::ListenServer, this);
        recv_thread.detach();
    }
    return status;
}

// TODO(Ze): 是否需要？析构函数是否就够了
int ClientAPI::Disconnect()
{
    return 0;
}

int ClientAPI::Login(unsigned long id, string pwd)
{
    DataType type = LoginCheck;
    DataLoginCheck data;
    data.id = id;
    data.pwd = pwd;
    unsigned int data_size = sizeof(data);

    // ---- Linux下不可用 ---- //
    // send(m_serv_sock, &type, sizeof(type), MSG_WAITALL);
    // send(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    // send(m_serv_sock, &data, data_size, MSG_WAITALL);
    //

    // TODO(Ze): 尝试使用更优雅的解决方案
    send(m_serv_sock, &type, sizeof(type), MSG_WAITALL);
    send(m_serv_sock, &data.id, sizeof(data.id), MSG_WAITALL);

    data_size = data.pwd.size();
    send(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    send(m_serv_sock, data.pwd.data(), data.pwd.size(), MSG_WAITALL);

    return 1;
}

int ClientAPI::Register(string name, string pwd)
{
    DataType type = RegisterCheck;
    DataRegisterCheck data;
    data.name = name;
    data.pwd = pwd;
    unsigned int data_size = sizeof(data);

    // ---- Linux下不可用 ---- //
    // send(m_serv_sock, &type, sizeof(type), MSG_WAITALL);
    // send(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    // send(m_serv_sock, &data, data_size, MSG_WAITALL);
    //
    
    // TODO(Ze): 尝试使用更优雅的解决方案
    send(m_serv_sock, &type, sizeof(type), MSG_WAITALL);

    data_size = data.name.size();
    send(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    send(m_serv_sock, data.name.data(), data.name.size(), MSG_WAITALL);

    data_size = data.pwd.size();
    send(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    send(m_serv_sock, data.pwd.data(), data.pwd.size(), MSG_WAITALL);

    return 1;
}

void ClientAPI::RequestSelfIfo()
{
    DataType type = SelfIfoRequest;

    send(m_serv_sock, &type, sizeof(type), MSG_WAITALL);
}

void ClientAPI::RequestOnlineUsers()
{
    DataType type = UsersListRequest;

    send(m_serv_sock, &type, sizeof(type), MSG_WAITALL); 
}

void ClientAPI::SendMsg(bool is_group, string content, string date, unsigned long sender_id, unsigned long recv_id)
{
    cout << "Send message" << endl;
    DataMsg msg;
    msg.content = content;
    msg.date = date;
    msg.sender_id = sender_id;
    msg.recv_id = recv_id;

    DataType type = ChatMsg;
    if(is_group == true)
        type = GroupChatMsg;
 
    unsigned int data_size = sizeof(msg);
    
    std::lock_guard<std::mutex> lock(m_mutex);
    bool is_new_talk = true;

    for(auto &talk : m_talks) {
        if(type == GroupChatMsg && talk.type == TALK_TYPE_GROUP) {
            cout << "Push group message in" << endl;
            talk.msgs.push_back(msg);
            is_new_talk = false;
            continue;
        }
        
        if(talk.recv_id == msg.recv_id && talk.type == TALK_TYPE_PRIATE) {
            cout << "Push message in" << endl;
            talk.msgs.push_back(msg);
            is_new_talk = false; 
        }
    }
    if(is_new_talk) {
        Talk new_talk;
        new_talk.type = TALK_TYPE_PRIATE;
        if(is_group)
            new_talk.type = TALK_TYPE_GROUP;
        new_talk.recv_id = msg.recv_id;
        new_talk.sender_id = m_self_ifo.id;
        cout << "Push new message in" << endl;
        new_talk.msgs.push_back(msg);
        m_talks.push_back(new_talk);
    }

    // ---- Linux下不可用 ---- //
    // send(m_serv_sock, &type, sizeof(type), MSG_WAITALL);
    // send(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    // send(m_serv_sock, &msg, data_size, MSG_WAITALL);
    //

    // TODO(Ze): 尝试使用更优雅的解决方案
    send(m_serv_sock, &type, sizeof(type), MSG_WAITALL);
    send(m_serv_sock, &msg.sender_id, sizeof(msg.sender_id), MSG_WAITALL);
    send(m_serv_sock, &msg.recv_id, sizeof(msg.recv_id), MSG_WAITALL);

    data_size = msg.content.size();
    send(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    send(m_serv_sock, msg.content.data(), msg.content.size(), MSG_WAITALL);

    data_size = msg.date.size();
    send(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
    send(m_serv_sock, msg.date.data(), msg.date.size(), MSG_WAITALL);

}

string ClientAPI::RecvString(int sock)
{
    unsigned int data_size;
    int recv_size;
    char *buffer;
    string s;

    recv_size = recv(sock, &data_size, sizeof(data_size), MSG_WAITALL);
    buffer = new char[data_size];
    recv_size = recv(sock, buffer, data_size, MSG_WAITALL);
    s.append(buffer, buffer+data_size);
    delete[] buffer;

    return s;
}

void ClientAPI::ListenServer() 
{
    int recv_size;
    DataType type;
    unsigned int data_size;
    unsigned int num_of_length;

    cout << "Start listening server" << endl;

    DataLoginCheckResult data;
    DataRegisterCheckResult data2;

    UserIfo data3;

    vector<UserIfo> data4;
    DataMsg data5;
    char *buffer;

    while(1) {
        // 读取服务器传回的数据
        // TODO(Ze): 读取前先判断是否还连接着服务器
        // 先读类型
        recv_size = recv(m_serv_sock, &type, sizeof(DataType), MSG_WAITALL);
        switch(type) {
            case HeartBeat:
                cout << "Receive heart beat" << endl;
                break;
            case LoginCheckResult:
                recv_size = recv(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
                recv_size = recv(m_serv_sock, &data, data_size, MSG_WAITALL);
                OnLoginConfirm(data);
                break;
            case RegisterCheckResult:
                recv_size = recv(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
                recv_size = recv(m_serv_sock, &data2, data_size, MSG_WAITALL);
                OnRegisterConfirm(data2);
                break;
            case SelfIfoResult:
                // recv_size = recv(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
                // recv_size = recv(m_serv_sock, &data3, data_size, MSG_WAITALL);

                recv_size = recv(m_serv_sock, &data3.id, sizeof(data3.id), MSG_WAITALL);
                
                data3.name = RecvString(m_serv_sock);
                data3.ip = RecvString(m_serv_sock);
                data3.profile_picture = RecvString(m_serv_sock);

                OnSelfIfo(data3);
                break;
            case UsersListResult:
                data4.clear();
                recv_size = recv(m_serv_sock, &num_of_length, sizeof(num_of_length), MSG_WAITALL);
                cout << "!Num of user=" << num_of_length << endl;
                for(int i=0; i<num_of_length; ++i) {
                    // recv_size = recv(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
                    // recv_size = recv(m_serv_sock, &data3, data_size, MSG_WAITALL);

                    recv_size = recv(m_serv_sock, &data3.id, sizeof(data3.id), MSG_WAITALL);
                    cout << "ID=" << data3.id << endl;
                    data3.name = RecvString(m_serv_sock);
                    cout << "Name=" << data3.name << endl;
                    data3.ip = RecvString(m_serv_sock);
                    data3.profile_picture = RecvString(m_serv_sock);

                    data4.push_back(data3);
                    cout << data4.size() << endl;
                }
                cout << "!Done" << endl;
                OnOnlineUsers(data4);
                break;
            case ChatMsg:
                // recv_size = recv(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
                // recv_size = recv(m_serv_sock, &data5, data_size, MSG_WAITALL);

                recv_size = recv(m_serv_sock, &data5.sender_id, sizeof(data5.sender_id), MSG_WAITALL);
                recv_size = recv(m_serv_sock, &data5.recv_id, sizeof(data5.recv_id), MSG_WAITALL);

                data5.content = RecvString(m_serv_sock);
                data5.date = RecvString(m_serv_sock);

                OnUserMsg(data5);
                break;
            case GroupChatMsg:
                // recv_size = recv(m_serv_sock, &data_size, sizeof(data_size), MSG_WAITALL);
                // recv_size = recv(m_serv_sock, &data5, data_size, MSG_WAITALL);
                recv_size = recv(m_serv_sock, &data5.sender_id, sizeof(data5.sender_id), MSG_WAITALL);
                recv_size = recv(m_serv_sock, &data5.recv_id, sizeof(data5.recv_id), MSG_WAITALL);

                data5.content = RecvString(m_serv_sock);
                data5.date = RecvString(m_serv_sock);

                OnGroupMsg(data5);
                break;
        }
    }
    cout << "Stop listening server" << endl;
}

void ClientAPI::OnLoginConfirm(DataLoginCheckResult ifo)
{
    cout << "Login Confirm, status=" << ifo.status << endl;

    std::lock_guard<std::mutex> lock(m_mutex);
    m_login_check_result = ifo;

    if(login_delegate != NULL) {
        login_delegate->Notify(NotificationLogin);
    }
}

void ClientAPI::OnRegisterConfirm(DataRegisterCheckResult ifo)
{
    cout << "register check, status=" << ifo.status << " ,id=" << ifo.id << endl;

    std::lock_guard<std::mutex> lock(m_mutex);
    m_register_check_result = ifo;

    if(login_delegate != NULL) {
        login_delegate->Notify(NotificationRegister);
    }
}

void ClientAPI::OnSelfIfo(UserIfo self_ifo)
{
    cout << "Receive Self Ifo: " << endl;
    cout << self_ifo.id << endl;
    cout << self_ifo.name << endl;
    cout << self_ifo.profile_picture << endl;
    cout << endl;

    std::lock_guard<std::mutex> lock(m_mutex);
    m_self_ifo = self_ifo;

    if(delegate != NULL) {
        delegate->Notify(NotificationUpdateSelfIfo);
    }
    // cout << "End Self Ifo" << endl;
}

void ClientAPI::OnOnlineUsers(vector<UserIfo> online_users)
{
    cout << "Receive online users Ifo: " << endl;
    for(auto ifo : online_users) {
        cout << ifo.id << endl;
        cout << ifo.name << endl;
        cout << ifo.profile_picture << endl;
        cout << endl;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    m_online_users = online_users;
    cout << "Done(online users)" << endl;

    if(delegate != NULL) {
        cout << "Try to notify" << endl;
        delegate->Notify(NotificationUpdateUserList);
        cout << "Done" << endl;
    } else {
        cout << "Delegate = NULL" << endl;
    }
}

void ClientAPI::OnUserMsg(DataMsg msg)
{
    cout << "Receive user message(id=" << msg.sender_id << ")" << endl;
    cout << msg.content << endl;

    std::lock_guard<std::mutex> lock(m_mutex);

    bool is_new_talk = true;
    for(auto &talk : m_talks) {
        if(talk.recv_id == msg.sender_id && talk.type == TALK_TYPE_PRIATE) {
            talk.isReceviewNewMsg = true;
            talk.msgs.push_back(msg);
            is_new_talk = false; 
        }
    }
    if(is_new_talk) {
        Talk new_talk;
        new_talk.type = TALK_TYPE_PRIATE;
        new_talk.recv_id = msg.sender_id;
        new_talk.sender_id = m_self_ifo.id;
        new_talk.isReceviewNewMsg = true;
        new_talk.msgs.push_back(msg);
        m_talks.push_back(new_talk);
    }

    if(delegate != NULL) {
        delegate->Notify(NotificationUserMsg);
    }
}
    
void ClientAPI::OnGroupMsg(DataMsg msg)
{
    cout << "Receive group message" << endl;
    cout << msg.content << endl;

    std::lock_guard<std::mutex> lock(m_mutex);
    bool is_new_talk = true;
    for(auto &talk : m_talks) {
        if(talk.type == TALK_TYPE_GROUP && msg.sender_id != m_self_ifo.id) {
            talk.isReceviewNewMsg = true;
            talk.msgs.push_back(msg);
            is_new_talk = false; 
        }
    }
    if(is_new_talk && msg.sender_id != m_self_ifo.id) {
        Talk new_talk;
        new_talk.type = TALK_TYPE_GROUP;
        new_talk.isReceviewNewMsg = true;
        new_talk.msgs.push_back(msg);
        m_talks.push_back(new_talk);
    }

    if(delegate != NULL) {
        delegate->Notify(NotificationGroupMsg);
    }
}

DataLoginCheckResult ClientAPI::GetLoginResult()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_login_check_result;
}

int ClientAPI::GetStatus()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return status;
}

UserIfo ClientAPI::GetSelfIfo()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_self_ifo;
}

vector<UserIfo> ClientAPI::GetOnlineUsers()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_online_users;
}

vector<Talk>& ClientAPI::GetTalks()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_talks;
}
