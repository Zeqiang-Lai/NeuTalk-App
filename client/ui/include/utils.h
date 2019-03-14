//
// utils.cpp
// NeuTalk
//
// 作者: 赖泽强
// 创建日期: 2018-9-4
//

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <gtkmm.h>

void ShowDialog(std::string msg, std::string title, 
                Gtk::MessageType type = Gtk::MESSAGE_INFO, 
                Gtk::ButtonsType buttons = Gtk::BUTTONS_OK);

std::string GetCurrentTime();

#endif // UTILS_H_