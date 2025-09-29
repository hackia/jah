#pragma once
#include <gtk/gtk.h>

namespace Jah {

constexpr auto APP_ID = "org.jah.flow";
constexpr auto WINDOW_TITLE = "Flow";
constexpr int DEFAULT_WINDOW_WIDTH = 800;
constexpr int DEFAULT_WINDOW_HEIGHT = 600;
constexpr int MIN_WINDOW_WIDTH = 400;
constexpr int MIN_WINDOW_HEIGHT = 300;

class Flow {
public:
  static void create_window_content(GtkWidget *window);
  static void on_window_destroy(const GtkWidget *widget, const gpointer *data);
  static void setup_window_properties(GtkWidget *window);
  static void on_activate(GtkApplication *app, gpointer *user_data);
};

} // namespace Jah