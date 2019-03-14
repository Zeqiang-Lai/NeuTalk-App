//
// mlog.h
// NeuTalk
//
// 作者: 赖泽强
// 创建日期: 2018-9-4
//

#ifndef MLOG_H_
#define MLOG_H_

#include <string>
using std::string;

class MLog
{
  public:
    // TODO(Ze): hide public initilizers
    static MLog* shared();
    
    // MLog(string log_file_path);
    void print(string msg);

private:
    static MLog *m_instance;
};

#endif //MLOG_H_