#include <stdlib.h>
#include <gtk/gtk.h>

void on_window_destroy(GtkObject *object, gpointer user_data) {
	exit(EXIT_SUCCESS);
	gtk_main_quit();
}

G_MODULE_EXPORT
void ElbowMainToolOpen(GtkWidget * object, gpointer user_data) {
	g_printf("Hello there.\n");
	return;
}

int main(int argc, char *argv[]) {
	GtkBuilder              *builder;
	GtkWidget               *window;

	gtk_init (&argc, &argv);

	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "ElbowMain.glade", NULL);

	window = GTK_WIDGET (gtk_builder_get_object (builder, "ElbowMain"));
	gtk_builder_connect_signals (builder, NULL);
	g_object_unref (G_OBJECT (builder));

	gtk_widget_show (window);
	gtk_main ();

	return 0;
}
