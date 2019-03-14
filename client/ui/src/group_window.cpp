#include <iostream>
#include "group_window.h"
#include "client_api.h"
#include "utils.h"

GroupWindow::GroupWindow(Glib::RefPtr<Gtk::Application> app, UserIfo self_ifo) : m_app(app), m_self_ifo(self_ifo),
                                                                                 m_history_window(NULL)
{
    set_default_size(380, 560);
    set_title("Big Group");

    hbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
    add(*hbox);
    hbox->set_border_width(10);

    grid = Gtk::manage(new Gtk::Grid);
    grid->set_border_width(2);
    hbox->add(*grid);
    //Add all buttons
    button_clear=manage(new Gtk::Button("Clear"));
    button_send=manage(new Gtk::Button("Send"));
    button_history=manage(new Gtk::Button("History"));
    //button_file=manage(new Gtk::Button("File"));
   //Connect signals:
    m_InfoBar.signal_response().connect(sigc::mem_fun(*this,
              &GroupWindow::on_infobar_response) );
    button_send->signal_clicked().connect(sigc::mem_fun(*this,
              &GroupWindow::on_button_send) );
    button_clear->signal_clicked().connect(sigc::mem_fun(*this,
              &GroupWindow::on_button_clear) );
    //button_file->signal_clicked().connect(sigc::mem_fun(*this,
              //&GroupWindow::on_button_file) );
    button_history->signal_clicked().connect(sigc::mem_fun(*this,
              &GroupWindow::on_button_history) );            

    //Add box1, box2, box3, box4 ,box5
    //box5=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    //box5->set_hexpand(true);
    //box5->set_vexpand(true);
    //grid->attach(*box5, 0, 0, 140, 10);


    box1=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    box1->set_hexpand(true);
    box1->set_vexpand(true); 
    box1->set_border_width(2);
    grid->attach(*box1, 0, 0, 140, 90);
    

    //box2=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    //box2->set_hexpand(true);
    //box2->set_vexpand(true);
    //box2->set_border_width(2);
    //grid->attach(*box2, 140, 0, 30, 100);

    box3=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    box3->set_hexpand(true);
    box3->set_vexpand(true);
    box3->set_border_width(2);
    grid->attach(*box3, 0, 100, 140, 40);

    //box4=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    //box4->set_hexpand(true);
    //box4->set_vexpand(true);
    //box4->set_border_width(2);
    //grid->attach(*box4, 140, 100, 30, 40);
    //Create buffer and Add tags
    buffer_chat = Gtk::TextBuffer::create();
    buffer_send = Gtk::TextBuffer::create();
    buffer_chat->set_text("");

    tag_left=buffer_chat->create_tag();
    tag_left->property_justification()= Gtk::Justification::JUSTIFY_LEFT;

    tag_small=buffer_chat->create_tag();
    tag_small->property_size_points()=7;

    tag_right=buffer_chat->create_tag();
    tag_right->property_justification()= Gtk::Justification::JUSTIFY_RIGHT;

    tag_right_small=buffer_chat->create_tag();
    tag_right_small->property_justification()= Gtk::Justification::JUSTIFY_RIGHT;
    tag_right_small->property_size_points()=7;

    tag_center=buffer_chat->create_tag();
    tag_center->property_justification()=Gtk::Justification::JUSTIFY_CENTER;

    tag_center_small=buffer_chat->create_tag();
    tag_center_small->property_justification()=Gtk::Justification::JUSTIFY_CENTER;
    tag_center_small->property_size_points()=7;


    tag_grey=buffer_chat->create_tag();
    tag_grey->property_foreground()="grey";

    tag_center_small_grey=buffer_chat->create_tag();
    tag_center_small_grey->property_size_points()=7;
    tag_center_small_grey->property_foreground()="grey";
    tag_center_small_grey->property_justification()=Gtk::Justification::JUSTIFY_CENTER;
    
    //Finish box5
    //label3.set_justify(Gtk::JUSTIFY_LEFT);
    //box5->pack_start(label3);
    //Finish box1
    window_chat=manage(new Gtk::ScrolledWindow );
    box1->pack_start(*window_chat);
    textview_chat=manage(new Gtk::TextView );
    window_chat->add(*textview_chat);
    window_chat->set_policy(Gtk::POLICY_AUTOMATIC,
         Gtk::POLICY_AUTOMATIC);
    textview_chat->set_buffer(buffer_chat);
    textview_chat->set_editable(FALSE);

    buttonbox1=manage(new Gtk::ButtonBox);
    box1->pack_start(*buttonbox1,Gtk::PACK_SHRINK);
    buttonbox1->pack_start(*button_history, Gtk::PACK_SHRINK);
    //buttonbox1->pack_start(*button_file, Gtk::PACK_SHRINK);
    buttonbox1->set_spacing(1);
    buttonbox1->set_layout(Gtk::BUTTONBOX_START);

    //Finish box2
    /*
    label1=manage(new Gtk::Label("Information puts here"));
    frame_sender=manage(new Gtk::Frame("SENDER") );
    box2->pack_start(*frame_sender);
    frame_sender->add(*label1);

    label2=manage(new Gtk::Label("Information puts here"));
    frame_receiver=manage(new Gtk::Frame("RECEIVER") );
    box2->pack_start(*frame_receiver);
    frame_receiver->add(*label2);
    */
    //Finish box3

     auto infoBarContainer =
    dynamic_cast<Gtk::Container*>(m_InfoBar.get_content_area());
    if (infoBarContainer)
    infoBarContainer->add(m_Message_Label);
    m_InfoBar.add_button("_OK", 0);
    box3->pack_start(m_InfoBar, Gtk::PACK_SHRINK);

    window_send=manage(new Gtk::ScrolledWindow );
    box3->pack_start(*window_send);
    textview_send=manage(new Gtk::TextView );
    window_send->add(*textview_send);
    window_send->set_policy(Gtk::POLICY_AUTOMATIC,
         Gtk::POLICY_AUTOMATIC);
    textview_send->set_buffer(buffer_send);

    buttonbox2=manage(new Gtk::ButtonBox);
    box3->pack_start(*buttonbox2,Gtk::PACK_SHRINK);
    buttonbox2->pack_start(*button_clear, Gtk::PACK_SHRINK);
    buttonbox2->pack_start(*button_send, Gtk::PACK_SHRINK);
    buttonbox2->set_spacing(1);
    buttonbox2->set_layout(Gtk::BUTTONBOX_END);

    //Finish box4
    /*
    m_connection_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,
              &GroupWindow::on_timeout), 50 );
    box4->pack_start(m_ProgressBar, Gtk::PACK_SHRINK, 5);
    
    */
    show_all_children();
    m_InfoBar.hide();
    //m_ProgressBar.hide();
  
}
GroupWindow::~GroupWindow()
{
}
void GroupWindow::UpdateChatContent(Talk talk)
{
    std::cout << "Update group message: count = " << talk.msgs.size() << std::endl;
    m_talk = talk;

    buffer_chat->set_text("");
    // Update all message
    for(auto msg : talk.msgs) {
      // My message
      if(msg.sender_id == m_self_ifo.id) {
          end=buffer_chat->end();
          buffer_chat->insert_with_tag(end, msg.date, tag_right_small);
          end=buffer_chat->end();
          buffer_chat->insert_with_tag(end, "From: " + std::to_string(msg.sender_id) + "\n" ,tag_right_small);
          end=buffer_chat->end();
          buffer_chat->insert_with_tag(end,msg.content + "\n\n",tag_right);
      } else {
          end=buffer_chat->end();
          buffer_chat->insert_with_tag(end, msg.date, tag_small);
          end=buffer_chat->end();
          buffer_chat->insert_with_tag(end, "From: " + std::to_string(msg.sender_id) + "\n" ,tag_small);
          end=buffer_chat->end();
          buffer_chat->insert_with_tag(end,msg.content + "\n\n",tag_left);
      }
    }
    while(gtk_events_pending())
      gtk_main_iteration_do(false);

    auto it = buffer_chat->end();
    textview_chat->scroll_to(it);
}
void GroupWindow::on_button_send()
{
     // buffer_send->set_text("message has send!");
    std::cout << buffer_send->get_text() << std::endl;
    ClientAPI::shared()->SendMsg(true, buffer_send->get_text(), GetCurrentTime(), 
                                 m_self_ifo.id, 0);
    end=buffer_chat->end();
    buffer_chat->insert_with_tag(end, GetCurrentTime(), tag_right_small);
    end=buffer_chat->end();
    buffer_chat->insert_with_tag(end, "From: " + std::to_string(m_self_ifo.id) + "\n" ,tag_right_small);
    end=buffer_chat->end();
    buffer_chat->insert_with_tag(end,buffer_send->get_text() + "\n\n",tag_right);

    buffer_send->set_text("");
}

void GroupWindow::on_infobar_response(int)
{
  // Clear the message and hide the info bar:
  m_Message_Label.set_text("");
  m_InfoBar.hide();
}

void GroupWindow::on_button_clear()
{
    buffer_send->set_text("");//clear buffer send
    m_Message_Label.set_text("Cleared the text.");
    m_InfoBar.set_message_type(Gtk::MESSAGE_INFO);
    m_InfoBar.show();
}

void GroupWindow::on_button_history()
{
  //show history window
   //show history window
    if(m_history_window != NULL) {
    m_history_window->UpdateHistoryContent(m_talk);
      m_history_window->show();
    } else {
      m_history_window = new HistoryWindow(m_self_ifo, m_self_ifo, true);
      m_app->add_window(*m_history_window);
      m_history_window->UpdateHistoryContent(m_talk);
      m_history_window->show();
    }
}
