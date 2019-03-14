//
// protected_user_ifo.h
// NeuTalk
//
// 作者: 赖泽强
// 创建日期: 2018-9-2
//

#ifndef PROTECTED_USER_IFO_
#define PROTECTED_USER_IFO_

#include <string>
using std::string;

struct ProtectedUserIfo {
    unsigned long id;
    string name;
    string pwd;
    string profile_picture;
};

#endif // PROTECTED_USER_IFO_