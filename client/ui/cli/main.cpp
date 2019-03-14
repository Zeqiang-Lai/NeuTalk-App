#include <gtkmm.h>
#include "login_window.h"
#include "config.h"

int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "com.gtkmm.tutorial4.base");

    Config config;
    if(config.Load() == 0) {
        ClientAPI::shared()->ConnectToServer(config.GetIP(), config.GetPort());
    } else {
        return -1;
    }
    LoginWindow window(app);

    return app->run(window);
}
