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
GtkWidget	*label_search_result;

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
	label_search_result = GTK_WIDGET(gtk_builder_get_object(builder,"label_search_result"));

	gtk_window_set_title (GTK_WINDOW (window), "HASH zhangrui");

	gtk_widget_show(window);

	gtk_main();

	return EXIT_SUCCESS;
}

void showHashFile()
{

    int fd;
    g_print("------------\n");
    fd=open(FILENAME,O_RDWR);
    lseek(fd,sizeof(struct HashFileHeader),SEEK_SET);
    struct jtRecord jt;
    struct CFTag tag;
    while(1)
    {
        if (read(fd,&tag,sizeof(struct CFTag))<=0)
            break;
        if(read(fd,&jt,sizeof(struct jtRecord))<=0)
            break;
        if (tag.free) {
            g_print("标签是<%d,%d>\t",tag.collision,tag.free);
            g_print("记录是{%d,%s}\n",jt.key,jt.other);
        }
    }
		g_print("------------\n");
    close(fd);
}

void refresh_label_pre()
{
	char *hash_buffer=malloc(sizeof(char)*500);
	memset(hash_buffer,0,sizeof(char)*500);

  int fd;
  fd=open(FILENAME,O_RDWR);
  lseek(fd,sizeof(struct HashFileHeader),SEEK_SET);
  struct jtRecord jt;
  struct CFTag tag;
  while(1)
  {
      if (read(fd,&tag,sizeof(struct CFTag))<=0)
          break;
      if(read(fd,&jt,sizeof(struct jtRecord))<=0)
          break;
      if (tag.free)
			{
				char label[30],record[30];
				sprintf(label,"标签是<%d,%d>\t",tag.collision,tag.free);
				sprintf(record,"记录是{%d,%s}\n",jt.key,jt.other);
				strcat(hash_buffer,label);
				strcat(hash_buffer,record);

        // g_print("标签是<%d,%d>\t",tag.collision,tag.free);
        // g_print("记录是{%d,%s}\n",jt.key,jt.other);
      }
  }
	// strcat(hash_buffer,'\0');
	// g_print("%s",hash_buffer);

	gtk_label_set_text (GTK_LABEL(hash_data_pre), (const gchar* ) hash_buffer);

  close(fd);
	return;
}


void refresh_label_now()
{
	char *hash_buffer=malloc(sizeof(char)*500);
	memset(hash_buffer,0,sizeof(char)*500);

	int fd;
	fd=open(FILENAME,O_RDWR);
	lseek(fd,sizeof(struct HashFileHeader),SEEK_SET);
	struct jtRecord jt;
	struct CFTag tag;
	while(1)
	{
			if (read(fd,&tag,sizeof(struct CFTag))<=0)
					break;
			if(read(fd,&jt,sizeof(struct jtRecord))<=0)
					break;
			if (tag.free)
			{
				char label[30],record[30];
				sprintf(label,"标签是<%d,%d>\t",tag.collision,tag.free);
				sprintf(record,"记录是{%d,%s}\n",jt.key,jt.other);
				strcat(hash_buffer,label);
				strcat(hash_buffer,record);

				// g_print("标签是<%d,%d>\t",tag.collision,tag.free);
				// g_print("记录是{%d,%s}\n",jt.key,jt.other);
			}
	}
	// strcat(hash_buffer,'\0');
	// g_print("%s",hash_buffer);

	gtk_label_set_text (GTK_LABEL(hash_data_now), (const gchar* ) hash_buffer);

	close(fd);
	return;
}


void on_but_add_clicked (GtkButton *b,gpointer pointer)   //save record uses 2 params ,key and other
{
	refresh_label_pre();

	int fd=hashfile_open(FILENAME,O_RDWR,0);
	struct jtRecord record;

  const gchar *key_buf;
	const gchar *value_buf;
	key_buf=gtk_entry_get_text(GTK_ENTRY(entry_key));
	value_buf=gtk_entry_get_text(GTK_ENTRY(entry_value));
	record.key=atoi(key_buf);
	sprintf(record.other,"%s",value_buf);
	// g_print("%s",record.other);
	// g_print("%s",key_buf);

	hashfile_saverec(fd,KEYOFFSET,KEYLEN,&record);
	hashfile_close(fd);

	refresh_label_now();
	showHashFile();
	return;
}

void  on_but_del_clicked (GtkButton *b)
{
	refresh_label_pre();

	int fd=hashfile_open(FILENAME,O_RDWR,0);
	struct HashFileHeader hfh;
  readHashFileHeader(fd,&hfh);
	if(hfh.current_rec_num==0)
	{
		g_print("now have no record\n");
		return;
	}

	struct jtRecord record;

	const gchar *key_buf;
	const gchar *value_buf;
	key_buf=gtk_entry_get_text(GTK_ENTRY(entry_key));
	value_buf=gtk_entry_get_text(GTK_ENTRY(entry_value));
	record.key=atoi(key_buf);
	sprintf(record.other,"%s",value_buf);


	hashfile_delrec(fd,KEYOFFSET,KEYLEN,&record);
	hashfile_close(fd);

	refresh_label_now();
	showHashFile();
	return;
}

void	on_but_search_clicked (GtkButton *b)   //search through the content of "other" of records
{
	refresh_label_pre();

	char *hash_buffer=malloc(sizeof(char)*500);
	memset(hash_buffer,0,sizeof(char)*500);
	char label_buffer[30],record_buffer[30],offset_buffer[30];


	int fd=hashfile_open(FILENAME,O_RDWR,0);

	struct HashFileHeader hfh;
	readHashFileHeader(fd,&hfh);
	if(hfh.current_rec_num==0)
	{
		g_print("no record to search \n");
		return;
	}


	struct jtRecord record;
  const gchar *key_buf;
	const gchar *value_buf;
	key_buf=gtk_entry_get_text(GTK_ENTRY(entry_key));
	value_buf=gtk_entry_get_text(GTK_ENTRY(entry_value));
	record.key=atoi(key_buf);
	sprintf(record.other,"%s",value_buf);

	int offset=-1;
	offset=hashfile_findrec(fd,KEYOFFSET,KEYLEN,&record);
	if(offset!=-1)
	{
		sprintf(offset_buffer,"偏移是 %d\n",offset);
		// g_print("偏移是 %d\n",offset);
		hashfile_close(fd);
		struct jtRecord jt;
		struct CFTag tag;
		fd=open(FILENAME,O_RDWR);
		lseek(fd,offset,SEEK_SET);
		read(fd,&tag,sizeof(struct CFTag));
		sprintf(label_buffer,"标签是<%d,%d>\t",tag.collision,tag.free);
		// g_print("标签是 <%d,%d>\t",tag.collision,tag.free);
		read(fd,&jt,sizeof(struct jtRecord));
		sprintf(record_buffer,"记录是{%d,%s}\n",jt.key,jt.other);
		// g_print("记录是 {%d,%s}\n",jt.key,jt.other);
		close(fd);
		strcat(hash_buffer,offset_buffer);
		strcat(hash_buffer,label_buffer);
		strcat(hash_buffer,record_buffer);
	}
	else
	{
		strcat(hash_buffer,"search not found");
	}

	gtk_label_set_text (GTK_LABEL(label_search_result), (const gchar* ) hash_buffer);

	refresh_label_now();
	showHashFile();
	return;
}






/*
gcc   jtRecord.c  -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic -rdynamic
gcc -Wno-format -o hash jtRecord.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
 */
