#include "../include/Atomizer.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>
using namespace Jah;
using namespace std;

GtkWidget *Atomizer::box = nullptr;
GtkWidget *Atomizer::type_label = nullptr;
GtkWidget *Atomizer::title_label = nullptr;
GtkWidget *Atomizer::title = nullptr;
GtkWidget *Atomizer::summary_label = nullptr;
GtkWidget *Atomizer::summary = nullptr;
GtkWidget *Atomizer::body_label = nullptr;
GtkWidget *Atomizer::body = nullptr;
GtkWidget *Atomizer::notes_label = nullptr;
GtkWidget *Atomizer::notes = nullptr;
GtkWidget *Atomizer::footer_label = nullptr;
GtkWidget *Atomizer::footer = nullptr;
GtkWidget *Atomizer::issues = nullptr;
const char *Atomizer::types[] = {"Star",
                                 "Comet",
                                 "Nebula",
                                 "Pulsar",
                                 "Quasar",
                                 "Asteroid Belt",
                                 "Solar Flare",
                                 "Dwarf Planet",
                                 "Terraform",
                                 "Black Hole",
                                 "Wormhole",
                                 "Big Bang",
                                 "Launch",
                                 "Lightspeed",
                                 "Mission Control",
                                 "Spacewalk",
                                 "Moon Landing",
                                 "First Contact",
                                 "Interstellar Communication",
                                 "Solar Eclipse",
                                 "Supernova",
                                 "Meteor Shower",
                                 "Cosmic Dawn",
                                 "Solar Storm",
                                 "Lunar Transit",
                                 "Perihelion",
                                 "Aphelion",
                                 "White Dwarf",
                                 "Red Giant",
                                 "Neutron Star",
                                 "Binary Star",
                                 "Brown Dwarf",
                                 "Quark Star",
                                 "Rogue Planet",
                                 "Stellar Nursery",
                                 "Planetary Nebula",
                                 "Globular Cluster",
                                 "Void",
                                 "Gravity",
                                 "Dark Matter",
                                 "Time Dilation",
                                 "Spacetime",
                                 "Gravitational Lensing",
                                 "Cosmic String",
                                 "Quantum Fluctuation",
                                 "Hawking Radiation",
                                 "Quantum Entanglement",
                                 "Gravitational Redshift",
                                 "Space Probe",
                                 "Space Station",
                                 "Rocket Launch",
                                 "Space Elevator",
                                 nullptr};
const char *Atomizer::tickets[] = {"#0001", "#0003", nullptr};

