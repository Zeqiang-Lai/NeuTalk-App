
#include <iostream>
#include "main_window.h"
#include "chat_window.h"
#include "utils.h"

using std::cout;
using std::endl;

MainWindow::MainWindow(Glib::RefPtr<Gtk::Application> app): 
                          m_app(app),
                          m_box_all(Gtk::ORIENTATION_VERTICAL),
                          m_box_ifo(Gtk::ORIENTATION_VERTICAL),
                          m_group_window(NULL)
                        //   m_button_info_bar("infobar")
{
    set_title("NeuTalk");
    set_default_size(240,560);

    cout << "Init ifo bar" << endl;
    // Info bar
    m_info_bar.signal_response().connect(sigc::mem_fun(*this,&MainWindow::OnInfoBarResponse));
    auto infoBarContainer=dynamic_cast<Gtk::Container*>(m_info_bar.get_content_area());
    if(infoBarContainer)
        infoBarContainer->add(m_label_info_bar);
    m_info_bar.add_button("_OK",0);
    m_box_all.pack_start(m_info_bar,Gtk::PACK_SHRINK);

    cout << "Init ifo panel" << endl;
    m_label_name.set_text("User name: Jack");//为label设置文本
    m_label_id.set_text("ID: 1000");

    m_label_name.set_halign(Gtk::ALIGN_START);//设置label的对齐方式
    m_label_id.set_halign(Gtk::ALIGN_START);

    m_box_ifo.set_margin_top(20);//设置边距
    m_box_ifo.set_margin_left(26);
    m_box_ifo.set_margin_right(16);
    m_box_ifo.set_spacing(12);

    m_box_ifo.pack_start(m_label_name, Gtk::PACK_SHRINK);//设置排列方式
    m_box_ifo.pack_start(m_label_id, Gtk::PACK_SHRINK);
    
    cout << "Init tree view" << endl;
    m_scrolled_window.add(m_tree_view);

    m_box_all.pack_start(m_box_ifo, Gtk::PACK_SHRINK);
    m_box_all.pack_start(m_scrolled_window);
    m_box_all.set_spacing(12);

    m_tree_view.signal_row_activated().connect(sigc::mem_fun(*this,&MainWindow::OnTreeviewRowClicked));
    
    cout << "Init tree view column" << endl;
    m_tree_view.append_column("Name", m_columns.m_col_name);
    m_tree_view.append_column("IP", m_columns.m_col_ip);
    
    // m_box_all.pack_end(m_button_info_bar,Gtk::PACK_SHRINK);
    // m_button_info_bar.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::OnInfoBarButtonClicked));

    add(m_box_all);
    show_all_children();
    
    m_update_ifo_dispatcher.connect(sigc::mem_fun(*this, &MainWindow::OnNotificationUpdateSelfIfo));
    m_update_user_list_dispatcher.connect(sigc::mem_fun(*this, &MainWindow::OnNotificationUpdateUserList));
    m_receive_msg_dispatcher.connect(sigc::mem_fun(*this, &MainWindow::OnNotificationReceiveMsg));

    m_info_bar.hide();

    cout << "Init ClientAPI" << endl;

    ClientAPI::shared()->delegate = this;

    // Send Data Request
    ClientAPI::shared()->RequestSelfIfo();
    ClientAPI::shared()->RequestOnlineUsers();
}

MainWindow::~MainWindow() {}

void MainWindow::UpdateTreeview(vector<UserIfo> users)//设置treeview数值的函数
{
    m_ref_tree_model = Gtk::TreeStore::create(m_columns);
    m_tree_view.remove_all_columns();
    m_tree_view.set_model(m_ref_tree_model);

    Gtk::TreeModel::Row row;

    // TODO(Ze): Group chatting.
    row = *(m_ref_tree_model->append());
    row[m_columns.m_col_name] = "Big Group";
    row[m_columns.m_col_ip] = "0.0.0.0";

    for(auto user : users) {
        row = *(m_ref_tree_model->append());
        row[m_columns.m_col_name] = user.name;
        row[m_columns.m_col_ip] = user.ip;
    }

    m_tree_view.append_column("Name", m_columns.m_col_name);
    m_tree_view.append_column("IP", m_columns.m_col_ip);
}

