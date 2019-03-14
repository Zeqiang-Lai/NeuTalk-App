//
// server_persistence.h
// NeuTalk
//
// 作者: 赖泽强
// 创建日期: 2018-9-2
//

#ifndef SERVER_PERSISTENCE_
#define SERVER_PERSISTENCE_

#include <fstream>
#include <vector>

#include "protected_user_ifo.h"

const string k_protected_users_ifo_file_path = "data/RegisteredUsers.list";

class ServerPersistenceManager
{
  public:
  
    int Write(std::vector<ProtectedUserIfo> &registered_users);

    int Load(std::vector<ProtectedUserIfo> &registered_users);
};

#endif // SERVER_PERSISTENCE_