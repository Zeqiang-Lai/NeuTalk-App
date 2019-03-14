//
// neu_talk_protocol.h
// NeuTalk
//
// 作者: 赖泽强
// 创建日期: 2018-9-2
//

#ifndef NEU_TALK_PROTOCOL_
#define NEU_TALK_PROTOCOL_

#include <string>
using std::string;

enum NotificationType {
    NotificationLogin,
    NotificationRegister,
    NotificationUpdateSelfIfo,
    NotificationUpdateUserList,
    NotificationUserMsg,
    NotificationGroupMsg,
    NotificationAny
};

enum DataType {
    HeartBeat,
    LoginCheck,
    LoginCheckResult,
    RegisterCheck,
    RegisterCheckResult,
    SelfIfoRequest,
    SelfIfoResult,
    UsersListRequest,
    UsersListResult,
    ChatMsg,
    GroupChatMsg,
};

struct DataLoginCheck {
    unsigned long id;
    string pwd;
};

struct DataRegisterCheck {
    string name;
    string pwd;
};

struct DataLoginCheckResult {
    int status;
};

struct DataRegisterCheckResult {
    int status;
    unsigned long id;
};

struct DataMsg {
    unsigned long sender_id;
    unsigned long recv_id;
    string content;
    string date;
};

struct UserIfo {
    // 用户名
    string name;
    // 唯一标识一个用户,由服务器端生成.
    unsigned long id;
    // IP地址
    string ip;
    // 头像图片名称
    string profile_picture;
};

#endif // NEU_TALK_PROTOCOL_