#ifndef LOGIN_WINDOW_H_
#define LOGIN_WINDOW_H_

#include <gtkmm.h>

#include "client_api.h"
#include "main_window.h"

class LoginWindow : public Gtk::Window, public ClientDelegate
{
    public:
        LoginWindow(Glib::RefPtr<Gtk::Application> app);
        virtual ~LoginWindow();
    protected:
        void OnLoginClicked(Gtk::Entry *euname, Gtk::Entry *epword);
        void OnQuitClicked();
        
        void Notify(NotificationType type);

        // Dispatcher handler.
        void OnNotificationLogin();
        void OnNotificationRegister();

    private:
        MainWindow *main_window;
        Glib::RefPtr<Gtk::Application> m_app;

        Glib::Dispatcher m_login_dispatcher;
        Glib::Dispatcher m_register_dispatcher;
};

#endif // LOGIN_WINDOW_H_