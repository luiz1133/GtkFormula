#include <gtk/gtk.h>

#include "../widget/gtkformula.h"

typedef struct
{
    GtkDrawingArea* drawing_area;
    GtkFormula* formula;
} FormulaPair;


static void file_set_callback( GtkFileChooserButton* widget, gpointer user_data )
{
    gchar* MML_filepath = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER( widget ) );

    FormulaPair* fp = user_data;

    gtk_formula_clear( fp->formula );

    fp->formula = gtk_formula_set( fp->drawing_area, MML_filepath );

    g_free( MML_filepath );
}

int main( int argc, char** argv )
{
    GtkBuilder* builder;
    GtkWindow* window;
    GtkFileChooserButton *file_chooser_button, *file_chooser_button1, *file_chooser_button2, *file_chooser_button3;
    FormulaPair fp[ 4 ] = { { NULL, NULL } };

    gtk_init( &argc, &argv );

    /* Construct a GtkBuilder instance and load our UI description */
    builder = gtk_builder_new();
    gtk_builder_add_from_file( builder, "hellogtkformula.ui", NULL );

    /* Connect signal handlers to the constructed widgets. */
    window = GTK_WINDOW( gtk_builder_get_object( builder, "window" ) );
    g_signal_connect( window, "destroy", G_CALLBACK( gtk_main_quit ), NULL );

    fp[ 0 ].drawing_area = GTK_DRAWING_AREA( gtk_builder_get_object( builder, "drawing_area" ) );
    fp[ 1 ].drawing_area = GTK_DRAWING_AREA( gtk_builder_get_object( builder, "drawing_area1" ) );
    fp[ 2 ].drawing_area = GTK_DRAWING_AREA( gtk_builder_get_object( builder, "drawing_area2" ) );
    fp[ 3 ].drawing_area = GTK_DRAWING_AREA( gtk_builder_get_object( builder, "drawing_area3" ) );

    GtkFileFilter* filter = gtk_file_filter_new();
    gtk_file_filter_set_name( filter, "MathML files" );
    gtk_file_filter_add_pattern( filter, "*.mml" );

    file_chooser_button = GTK_FILE_CHOOSER_BUTTON( gtk_builder_get_object( builder, "file_chooser_button" ) );
    gtk_file_chooser_set_filter( GTK_FILE_CHOOSER( file_chooser_button ), filter );
    g_signal_connect( file_chooser_button, "file-set", G_CALLBACK( file_set_callback ), &fp[ 0 ] );

    file_chooser_button1 = GTK_FILE_CHOOSER_BUTTON( gtk_builder_get_object( builder, "file_chooser_button1" ) );
    gtk_file_chooser_set_filter( GTK_FILE_CHOOSER( file_chooser_button1 ), filter );
    g_signal_connect( file_chooser_button1, "file-set", G_CALLBACK( file_set_callback ), &fp[ 1 ] );

    file_chooser_button2 = GTK_FILE_CHOOSER_BUTTON( gtk_builder_get_object( builder, "file_chooser_button2" ) );
    gtk_file_chooser_set_filter( GTK_FILE_CHOOSER( file_chooser_button2 ), filter );
    g_signal_connect( file_chooser_button2, "file-set", G_CALLBACK( file_set_callback ), &fp[ 2 ] );

    file_chooser_button3 = GTK_FILE_CHOOSER_BUTTON( gtk_builder_get_object( builder, "file_chooser_button3" ) );
    gtk_file_chooser_set_filter( GTK_FILE_CHOOSER( file_chooser_button3 ), filter );
    g_signal_connect( file_chooser_button3, "file-set", G_CALLBACK( file_set_callback ), &fp[ 3 ] );

    gtk_main();

    gtk_formula_clear( fp[ 0 ].formula );
    gtk_formula_clear( fp[ 1 ].formula );
    gtk_formula_clear( fp[ 2 ].formula );
    gtk_formula_clear( fp[ 3 ].formula );
    gtk_formula_free();

    return 0;
}

