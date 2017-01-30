#ifndef _GTK_FORMULA_H_
#define _GTK_FORMULA_H_

#include <gtk/gtk.h>

/**
 * GtkFormula represents a visible formula on a drawing area.
 */
typedef struct GtkFormula GtkFormula;

/**
 * Draws formula on the drawing area.
 *
 * drawing_area: pointer to the drawing area widget where the formula
 *               will be drawn. this parameter should not be null.
 *
 * mathML_file: path to the file with the MathML description of the formula.
 *
 * returns: pointer to the GtkFormula created.
 *          free with gtk_formula_clear when done.
 */
GtkFormula* gtk_formula_set( GtkDrawingArea* drawing_area, const char* mathML_file );

/**
 * Clears formula from its drawing area and frees GtkFormula.
 *
 * formula: pointer to the drawing area widget to be cleared.
 *          this parameter may be null, in which case the function does nothing.
 */
void gtk_formula_clear( GtkFormula* formula );

/**
 * Releases the required resources. Call this when your program is finishing.
 */
void gtk_formula_free();

#endif

