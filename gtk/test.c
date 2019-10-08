#include<gtk/gtk.h>
int main(int argc, char *argv[])
{
  GtkWidget *windows;
  gtk_init(&argc,&argv);
  windows=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_show(windows);
  gtk_main();
  return 0;
}


// yum install gtk3 gtk3-devel gtk3-devel-docs
// yum install gnome-devel gnome-devel-docs
// sudo apt-get install libgtk2.0*     用的是3 apt不清楚
// https://blog.csdn.net/pfysw/article/details/81190404 基础
// https://blog.csdn.net/Rong_Toa/article/details/85856499 入门
// https://blog.csdn.net/libinbin_1014/article/details/44940197 安装
