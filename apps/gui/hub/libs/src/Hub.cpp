#include "../include/Hub.hpp"
#include <stdexcept>

using namespace Jah;

void Hub::create_window_content(GtkWidget *window) {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_margin_start(box, 20);
  gtk_widget_set_margin_end(box, 20);
  gtk_widget_set_margin_top(box, 20);
  gtk_widget_set_margin_bottom(box, 20);
  GtkWidget *header = gtk_label_new(nullptr);
  gtk_label_set_markup(
      GTK_LABEL(header),
      "<span size='xx-large' weight='bold'>Welcome to Hub</span>");
  gtk_widget_set_margin_bottom(header, 20);
  gtk_box_append(GTK_BOX(box), header);
  gtk_window_set_child(GTK_WINDOW(window), box);
}
void Hub::on_window_destroy(const GtkWidget *widget, const gpointer *data) {
  (void)widget;
  (void)data;
}

void Hub::setup_window_properties(GtkWidget *window) {
  gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);

  gtk_window_set_default_size(GTK_WINDOW(window), DEFAULT_WINDOW_WIDTH,
                              DEFAULT_WINDOW_HEIGHT);

  gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
}
void Hub::on_activate(GtkApplication *app, gpointer *user_data) {
  try {
    GtkWidget *window = gtk_application_window_new(app);
    if (!window) {
      throw std::runtime_error("Failed to create application window");
    }
    setup_window_properties(window);

    create_window_content(window);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy),
                     user_data);

    gtk_window_present(GTK_WINDOW(window));
  } catch (const std::exception &e) {
    g_warning("Error activating application: %s", e.what());
  }
}
