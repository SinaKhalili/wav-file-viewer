#include <stdio.h>
#include <gtk/gtk.h>

static void open_file_dialog(GtkWidget* button, gpointer window){
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;
  dialog = gtk_file_chooser_dialog_new("Open File",
                                      window,
                                      action,
                                      "_Beans_",
                                      GTK_RESPONSE_CANCEL,
                                       "_Open_",
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

  res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT) {
      char *filename;
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
      filename = gtk_file_chooser_get_filename (chooser);
      printf("%s\n", filename);
  }
  gtk_widget_destroy(dialog);
}

static void activate (GtkApplication* app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *button_box;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW (window), "Window");
  gtk_window_set_default_size(GTK_WINDOW (window), 200, 200);

  button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add(GTK_CONTAINER(window), button_box);

  button = gtk_button_new_with_label("l337 gui programming");
  g_signal_connect(button, "clicked", G_CALLBACK(open_file_dialog), window);
  gtk_container_add(GTK_CONTAINER(button_box), button);

  gtk_widget_show_all(window);
}

int main(int argc, char **argv){
  GtkApplication *app;

  app = gtk_application_new("org.sina.wavysee", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL); // Wait for destroy
  int status = g_application_run(G_APPLICATION(app), argc, argv);
  printf("%d\n", status);

  g_object_unref(app);
}
