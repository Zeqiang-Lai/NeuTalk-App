#include "utils.h"

void ShowDialog(std::string msg, std::string title, 
                Gtk::MessageType type, 
                Gtk::ButtonsType buttons)
{
    Gtk::MessageDialog dlg(msg, false, type, buttons, true);
    dlg.set_title(title);
    dlg.run();
}

std::string GetCurrentTime()
{
	time_t now;
	time(&now);
	return ctime(&now);
}