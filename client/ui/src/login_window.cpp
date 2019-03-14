#include <iostream>
#include <string>
#include <exception>  

#include "login_window.h"
#include "main_window.h"
#include "utils.h"

LoginWindow::LoginWindow(Glib::RefPtr<Gtk::Application> app) : m_app(app)
{
    set_default_size(200, 100);
    set_position(Gtk::WIN_POS_CENTER);
    //TODO(Ze): Disable vertical resizing
    
    set_title("NeuTalk");

    Gtk::Box *vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    add(*vbox);
    
    // ---- Menubar ---- //
    Gtk::MenuBar *menubar = Gtk::manage(new Gtk::MenuBar());
    vbox->pack_start(*menubar, Gtk::PACK_SHRINK, 0);

    Gtk::MenuItem *menuitem_file = Gtk::manage(new Gtk::MenuItem("_File", true));
    menubar->append(*menuitem_file);
    Gtk::Menu *filemenu = Gtk::manage(new Gtk::Menu());
    menuitem_file->set_submenu(*filemenu);
    Gtk::MenuItem *menuitem_quit = Gtk::manage(new Gtk::MenuItem("_Quit", true));
    menuitem_quit->signal_activate().connect(sigc::mem_fun(*this, &LoginWindow::OnQuitClicked));
    filemenu->append(*menuitem_quit);

    Gtk::Grid *grid = Gtk::manage(new Gtk::Grid);
    grid->set_border_width(10);
    grid->set_row_spacing(5);
    vbox->add(*grid);
    // ---------------- //

    Gtk::Label *lusername = Gtk::manage(new Gtk::Label("User ID: "));
    grid->attach(*lusername, 0, 0, 1, 1);

    Gtk::Label *lpassword = Gtk::manage(new Gtk::Label("Password: "));
    grid->attach(*lpassword, 0, 1, 1, 1);

    Gtk::Entry *eusername = Gtk::manage(new Gtk::Entry());
    eusername->set_hexpand(true);
    grid->attach(*eusername, 1, 0, 2, 1);

    Gtk::Entry *epassword = Gtk::manage(new Gtk::Entry());
    epassword->set_hexpand(true);
    epassword->set_visibility(false);
    grid->attach(*epassword, 1, 1, 2, 1);

    Gtk::Button *blogin = Gtk::manage(new Gtk::Button("Login"));
    blogin->signal_clicked().connect(sigc::bind<Gtk::Entry*, Gtk::Entry*>(sigc::mem_fun(*this, &LoginWindow::OnLoginClicked), eusername, epassword));
    grid->attach(*blogin, 2, 2, 1, 1);

    // Set up delegate
    ClientAPI::shared()->login_delegate = this;

    // Connect the handler to the dispatcher.
    m_login_dispatcher.connect(sigc::mem_fun(*this, &LoginWindow::OnNotificationLogin));
    m_register_dispatcher.connect(sigc::mem_fun(*this, &LoginWindow::OnNotificationRegister));
    
    vbox->show_all();
}

LoginWindow::~LoginWindow()
{
    
}

void LoginWindow::OnLoginClicked(Gtk::Entry *euname, Gtk::Entry *epword)
{
    if(ClientAPI::shared()->GetStatus() == -1) {
        Gtk::MessageDialog dlg("Can not connect to server.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dlg.set_title("Internet Error");
        dlg.run();
        return;
    }
    unsigned long id;
    string pwd = epword->get_text();

    try {
        id = std::stoul(euname->get_text());
    } catch(std::exception& e) {
        ShowDialog("Invalid ID!", "Login fail", Gtk::MESSAGE_ERROR);
        return;
    }

    ClientAPI::shared()->Login(id, pwd);
}

void LoginWindow::OnQuitClicked()
{
    hide();
}

void LoginWindow::Notify(NotificationType type)
{
    std::cout << "Notify" << std::endl;
    if(type == NotificationLogin) {
        m_login_dispatcher.emit();
    } else if(type == NotificationRegister) {
        m_register_dispatcher.emit();
    } else {
        
    }
}

void LoginWindow::OnNotificationLogin()
{
    if(ClientAPI::shared()->GetLoginResult().status == 0)
    {
        std::cout << "Show main window" << std::endl;
        main_window = new MainWindow(m_app);
        std::cout << "Init Done" << std::endl;
        m_app->add_window(*main_window);
        main_window->show();
        this->hide();
        // m_app->remove_window(*this);
    }
    else
    {
        Gtk::MessageDialog dlg("Unknown username or password.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dlg.set_title("Login fail");
        dlg.run();
    }
}

void LoginWindow::OnNotificationRegister()
{

}