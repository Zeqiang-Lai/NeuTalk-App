#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <vector>
#include <gtkmm.h>

#include "chat_window.h"
#include "group_window.h"
#include "client_api.h"
#include "neu_talk_protocol.h"

using std::vector;

class MainWindow : public Gtk::Window, public ClientDelegate
{
  public:
    MainWindow(Glib::RefPtr<Gtk::Application> app);
    virtual ~MainWindow();

  protected:
    void UpdateTreeview(vector<UserIfo>);

    void OnTreeviewRowClicked(const Gtk::TreeModel::Path &path, Gtk::TreeViewColumn *column);

    void Notify(NotificationType type);

    void OnNotificationUpdateSelfIfo();

    void OnNotificationUpdateUserList();

    void OnNotificationReceiveMsg();

    // Event handlers for ifo bar.
    void OnInfoBarResponse(int response);

    void OnUnreadMessage(unsigned long id, string name = "");

  private:

    UserIfo m_self_ifo;

    // ---- Handle Chat ---- //
    vector<ChatWindow*> all_chat_windows;
    GroupWindow *m_group_window;

    // ---- Gtk Widgets ---- //

    Glib::RefPtr<Gtk::Application> m_app;

    Glib::Dispatcher m_update_ifo_dispatcher;
    Glib::Dispatcher m_update_user_list_dispatcher;
    Glib::Dispatcher m_receive_msg_dispatcher;

    class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
      public:
        ModelColumns()
        {
            add(m_col_name); //向tree结构中加入列
            add(m_col_ip);
        }
        Gtk::TreeModelColumn<Glib::ustring> m_col_name; //定义加入的数据类型
        Gtk::TreeModelColumn<Glib::ustring> m_col_ip;
    };

    ModelColumns m_columns;
    Gtk::ScrolledWindow m_scrolled_window; //把treeview装入scrolledview
    Gtk::TreeView m_tree_view;
    Glib::RefPtr<Gtk::TreeStore> m_ref_tree_model;

    Gtk::Box m_box_all; //装所有控件的盒子
    Gtk::Box m_box_ifo; //用户信息的盒子

    Gtk::Label m_label_name;
    Gtk::Label m_label_id;

    // Ifo bar: notify user when there is new message.
    Gtk::InfoBar m_info_bar;
    Gtk::Label m_label_info_bar;
    // Gtk::Button m_button_info_bar;
};
#endif //MAIN_WINDOW_H_