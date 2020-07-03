#include<stdio.h>
#include<gmodule.h>
#include<gtk/gtk.h>
#include"wav_reader.h"

// The following struct is used to store a filename and a window instance
typedef struct window_app_t {
    GtkWidget* window;
    GtkApplication* app;
} window_app_t;

gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data) {
  guint width, height;
  GdkRGBA color;
  GtkStyleContext *context;

  context = gtk_widget_get_style_context(widget);

  width = gtk_widget_get_allocated_width(widget);
  height = gtk_widget_get_allocated_height(widget);

  gtk_render_background(context, cr, 0, 0, width, height);

  /* cairo_arc(cr, */
  /*           width / 2.0, height / 2.0, */
  /*           MIN (width, height) / 2.0, */
  /*           0, 2 * G_PI); */

  cairo_move_to(cr, width/2, height);
  cairo_line_to(cr, 0, height/2);
  cairo_line_to(cr, width/2, height/2);
  cairo_line_to(cr, width/2, 0);
  cairo_line_to(cr, width, height/2);
  cairo_set_line_width(cr, 1);

  gtk_style_context_get_color(context,
                              gtk_style_context_get_state(context),
                              &color);
  gdk_cairo_set_source_rgba(cr, &color);

  cairo_stroke(cr);

  return FALSE;
}

static void create_new_wave_window(GtkApplication* app, char* filename) {
  GtkWidget *window_new;
  GtkWidget *vbox;

  GtkWidget *instruction_label;
  GtkWidget *drawing_area;

  window_new = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW (window_new), filename);
  gtk_window_set_default_size(GTK_WINDOW (window_new), 300, 100);
  gtk_container_set_border_width(GTK_CONTAINER(window_new), 5);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(window_new), vbox);

  wav_file_t wav_file = parse_wav_file(filename);
  printf("Number of data channels: %d \n", wav_file.num_channels);

  instruction_label = gtk_label_new(filename); /* label is just the filename */
  gtk_box_pack_start(GTK_BOX(vbox), instruction_label, TRUE, TRUE, 0);

  drawing_area = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawing_area, 100, 100);
  g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_callback), NULL);

  gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

  gtk_widget_show_all(window_new);
}

static void open_file_dialog(GtkWidget* button, gpointer user_data){
  window_app_t *wa = user_data;
  GtkWindow *window = GTK_WINDOW(wa->window);
  GtkApplication *app = wa->app;
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;
  dialog = gtk_file_chooser_dialog_new("Open File",
                                      window,
                                      action,
                                      "_Cancel_",
                                      GTK_RESPONSE_CANCEL,
                                      "_Open_",
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

  res = gtk_dialog_run(GTK_DIALOG(dialog));
  char* filename;
  if (res == GTK_RESPONSE_ACCEPT) {
      GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
      filename = gtk_file_chooser_get_filename(chooser);
      create_new_wave_window(app, filename);
  }

  gtk_widget_destroy(dialog);
}


static void activate (GtkApplication* app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *vbox;

  GtkWidget *button;
  GtkWidget *instruction_label;
  GtkWidget *button_box;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW (window), "WAVYsee");
  gtk_window_set_default_size(GTK_WINDOW (window), 200, 200);
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  window_app_t *wa = g_slice_new(window_app_t); /*creates an efficient memory slice*/
  wa->window = window;
  wa->app = app;

  instruction_label = gtk_label_new("Directions: \nChose a valid .wav file and a new window showing the wave function of the .wav file will be shown.\nAny number of .wav files may be opened simultaneously.");
  gtk_box_pack_start(GTK_BOX(vbox), instruction_label, TRUE, TRUE, 0);

  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add(GTK_CONTAINER(vbox), button_box);

  button = gtk_button_new_with_label("Choose file");
  g_signal_connect(button, "clicked", G_CALLBACK(open_file_dialog), wa);
  gtk_container_add(GTK_CONTAINER(button_box), button);

  gtk_widget_show_all(window);
}

int main(int argc, char **argv){
  GtkApplication *app;

  app = gtk_application_new("org.sina.wavysee", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  int status = g_application_run(G_APPLICATION(app), argc, argv);// Wait for destroy of all windows
  printf("%d\n", status);

  g_object_unref(app);
}
