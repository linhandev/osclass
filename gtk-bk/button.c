#include <stdio.h>
#include <gtk/gtk.h>

/*
 * callback event function for the gtk delete event. invoked
 * when the application is closed using the window manager.
 */
int
delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    /* If you return FALSE in the "delete_event" signal handler,
     * GTK will emit the "destroy" signal. Returning TRUE means
     * you don't want the window to be destroyed.
     * This is useful for popping up 'are you sure you want to quit?'
     * type dialogs.
     */

    return(FALSE);
}


/* exit the application. */
void
exit_cb(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

/* "print text" button "clicked" callback function. */
void
on_button1_clicked(GtkButton* button, gpointer data)
{
    /* cast the data back to a GtkEntry*  */
    GtkEntry* entry = (GtkEntry*)data;

    printf("on_button_clicked - entry = '%s'\n", gtk_entry_get_text(entry));
    fflush(stdout);
}

/* "toggle editability" button "clicked" callback function. */
void
on_button2_clicked(GtkButton* button, gpointer data)
{
    static gboolean is_editable = TRUE;
    /* cast the data back to a GtkEntry*  */
    GtkEntry* entry = (GtkEntry*)data;

    if (is_editable) {
	gtk_entry_set_editable(entry, FALSE);
        is_editable = FALSE;
    }
    else {
	gtk_entry_set_editable(entry, TRUE);
        is_editable = TRUE;
    }
}

int
main(int argc, char* argv[])
{
    /* this variable will store a pointer to the window object. */
    GtkWidget* main_window;
    /* this will store a horizontal box. */
    GtkWidget* hbox;
    /* these will store push buttons. */
    GtkWidget* button1;
    GtkWidget* button2;
    /* this will store a text entry. */
    GtkWidget* entry;


    /* This is called in all GTK applications. Arguments */
    /* are parsed from the command line and are returned */
    /* to the application.                               */
    gtk_init(&argc, &argv);

    /* create a new top-level window. */
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    /* define some of the window's attributes. */
    gtk_window_set_title(GTK_WINDOW (main_window),
                         "A Top Window For The Rest Of Us...");
    gtk_container_set_border_width(GTK_CONTAINER (main_window), 5);

    /* make the window visible. */
    gtk_widget_show(main_window);

    /* When the window is given the "delete_event" signal (this is given
     * by the window manager, usually by the "close" option, or on the
     * titlebar), we ask it to call the delete_event() function
     * as defined above. The data passed to the callback
     * function is NULL and is ignored in the callback function.
     */
    gtk_signal_connect(GTK_OBJECT (main_window), "delete_event",
                       GTK_SIGNAL_FUNC (delete_event), NULL);

    /* Here we connect the "destroy" event to a signal handler.
     * This event occurs when we call gtk_widget_destroy() on the window,
     * or if we return FALSE in the "delete_event" callback.
     */
    gtk_signal_connect(GTK_OBJECT (main_window), "destroy",
                       GTK_SIGNAL_FUNC (exit_cb), NULL);

    /* create a new horizontal box, and add to top-level window. */
    hbox = gtk_hbox_new(TRUE, 20);
    gtk_container_add(GTK_CONTAINER(main_window), hbox);
    gtk_widget_show(hbox);

    /* create a text entry. */
    entry = gtk_entry_new_with_max_length(20);

    /* insert the following text into the text entry, as its initial value. */
    gtk_entry_set_text(GTK_ENTRY(entry), "hello there");

    /* make the text entry visible. */
    gtk_widget_show(entry);

    /* add the text entry to the hbox. */
    gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, TRUE, 1);

    /* create a button, place in hbox, attach callback.   */
    /* note: button is created second, cause it needs the */
    /* entry's pointer as a parameter to its callback.    */
    button1 = gtk_button_new_with_label("Print Text");
    gtk_widget_show(button1);
    gtk_box_pack_start(GTK_BOX(hbox), button1, FALSE, TRUE, 1);
    gtk_signal_connect(GTK_OBJECT(button1), "clicked",
                       GTK_SIGNAL_FUNC(on_button1_clicked),
                       (gpointer)entry);

    /* create a 2nd button, place in hbox, attach callback. */
    button2 = gtk_button_new_with_label("Toggle Editablibity");
    gtk_widget_show(button2);
    gtk_box_pack_start(GTK_BOX(hbox), button2, FALSE, TRUE, 1);
    gtk_signal_connect(GTK_OBJECT(button2), "clicked",
                       GTK_SIGNAL_FUNC(on_button2_clicked),
                       (gpointer)entry);


    /* All GTK applications must have a gtk_main(). Control
     * ends here and waits for an event to occur (like a
     * key press or mouse event).
     */
    gtk_main();

    exit(0);
}
