//
// server_persistence.cpp
// NeuTalk
//
// 作者: 赖泽强
// 创建日期: 2018-9-3
//

#include "server_persistence.h"

int ServerPersistenceManager::Write(std::vector<ProtectedUserIfo> &registered_users)
{
    return 0;
}

int ServerPersistenceManager::Load(std::vector<ProtectedUserIfo> &registered_users)
{
    ProtectedUserIfo ifo1;
    ifo1.id = 1000; ifo1.name = "Amy"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1001; ifo1.name = "Sam"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1002; ifo1.name = "Jack"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1003; ifo1.name = "User3"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1004; ifo1.name = "User4"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1005; ifo1.name = "User5"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1006; ifo1.name = "User6"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1007; ifo1.name = "User7"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1008; ifo1.name = "User8"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1010; ifo1.name = "User9"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1011; ifo1.name = "User10"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1012; ifo1.name = "User11"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1013; ifo1.name = "User12"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1014; ifo1.name = "User13"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1015; ifo1.name = "User14"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1016; ifo1.name = "User15"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1017; ifo1.name = "User16"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1018; ifo1.name = "User17"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1019; ifo1.name = "User18"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    ifo1.id = 1020; ifo1.name = "User19"; ifo1.profile_picture = "default.jpg"; ifo1.pwd = "123456";
    registered_users.push_back(ifo1);

    return 0;
}