#include "history_window.h"
#include <iostream>

HistoryWindow::HistoryWindow(UserIfo sender_ifo, UserIfo recv_ifo, bool is_group) : m_VBox(Gtk::ORIENTATION_VERTICAL), m_SearchButton("Search"),
                                 m_sender_ifo(sender_ifo), m_recv_ifo(recv_ifo), m_is_group(is_group)
{
  set_title("History");
  set_border_width(5);
  set_default_size(330, 420);

  //加入盒子包含所有空间
  add(m_VBox);

  //加入文本框
  m_ScrolledWindow.add(m_TextView);
  m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  m_VBox.pack_start(m_ScrolledWindow);

  //加入搜索盒子，包含搜索框与按钮
  m_VBox.pack_start(m_SearchBox, Gtk::PACK_SHRINK);
  m_SearchBox.pack_start(m_SearchBar);
  m_SearchBox.pack_start(m_SearchEntry);

  //点击搜索按钮
  m_SearchButton.signal_clicked().connect(sigc::mem_fun(*this,&HistoryWindow::on_button_clicked));

  //缓冲区
  m_refTextBuffer = Gtk::TextBuffer::create();
  m_refTextBuffer->set_text("");
  m_TextView.set_buffer(m_refTextBuffer);

  m_SearchBox.pack_start(m_SearchButton, Gtk::PACK_SHRINK);
  m_SearchBox.set_margin_top(15);
  m_SearchBox.set_margin_bottom(10);
  m_SearchEntry.set_margin_right(40);
  m_SearchButton.set_margin_right(20);

  show_all_children();
}

HistoryWindow::~HistoryWindow()
{
}

void HistoryWindow::UpdateHistoryContent(Talk talk)
{
    // Load previous history from file

    // Load current history
    Gtk::TextIter iter;
    for(auto msg : talk.msgs) {
      iter = m_refTextBuffer->end();
      m_refTextBuffer->insert(iter, std::to_string(msg.sender_id) + ": " + msg.date);
      iter = m_refTextBuffer->end();
      m_refTextBuffer->insert(iter, msg.content + "\n\n");
    }
}

void HistoryWindow::on_button_clicked()
{
  //获取搜索框内的值输出
  const auto search_text = m_SearchEntry.get_text();
  std::cout << search_text << std::endl;
}