void MainWindow::OnTreeviewRowClicked(const Gtk::TreeModel::Path& path, 
                                           Gtk::TreeViewColumn* column )
{
    Gtk::TreeModel::iterator iter = m_ref_tree_model->get_iter(path);
    if(iter)
    {
        Gtk::TreeModel::Row row = *iter;
        cout << row[m_columns.m_col_name] << row[m_columns.m_col_ip] << endl;

        // Group clicked
        if(row[m_columns.m_col_ip] == "0.0.0.0") {
            if(m_group_window == NULL) {
                cout << "Try to Init" << endl;
                m_group_window = new GroupWindow(m_app, m_self_ifo);
                cout << "Group init done" << endl;
                m_app->add_window(*m_group_window);
            }
            cout << "Try to show group window" << endl;
            m_group_window->show();
            OnNotificationReceiveMsg();
            return;
        }

        // TODO(Ze): if user send msg to himself?

        vector<UserIfo> online_users = ClientAPI::shared()->GetOnlineUsers();
        for(auto user : online_users) {
            // Find the id of corresponding user.
            if(user.ip == row[m_columns.m_col_ip]) {
                cout << user.id << endl;
                for(auto window : all_chat_windows) {
                    if(window->m_recv_ifo.id == user.id) {
                        window->show();
                        return;
                    } 
                }
                ChatWindow *chat_window = new ChatWindow(m_app, user, m_self_ifo);
                all_chat_windows.push_back(chat_window);
                m_app->add_window(*chat_window);
                chat_window->show();
                
                // TODO(Ze): need a more proper way to refresh chat window 
                // when user launch a window for the first time.
                OnNotificationReceiveMsg();
            }
        }
    }
}

void MainWindow::Notify(NotificationType type)
{
    cout << "Notify: mainwindow" << endl;
    if(type == NotificationUpdateSelfIfo) {
        m_update_ifo_dispatcher.emit();
        cout << "Emit" << endl;
    } else if(type == NotificationUpdateUserList) {
        m_update_user_list_dispatcher.emit();
    } else if(type == NotificationUserMsg) {
        m_receive_msg_dispatcher.emit();
    } else if(type == NotificationGroupMsg) {
        m_receive_msg_dispatcher.emit();
    } else {
        
    }
}

void MainWindow::OnNotificationUpdateSelfIfo()
{
    cout << "Update self ifo" << endl;
    m_self_ifo = ClientAPI::shared()->GetSelfIfo();

    string text_name = "User name: " + m_self_ifo.name;
    string text_id = "ID: " + std::to_string(m_self_ifo.id);

    m_label_name.set_text(text_name);
    m_label_id.set_text(text_id);
}

void MainWindow::OnNotificationUpdateUserList()
{
    cout << "Update user list" << endl;
    UpdateTreeview(ClientAPI::shared()->GetOnlineUsers());
}

void MainWindow::OnNotificationReceiveMsg()
{
    cout << "Receview Message" << endl;
    vector<Talk>& talks = ClientAPI::shared()->GetTalks();
    for(auto &talk : talks) {
        cout << "Talk Talk count count" << endl;
        bool isExist = false;
        if(talk.type == TALK_TYPE_GROUP) {
            cout << "Group message" << endl;
            if(m_group_window != NULL){
                m_group_window->UpdateChatContent(talk);
                if(!m_group_window->is_active() && talk.isReceviewNewMsg) {
                    cout << "Notif<y group unread." << endl;
                    OnUnreadMessage(0);
                }
                talk.isReceviewNewMsg = false;
                isExist = true;
                cout << "Try(isExist)" << isExist << endl;
            }
        } else {
            for(auto window : all_chat_windows) {
                if(talk.recv_id == window->m_recv_ifo.id) {
                    window->UpdateChatContent(talk);
                    isExist = true;
                    if(!window->is_active() && talk.isReceviewNewMsg) {
                        OnUnreadMessage(talk.recv_id);
                    }
                    talk.isReceviewNewMsg = false;
                }
            }
        }
        cout << "Try(isExist)" << isExist << endl;
        if(isExist == false && talk.isReceviewNewMsg) {
            cout << "Try(isExist)" << isExist << endl;
            cout << "Notify unread." << endl;
            // TODO(Ze): need more precise notification.
            OnUnreadMessage(talk.recv_id);
            talk.isReceviewNewMsg = false;
        }
    }
}


void MainWindow::OnInfoBarResponse(int response)
{
    m_info_bar.hide();
}

void MainWindow::OnUnreadMessage(unsigned long sender_id, string sender_name)
{
    m_label_info_bar.set_text("Get a new message\nFrom " + std::to_string(sender_id) + ".");
    m_info_bar.set_message_type(Gtk::MESSAGE_INFO);
    m_info_bar.show();
}
