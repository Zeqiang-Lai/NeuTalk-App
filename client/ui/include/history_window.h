#ifndef HISTORY_WINDOW_H_
#define HISTORY_WINDOW_H_

#include <gtkmm.h>
#include "neu_talk_protocol.h"
#include "talk.h"

class HistoryWindow : public Gtk::Window
{
public:
  HistoryWindow(UserIfo sender_ifo, UserIfo recv_ifo, bool is_group);
  virtual ~HistoryWindow();
  void UpdateHistoryContent(Talk talk);

protected:
  void on_button_clicked();

private:
  UserIfo m_sender_ifo;
  UserIfo m_recv_ifo;
  bool m_is_group;

  Gtk::Box m_VBox;

  Gtk::TextView title_TextView;

  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::TextView m_TextView;

  Gtk::Box m_SearchBox;
  Gtk::SearchBar m_SearchBar;
  Gtk::SearchEntry m_SearchEntry;
  Gtk::Button m_SearchButton;

  Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer;
};

#endif // HISTORY_WINDOW_H_