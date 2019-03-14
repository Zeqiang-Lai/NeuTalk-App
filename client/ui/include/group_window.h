#ifndef GROUP_WINDOW_H_
#define GROUP_WINDOW_H_

#include <gtkmm.h>
#include "neu_talk_protocol.h"
#include "talk.h"
#include "history_window.h"

class GroupWindow : public Gtk::Window
{
  public:
    GroupWindow(Glib::RefPtr<Gtk::Application> app, UserIfo self_ifo);
    virtual ~GroupWindow();
    void UpdateChatContent(Talk talk);

  protected:

    //Signal handlers:
    void on_button_send();
    void on_button_clear();
    void on_button_history();
    //void on_button_file();
    void on_infobar_response(int response);
    //bool on_timeout();

  private:
    
    Glib::RefPtr<Gtk::Application> m_app;

    UserIfo m_self_ifo;

    HistoryWindow *m_history_window;

    Talk m_talk;

    //Child widgets
    Gtk::Grid *grid;
    //main box horizoncal
    Gtk::Box *hbox;
    //vertial box
    Gtk::Box *box1;
    Gtk::Box *box2;
    Gtk::Box *box3;
    Gtk::Box *box4;
    Gtk::Box *box5;

    Gtk::ScrolledWindow *window_chat;
    Gtk::ScrolledWindow *window_send;
    Gtk::TextView *textview_chat;
    Gtk::TextView *textview_send;

    Glib::RefPtr<Gtk::TextBuffer> buffer_chat, buffer_send;

    Gtk::ButtonBox *buttonbox1;
    Gtk::ButtonBox *buttonbox2;
    Gtk::Button *button_send, *button_clear, *button_file, *button_history;

    //frame and labels
    Gtk::Frame *frame_sender, *frame_receiver;
    Gtk::Label *label1, *label2, *label4;
    Gtk::InfoBar m_InfoBar;
    Gtk::Label m_Message_Label;
    //Progress bar
    //Gtk::ProgressBar m_ProgressBar;
    //sigc::connection m_connection_timeout;
    //bool m_bActivityMode;
    //Tags
    Glib::RefPtr<Gtk::TextBuffer::Tag> tag_center;
    Glib::RefPtr<Gtk::TextBuffer::Tag> tag_left;
    Glib::RefPtr<Gtk::TextBuffer::Tag> tag_right;
    Glib::RefPtr<Gtk::TextBuffer::Tag> tag_right_small;
    Glib::RefPtr<Gtk::TextBuffer::Tag> tag_center_small;
    Glib::RefPtr<Gtk::TextBuffer::Tag> tag_center_small_grey;
    Glib::RefPtr<Gtk::TextBuffer::Tag> tag_small;
    Glib::RefPtr<Gtk::TextBuffer::Tag> tag_grey;

    //iterator
    Gtk::TextIter start, end; //end 用于在后面插入新的消息
};
#endif //GROUP_WINDOW_H_