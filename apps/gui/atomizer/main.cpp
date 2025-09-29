#include "gtk/gtk.h"
#include <stdexcept>
// Constants
constexpr auto APP_ID = "org.hackia.atomizer";
constexpr auto WINDOW_TITLE = "Atomizer";
constexpr int DEFAULT_WINDOW_WIDTH = 800;  // Increased width
constexpr int DEFAULT_WINDOW_HEIGHT = 600; // Increased height
constexpr int MIN_WINDOW_WIDTH = 400;
constexpr int MIN_WINDOW_HEIGHT = 300;

// Forward declarations
static void create_window_content(GtkWidget *window);
static void on_window_destroy(const GtkWidget *widget, const gpointer *data);
static void setup_window_properties(GtkWidget *window);

static void on_activate(GtkApplication *app, gpointer *user_data) {
  try {
    // Create the main application window
    GtkWidget *window = gtk_application_window_new(app);
    if (!window) {
      throw std::runtime_error("Failed to create application window");
    }

    // Configure window properties
    setup_window_properties(window);

    // Set up window content
    create_window_content(window);

    // Connect destroy signal
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy),
                     user_data);

    // Show the window
    gtk_window_present(GTK_WINDOW(window));
  } catch (const std::exception &e) {
    g_warning("Error activating application: %s", e.what());
  }
}

static void setup_window_properties(GtkWidget *window) {
  gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);

  gtk_window_set_default_size(GTK_WINDOW(window), DEFAULT_WINDOW_WIDTH,
                              DEFAULT_WINDOW_HEIGHT);

  gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
}

static void create_window_content(GtkWidget *window) {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_margin_start(box, 20);
  gtk_widget_set_margin_end(box, 20);
  gtk_widget_set_margin_top(box, 20);
  gtk_widget_set_margin_bottom(box, 20);

  GtkWidget *header = gtk_label_new(nullptr);
  gtk_label_set_markup(
      GTK_LABEL(header),
      "<span size='xx-large' weight='bold'>Welcome to Atomizer</span>");
  gtk_widget_set_margin_bottom(header, 20);

  // Create description label
  GtkWidget *description =
      gtk_label_new("A powerful tool for managing your workflow");
  gtk_label_set_selectable(GTK_LABEL(description), TRUE);

  gtk_box_append(GTK_BOX(box), header);
  gtk_box_append(GTK_BOX(box), description);

  // Set box as the window's child
  gtk_window_set_child(GTK_WINDOW(window), box);
}

static void on_window_destroy(const GtkWidget *widget, const gpointer *data) {
  (void)widget;
  (void)data;
}

int main(const int argc, char **argv) {
  try {
    // Create the application
    GtkApplication *app =
        gtk_application_new(APP_ID, G_APPLICATION_DEFAULT_FLAGS);
    if (!app) {
      throw std::runtime_error("Failed to create GTK application");
    }

    // Connect signal handlers
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    // Run the application
    const int status = g_application_run(G_APPLICATION(app), argc, argv);

    // Cleanup
    g_object_unref(app);

    return status;

  } catch (const std::exception &e) {
    g_critical("Fatal error: %s", e.what());
    return 1;
  }
}