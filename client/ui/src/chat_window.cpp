#include <iostream>
#include "chat_window.h"
#include "client_api.h"
#include "utils.h"
#include "history_window.h"

ChatWindow::ChatWindow(Glib::RefPtr<Gtk::Application> app, UserIfo recv_ifo, UserIfo self_ifo) : 
  m_app(app), m_recv_ifo(recv_ifo), m_self_ifo(self_ifo),label3("  To be or not to be? It is a question."),
  m_history_window(NULL)
{
    set_default_size(680, 560);
    set_title("NeuTalk");

    hbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
    add(*hbox);
    hbox->set_border_width(2);

    grid = Gtk::manage(new Gtk::Grid);
    grid->set_border_width(2);
    hbox->add(*grid);

    //Add all buttons
    button_clear=manage(new Gtk::Button("Clear"));
    button_send=manage(new Gtk::Button("Send"));
    button_history=manage(new Gtk::Button("History"));
    button_file=manage(new Gtk::Button("File"));

    //Connect signals:
    m_InfoBar.signal_response().connect(sigc::mem_fun(*this,
              &ChatWindow::on_infobar_response) );
    button_send->signal_clicked().connect(sigc::mem_fun(*this,
              &ChatWindow::on_button_send) );
    button_clear->signal_clicked().connect(sigc::mem_fun(*this,
              &ChatWindow::on_button_clear) );
    button_file->signal_clicked().connect(sigc::mem_fun(*this,
              &ChatWindow::on_button_file) );
    button_history->signal_clicked().connect(sigc::mem_fun(*this,
              &ChatWindow::on_button_history) );            

    //Add box1, box2, box3, box4 ,box5
    box5=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    box5->set_hexpand(true);
    box5->set_vexpand(true);
    grid->attach(*box5, 0, 0, 140, 10);


    box1=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    box1->set_hexpand(true);
    box1->set_vexpand(true); 
    box1->set_border_width(2);
    grid->attach(*box1, 0, 10, 140, 90);
    

    box2=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    box2->set_hexpand(true);
    box2->set_vexpand(true);
    box2->set_border_width(2);
    grid->attach(*box2, 140, 0, 30, 100);

    box3=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    box3->set_hexpand(true);
    box3->set_vexpand(true);
    box3->set_border_width(2);
    grid->attach(*box3, 0, 100, 140, 40);

    box4=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    box4->set_hexpand(true);
    box4->set_vexpand(true);
    box4->set_border_width(2);
    grid->attach(*box4, 140, 100, 30, 20);

    box6=Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    box6->set_hexpand(true);
    box6->set_vexpand(true);
    box6->set_border_width(2);
    grid->attach(*box6, 140, 120, 30, 20);

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


    //initinal buffer
    // fill_buffers();
    
    //Finish box5
    label3.set_justify(Gtk::JUSTIFY_LEFT);
    label3.set_halign(Gtk::ALIGN_START);
    frame_left=manage(new Gtk::Frame);
    frame_left->add(label3);
    box5->pack_start(*frame_left);

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
    buttonbox1->pack_start(*button_file, Gtk::PACK_SHRINK);
    buttonbox1->set_spacing(1);
    buttonbox1->set_layout(Gtk::BUTTONBOX_START);

    //Finish box2
    
    recv_ifo_label=manage(new Gtk::Label("Information puts here\nInformation puts here\nInformation puts here\n"));
    sender_ifo_label=manage(new Gtk::Label("Information puts here\nInformation puts here\nInformation puts here\n"));
    
    frame_receiver=manage(new Gtk::Frame("Receiver Ifo") );
    frame_sender=manage(new Gtk::Frame("Sender Ifo") );

    frame_receiver->add(*recv_ifo_label);
    frame_sender->add(*sender_ifo_label);

    box2->pack_start(*frame_receiver);
    box2->pack_start(*frame_sender);

    //Finish box3
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

    auto infoBarContainer =
    dynamic_cast<Gtk::Container*>(m_InfoBar.get_content_area());
    if (infoBarContainer)
    infoBarContainer->add(m_Message_Label);
    m_InfoBar.add_button("_OK", 0);
    


    //Finish box6
    m_connection_timeout = Glib::signal_timeout().connect(sigc::mem_fun(*this,
              &ChatWindow::on_timeout), 50 );
    box6->pack_start(m_ProgressBar, Gtk::PACK_SHRINK, 5);
    

    // Set up right panel
    std::string str_recv_ifo = "Name: " + m_recv_ifo.name + "\n" +
                          "ID: " + std::to_string(m_recv_ifo.id) + "\n" +
                          "IP: " + m_recv_ifo.ip;
    recv_ifo_label->set_text(str_recv_ifo);
    std::string str_sender_ifo = "Name: " + m_self_ifo.name + "\n" +
                          "ID: " + std::to_string(m_self_ifo.id) + "\n" +
                          "IP: " + m_self_ifo.ip;
    sender_ifo_label->set_text(str_sender_ifo);

    show_all_children();
    m_InfoBar.hide();
    m_ProgressBar.hide();
}
ChatWindow::~ChatWindow()
{
}
void ChatWindow::UpdateChatContent(Talk talk)
{
    buffer_chat->set_text("");
    m_talk = talk;
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
void ChatWindow::on_button_send()
{
    // buffer_send->set_text("message has send!");
    std::cout << buffer_send->get_text() << std::endl;
    ClientAPI::shared()->SendMsg(false, buffer_send->get_text(), GetCurrentTime(), 
                                 m_self_ifo.id, m_recv_ifo.id);
    end=buffer_chat->end();
    buffer_chat->insert_with_tag(end, GetCurrentTime(), tag_right_small);
    end=buffer_chat->end();
    buffer_chat->insert_with_tag(end, "From: " + std::to_string(m_self_ifo.id) + "\n" ,tag_right_small);
    end=buffer_chat->end();
    buffer_chat->insert_with_tag(end,buffer_send->get_text() + "\n\n",tag_right);

    buffer_send->set_text("");
}

void ChatWindow::on_infobar_response(int)
{
    // Clear the message and hide the info bar:
    m_Message_Label.set_text("");
    m_InfoBar.hide();
}

void ChatWindow::on_button_clear()
{
    buffer_send->set_text("");//clear buffer send
    m_Message_Label.set_text("Cleared the text.");
    m_InfoBar.set_message_type(Gtk::MESSAGE_INFO);
    m_InfoBar.show();
}
void ChatWindow::on_button_file()
{
    //show progress bar
      Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    //Add filters, so that only certain file types can be selected:

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Text files");
    filter_text->add_mime_type("text/plain");
    dialog.add_filter(filter_text);

    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("C/C++ files");
    filter_cpp->add_mime_type("text/x-c");
    filter_cpp->add_mime_type("text/x-c++");
    filter_cpp->add_mime_type("text/x-c-header");
    dialog.add_filter(filter_cpp);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog.add_filter(filter_any);

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    //Handle the response:
    switch(result)
    {
      case(Gtk::RESPONSE_OK):
      {
        std::cout << "Open clicked." << std::endl;

        //Notice that this is a std::string, not a Glib::ustring.
        std::string filename = dialog.get_filename();
        std::cout << "File selected: " <<  filename << std::endl;
        break;
      }
      case(Gtk::RESPONSE_CANCEL):
      {
        std::cout << "Cancel clicked." << std::endl;
        break;
      }
      default:
      {
        std::cout << "Unexpected button clicked." << std::endl;
        break;
      }
    }
    m_ProgressBar.show();
}
void ChatWindow::on_button_history()
{
    //show history window
    if(m_history_window != NULL) {
      m_history_window->UpdateHistoryContent(m_talk);
      m_history_window->show();
    } else {
      m_history_window = new HistoryWindow(m_self_ifo, m_recv_ifo, false);
      m_app->add_window(*m_history_window);
      m_history_window->UpdateHistoryContent(m_talk);
      m_history_window->show();
    }
}
bool ChatWindow::on_timeout()
{
    if(m_bActivityMode)
      m_ProgressBar.pulse();
    else
    {
      double new_val = m_ProgressBar.get_fraction() + 0.01;

      if(new_val > 1.0)
        new_val = 0.0;

      //Set the new value:
      m_ProgressBar.set_fraction(new_val);
    }

    //As this is a timeout function, return true so that it
    //continues to get called
    return true;
}