#pragma once
#include <gtk/gtk.h>
#include <vector>
using namespace std;
namespace Jah {

constexpr auto APP_ID = "org.jah.atomizer";
constexpr auto WINDOW_TITLE = "Atomizer";
constexpr int DEFAULT_WINDOW_WIDTH = 800;  // Increased width
constexpr int DEFAULT_WINDOW_HEIGHT = 600; // Increased height
constexpr int MIN_WINDOW_WIDTH = 400;
constexpr int MIN_WINDOW_HEIGHT = 300;

class Atomizer {
public:
  Atomizer();
  static void create_window_content(GtkWidget *window);
  static void on_window_destroy(const GtkWidget *widget, const gpointer *data);
  static void on_submit_clicked(const GtkWidget *widget, const gpointer *data);
  static void setup_window_properties(GtkWidget *window);
  static void on_activate(GtkApplication *app, gpointer *user_data);

private:
  static const char *types[];
  static const char *tickets[];
  static GtkWidget *box;
  static GtkWidget *type_label;
  static GtkWidget *title_label;
  static GtkWidget *title;
  static GtkWidget *summary_label;
  static GtkWidget *summary;
  static GtkWidget *body_label;
  static GtkWidget *body;
  static GtkWidget *notes_label;
  static GtkWidget *notes;
  static GtkWidget *footer_label;
  static GtkWidget *footer;
  static GtkWidget *issues;
};

} // namespace Jah