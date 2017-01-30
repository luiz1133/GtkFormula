#include "gtkformula.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Saxon.C.API/SaxonCGlue.h"
#include "Saxon.C.API/SaxonCProcessor.h"

#include <cairo.h>
#include <librsvg/rsvg.h>

struct GtkFormula
{
    RsvgHandle* handle;
    GtkDrawingArea* drawing_area;
};

static sxnc_environment* environ;

enum state { UNINITIALIZED, INITIALIZED, FINISHED };
static enum state current_state = UNINITIALIZED;

/**
 * Acquires required resources and initialize global variables
 */
static void initialize()
{
    environ = malloc( sizeof( sxnc_environment ) );

    /**
     * First of all, load required component.
     * By the time of JET initialization, all components should be loaded.
     */
    environ->myDllHandle = loadDefaultDll();

    /**
     * Initialize JET run-time.
     * The handle of loaded component is used to retrieve Invocation API.
     */
    initDefaultJavaRT( &environ );

    current_state = INITIALIZED;
}

/**
 * Converts a MathML file into an SVG file using the XSLT stylesheet specified in the function.
 */
static const char* mml2svg( const char* mathML_file )
{
    if( current_state == INITIALIZED )
    {
        const char* stylesheet = "/local/luizlima/mathml/pMML2SVG-0.8.5/XSLT2/pmml2svg.xsl";
        return xsltApplyStylesheet( *environ, NULL, NULL, mathML_file, stylesheet, NULL, NULL, 0, 0 );
    }
}

/**
 * Effectively draws the formula on the widget
 */
static gboolean expose_event_callback( GtkWidget* widget, GdkEventExpose* event, gpointer user_data )
{
    RsvgHandle* handle = user_data;

    cairo_t* cr = gdk_cairo_create( gtk_widget_get_window( widget ) );

    /* Fit the formula into the widget */
    RsvgDimensionData dimension_data;
    rsvg_handle_get_dimensions( handle, &dimension_data );
    int svg_width = dimension_data.width, svg_height = dimension_data.height;
    double svg_ratio = ( double ) svg_width / ( double ) svg_height;

    GtkAllocation allocation;
    gtk_widget_get_allocation( widget, &allocation );
    int widget_width = allocation.width, widget_height = allocation.height;
    double widget_ratio = ( double ) widget_width / ( double ) widget_height;

    double scale_factor;

    if( svg_ratio > widget_ratio ) 
    {
        scale_factor = ( double ) widget_width / ( double ) svg_width;
    }
    else
    {
        scale_factor = ( double ) widget_height / ( double ) svg_height;
    }

    cairo_scale( cr, scale_factor, scale_factor );

    if( handle == NULL )
    {
        fprintf( stdout, "Nothing to draw\n" );
    }
    else if( !rsvg_handle_render_cairo( handle, cr ) )
    {
        fprintf( stderr, "Failed on rsvg_handle_render_cairo()\n" );
    }

    cairo_destroy( cr );

    return FALSE;
}

GtkFormula* gtk_formula_set( GtkDrawingArea* drawing_area, const char* mathML_file )
{
    if( drawing_area == NULL || mathML_file == NULL || mathML_file[ 0 ] == 0 )
    {
        return NULL;
    }

    GtkFormula* formula = malloc( sizeof( GtkFormula ) );
    if( formula == NULL )
    {
        return NULL;
    }
    formula->drawing_area = drawing_area;

    if( current_state == UNINITIALIZED )
    {
        initialize();
    }

    GError* err = NULL;
    const char* SVG_string = mml2svg( mathML_file );
    formula->handle = rsvg_handle_new_from_data( SVG_string, strlen( SVG_string ), &err );
    if( err != NULL )
    {
        /* Report error to user, and free error */
        fprintf( stderr, "Error: %s\n", err->message );
        g_error_free( err );
        free( formula );
        formula = NULL;
    }
    else
    {
        g_signal_connect( formula->drawing_area, "expose-event", G_CALLBACK( expose_event_callback ), formula->handle );
        gtk_widget_queue_draw( GTK_WIDGET( formula->drawing_area ) );
    }

    return formula;
}

void gtk_formula_clear( GtkFormula* formula )
{
    if( formula == NULL )
    {
        return;
    }

    g_signal_handlers_disconnect_by_func( formula->drawing_area, expose_event_callback, formula->handle );
    g_object_unref( formula->handle );
    gtk_widget_queue_draw( GTK_WIDGET( formula->drawing_area ) );
}

void gtk_formula_free()
{
    if( current_state == INITIALIZED )
    {
        /**
         * Finalize JET run-time.
         */
        finalizeJavaRT( environ->jvm );
        free( environ );
        current_state = FINISHED;
    }
}