void Atomizer::create_window_content(GtkWidget *window) {
  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_margin_start(box, 20);
  gtk_widget_set_margin_end(box, 20);
  gtk_widget_set_margin_top(box, 20);
  gtk_widget_set_margin_bottom(box, 20);
  type_label = gtk_label_new("Type of change");
  gtk_widget_set_halign(type_label, GTK_ALIGN_START);
  GtkWidget *dropdown = gtk_drop_down_new_from_strings(types);
  gtk_drop_down_set_enable_search(GTK_DROP_DOWN(dropdown), TRUE);

  // Build a filterable model for the issues dropdown
  GtkStringList *type_list = gtk_string_list_new(types);
  GtkExpression *string_type_expr =
      gtk_property_expression_new(GTK_TYPE_STRING_OBJECT, nullptr, "string");
  GtkStringFilter *type_str_filter = gtk_string_filter_new(string_type_expr);
  gtk_string_filter_set_match_mode(type_str_filter,
                                   GTK_STRING_FILTER_MATCH_MODE_SUBSTRING);
  GtkFilterListModel *type_filtered_model = gtk_filter_list_model_new(
      G_LIST_MODEL(type_list), GTK_FILTER(type_str_filter));

  GtkWidget *type_dropdown =
      gtk_drop_down_new(G_LIST_MODEL(type_filtered_model), string_type_expr);
  gtk_drop_down_set_enable_search(GTK_DROP_DOWN(type_dropdown), TRUE);
  gtk_widget_set_margin_bottom(type_dropdown, 10);
  gtk_widget_set_margin_top(dropdown, 10);
  gtk_widget_set_margin_bottom(dropdown, 10);
  gtk_box_append(GTK_BOX(box), type_dropdown);
  g_object_set_data(G_OBJECT(window), "type-dd", type_dropdown);

  title_label = gtk_label_new("Title of change");
  gtk_widget_set_halign(title_label, GTK_ALIGN_START);
  gtk_box_append(GTK_BOX(box), title_label);

  title = gtk_entry_new();
  gtk_widget_set_margin_bottom(title, 10);
  gtk_box_append(GTK_BOX(box), title);

  summary_label = gtk_label_new("Summary of changes");
  gtk_widget_set_halign(summary_label, GTK_ALIGN_START);
  gtk_box_append(GTK_BOX(box), summary_label);

  summary = gtk_entry_new();
  gtk_widget_set_margin_bottom(summary, 10);
  gtk_box_append(GTK_BOX(box), summary);

  body_label = gtk_label_new("Details of changes");
  gtk_widget_set_halign(body_label, GTK_ALIGN_START);
  gtk_box_append(GTK_BOX(box), body_label);

  body = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(body), GTK_WRAP_WORD_CHAR);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(body), TRUE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(body), TRUE);
  gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(body), FALSE);
  gtk_text_view_set_indent(GTK_TEXT_VIEW(body), 4);
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(body), 4);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(body), 4);
  gtk_text_view_set_top_margin(GTK_TEXT_VIEW(body), 4);
  gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(body), 4);

  GtkWidget *body_scroll = gtk_scrolled_window_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(body_scroll), body);
  gtk_widget_set_size_request(body_scroll, -1, 100);
  gtk_widget_set_margin_bottom(body_scroll, 10);
  gtk_box_append(GTK_BOX(box), body_scroll);

  notes_label = gtk_label_new("Notes");
  gtk_widget_set_halign(notes_label, GTK_ALIGN_START);

  gtk_box_append(GTK_BOX(box), notes_label);

  notes = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(notes), GTK_WRAP_WORD_CHAR);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(notes), TRUE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(notes), TRUE);
  gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(notes), FALSE);
  gtk_text_view_set_indent(GTK_TEXT_VIEW(notes), 4);
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(notes), 4);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(notes), 4);
  gtk_text_view_set_top_margin(GTK_TEXT_VIEW(notes), 4);
  gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(notes), 4);
  GtkWidget *notes_scroll = gtk_scrolled_window_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(notes_scroll), notes);
  gtk_widget_set_size_request(notes_scroll, -1, 100);
  gtk_widget_set_margin_bottom(notes_scroll, 10);
  gtk_box_append(GTK_BOX(box), notes_scroll);

  footer_label = gtk_label_new("Footer");
  gtk_widget_set_halign(footer_label, GTK_ALIGN_START);
  gtk_box_append(GTK_BOX(box), footer_label);

  footer = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(footer), GTK_WRAP_WORD_CHAR);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(footer), TRUE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(footer), TRUE);
  gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(footer), FALSE);
  gtk_text_view_set_indent(GTK_TEXT_VIEW(footer), 4);
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(footer), 4);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(footer), 4);
  gtk_text_view_set_top_margin(GTK_TEXT_VIEW(footer), 4);
  gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(footer), 4);
  GtkWidget *footer_scroll = gtk_scrolled_window_new();
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(footer_scroll), footer);
  gtk_widget_set_size_request(footer_scroll, -1, 100);
  gtk_widget_set_margin_bottom(footer_scroll, 10);
  gtk_box_append(GTK_BOX(box), footer_scroll);

  GtkWidget *issues_label = gtk_label_new("Issues");
  gtk_widget_set_halign(issues_label, GTK_ALIGN_START);
  gtk_box_append(GTK_BOX(box), issues_label);

  // Build a filterable model for the issues dropdown
  GtkStringList *issues_list = gtk_string_list_new(tickets);
  GtkExpression *string_expr =
      gtk_property_expression_new(GTK_TYPE_STRING_OBJECT, nullptr, "string");
  GtkStringFilter *issues_str_filter = gtk_string_filter_new(string_expr);
  gtk_string_filter_set_match_mode(issues_str_filter,
                                   GTK_STRING_FILTER_MATCH_MODE_SUBSTRING);
  GtkFilterListModel *issues_filtered_model = gtk_filter_list_model_new(
      G_LIST_MODEL(issues_list), GTK_FILTER(issues_str_filter));

  GtkWidget *i =
      gtk_drop_down_new(G_LIST_MODEL(issues_filtered_model), string_expr);
  gtk_drop_down_set_enable_search(GTK_DROP_DOWN(i), TRUE);
  gtk_widget_set_margin_bottom(i, 10);
  gtk_box_append(GTK_BOX(box), i);
  g_object_set_data(G_OBJECT(window), "issues-dd", i);

  GtkWidget *submit_button = gtk_button_new_with_label("Create atom");
  g_signal_connect(submit_button, "clicked",
                   G_CALLBACK(Atomizer::on_submit_clicked), window);
  gtk_widget_set_margin_top(submit_button, 20);
  gtk_box_append(GTK_BOX(box), submit_button);
  gtk_window_set_child(GTK_WINDOW(window), box);
}
void Atomizer::on_window_destroy(const GtkWidget *widget,
                                 const gpointer *data) {
  (void)widget;
  (void)data;
}

