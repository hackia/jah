#include "../libs/include/Hub.hpp"
#include <gtk/gtk.h>
#include <stdexcept>

using namespace Jah;
int main(const int argc, char **argv) {
  try {
    // Create the application
    GtkApplication *app =
        gtk_application_new(APP_ID, G_APPLICATION_DEFAULT_FLAGS);
    if (!app) {
      throw std::runtime_error("Failed to create GTK application");
    }
    g_signal_connect(app, "activate", G_CALLBACK(Hub::on_activate), NULL);

    const int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
  } catch (const std::exception &e) {
    g_critical("Fatal error: %s", e.what());
    return 1;
  }
}