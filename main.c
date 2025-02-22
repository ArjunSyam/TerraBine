#include <gtk/gtk.h>
//creating a window

static void activate(GtkApplication* app, gpointer user_data) {
	GtkWidget* window;
	GtkWidget* label;

	window = gtk_application_window_new (app);
	label = gtk_label_new("Hello World,Welcome to TerraBine !! ");
	gtk_container_add(GTK_CONTAINER (window), label);
	gtk_window_set_title(GTK_WINDOW (window), "Terrabine");
	gtk_window_set_default_size(GTK_WINDOW (window), 400, 200);
	gtk_widget_show_all(window);
}

//main function

int main(int argc, char** argv) {
	GtkApplication* app;
	int status;

	app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);

	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}