void Atomizer::on_submit_clicked(const GtkWidget *widget,
                                 const gpointer *data) {
  const GtkWindow *win = GTK_WINDOW(data);
  GtkDropDown *type_dropdown =
      GTK_DROP_DOWN(gtk_widget_get_first_child(GTK_WIDGET(box)));
  const auto type = gtk_drop_down_get_selected(type_dropdown);

  const auto *title_buff_text = gtk_entry_get_buffer(GTK_ENTRY(title));
  const auto *title_text =
      gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(title_buff_text));

  const auto *summary_buff_text = gtk_entry_get_buffer(GTK_ENTRY(summary));
  const auto *summary_text =
      gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(summary_buff_text));
  GtkTextBuffer *body_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(body));
  GtkTextIter body_start, body_end;
  gtk_text_buffer_get_bounds(body_buffer, &body_start, &body_end);
  const auto *body_text =
      gtk_text_buffer_get_text(body_buffer, &body_start, &body_end, FALSE);

  GtkTextBuffer *notes_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(notes));
  GtkTextIter notes_start, notes_end;
  gtk_text_buffer_get_bounds(notes_buffer, &notes_start, &notes_end);
  const auto *notes_text =
      gtk_text_buffer_get_text(notes_buffer, &notes_start, &notes_end, FALSE);

  GtkTextBuffer *footer_buffer =
      gtk_text_view_get_buffer(GTK_TEXT_VIEW(footer));
  GtkTextIter footer_start, footer_end;
  gtk_text_buffer_get_bounds(footer_buffer, &footer_start, &footer_end);
  const auto *footer_text = gtk_text_buffer_get_text(
      footer_buffer, &footer_start, &footer_end, FALSE);
  GtkDropDown *issue =
      GTK_DROP_DOWN(g_object_get_data(G_OBJECT(win), "issues-dd"));
  gpointer sel_item = gtk_drop_down_get_selected_item(issue);
  GtkStringObject *issue_str_obj =
      sel_item ? GTK_STRING_OBJECT(sel_item) : nullptr;
  const char *issue_text =
      issue_str_obj ? gtk_string_object_get_string(issue_str_obj) : "";

  cout << "Type: " << types[type] << endl;
  cout << "Issue: " << (issue_text ? issue_text : "") << endl;
  cout << "Title: " << title_text << endl;
  cout << "Summary: " << summary_text << endl;
  cout << "Body: " << body_text << endl;
  cout << "Notes: " << notes_text << endl;
  cout << "Footer: " << footer_text << endl;
  (void)widget;
  (void)data;
}

void Atomizer::setup_window_properties(GtkWidget *window) {
  gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);

  gtk_window_set_default_size(GTK_WINDOW(window), DEFAULT_WINDOW_WIDTH,
                              DEFAULT_WINDOW_HEIGHT);
  gtk_window_set_icon_name(GTK_WINDOW(window), APP_ID);

  gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
}
void Atomizer::on_activate(GtkApplication *app, gpointer *user_data) {
  try {
    // Create the main application window
    GtkWidget *window = gtk_application_window_new(app);
    if (!window) {
      throw std::runtime_error("Failed to create application window");
    }
    setup_window_properties(window);

    create_window_content(window);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy),
                     user_data);

    // Show the window
    gtk_window_present(GTK_WINDOW(window));
  } catch (const std::exception &e) {
    g_warning("Error activating application: %s", e.what());
  }
}