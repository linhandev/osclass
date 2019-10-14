#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <ctype.h>
//GTK include


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define RECORDLEN 32   //一条记录整个的大小，包括key和内容
#define COLLISIONFACTOR 0.5  //Hash函数冲突率，已经定义过，要改的话在这改

#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "HashFile.h"
#include "jtRecord.h"
#define KEYOFFSET 0   //key在struct里的offset,单位字节
#define KEYLEN sizeof(int)
#define FILENAME "jing.hash"


// Make them global

GtkWidget	*window;
GtkWidget	*entry_key;
GtkWidget	*entry_value;
GtkWidget	*but_add;
GtkWidget	*but_del;
GtkWidget	*but_search;
GtkWidget	*hash_data_pre;
GtkWidget	*hash_data_now;

GtkBuilder	*builder;
GError *error = NULL;



int main(int argc, char *argv[])
{
	int total_record_number=6;


	int fd=hashfile_creat(FILENAME,O_RDWR|O_CREAT,RECORDLEN,total_record_number);
  if(fd!=-1)
    printf("已经成功创建文件\n");
  else
    return 0;


	gtk_init(&argc, &argv); // init Gtk
//---------------------------------------------------------------------
// establish contact with xml code used to adjust widget settings
//---------------------------------------------------------------------

  builder = gtk_builder_new_from_file ("layout.glade");

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_builder_connect_signals(builder, NULL);

  entry_key = GTK_WIDGET(gtk_builder_get_object(builder,"entry_key"));
  entry_value = GTK_WIDGET(gtk_builder_get_object(builder,"entry_value"));
  but_add = GTK_WIDGET(gtk_builder_get_object(builder,"but_add"));
  but_del = GTK_WIDGET(gtk_builder_get_object(builder,"but_del"));
  but_search = GTK_WIDGET(gtk_builder_get_object(builder,"but_search"));
  hash_data_pre = GTK_WIDGET(gtk_builder_get_object(builder,"hash_data_pre"));
  hash_data_now = GTK_WIDGET(gtk_builder_get_object(builder,"hash_data_now"));

	gtk_widget_show(window);

	gtk_main();

	return EXIT_SUCCESS;
}

void on_but_add_clicked (GtkButton *b,gpointer pointer)
{
	int fd=hashfile_open(FILENAME,O_RDWR,0);
	struct jtRecord record;

  const gchar *key_buf;
	const gchar *value_buf;

	key_buf=gtk_entry_get_text(GTK_ENTRY(entry_key));
	value_buf=gtk_entry_get_text(GTK_ENTRY(entry_value));
	// gtk_label_set_text (GTK_LABEL(hash_data_pre), key_buf );

	int key = atoi(key_buf);
	record.key=key;
	record.other=*value_buf;
	hashfile_saverec(fd,KEYOFFSET,KEYLEN,&record);
	hashfile_close(fd);

	return;
}

void  on_but_del_clicked (GtkButton *b)
{
	int fd=hashfile_open(FILENAME,O_RDWR,0);
	struct jtRecord record;

	const gchar *key_buf;
	const gchar *value_buf;

	key_buf=gtk_entry_get_text(GTK_ENTRY(entry_key));
	value_buf=gtk_entry_get_text(GTK_ENTRY(entry_value));
	gtk_label_set_text (GTK_LABEL(hash_data_pre), (const gchar* ) "Hello World");
}

void	on_but_search_clicked (GtkButton *b)
{
	gtk_label_set_text (GTK_LABEL(hash_data_now), (const gchar* ) "Hello World");
}




/*
gcc   jtRecord.c  -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic -rdynamic
gcc -Wno-format -o hash jtRecord.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
 */
