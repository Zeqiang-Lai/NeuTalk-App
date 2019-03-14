//
// mlog.cpp
// NeuTalk
//
// 作者: 赖泽强
// 创建日期: 2018-9-4
//

#include <iostream>
#include "mlog.h"

MLog* MLog::m_instance = NULL;

MLog* MLog::shared()
{
    if(m_instance == NULL)
        m_instance = new MLog();
 
    return m_instance;
}

void MLog::print(string msg)
{
    std::cout << msg << std::endl;
}