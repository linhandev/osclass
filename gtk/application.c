#include <gtk/gtk.h>

static void activate(GtkApplication* app,gpointer user_data)
{
  GtkWidget *window;

  window=gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window),"my window");
  gtk_window_set_default_size(GTK_WINDOW(window),200,200);
  gtk_widget_show_all(window);

}

int main(int argc,char ** argv)
{
  GtkApplication *app;
  int status;

  app=gtk_application_new("org.gtk.example",G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app,"activate",G_CALLBACK(activate),NULL);
  status=g_application_run(G_APPLICATION(app),argc,argv);
  g_object_unref(app);

  return status;
}




// yum install gtk3 gtk3-devel gtk3-devel-docs
// yum install gnome-devel gnome-devel-docs
// sudo apt-get install libgtk2.0*     用的是3 apt不清楚
// https://blog.csdn.net/pfysw/article/details/81190404 基础
// https://blog.csdn.net/Rong_Toa/article/details/85856499 入门
// https://blog.csdn.net/libinbin_1014/article/details/44940197 安装
