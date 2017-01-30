# GtkFormula

GTK+ widget to display MathML formulas.

## Usage

Sample code is provided. First, install the dependencies. Then, adjust the hard-coded paths to your needs. Finally, compile with ```gcc `pkg-config --cflags librsvg-2.0 gtk+-2.0` -I/usr/lib/Saxonica /usr/lib/Saxonica/Saxon.C.API/SaxonCGlue.c /usr/lib/Saxonica/Saxon.C.API/SaxonCProcessor.c ../gtkformula.c hellogtkformula.c `pkg-config --libs librsvg-2.0 gtk+-2.0` -ldl -o hellogtkformula```, adjusting the paths to your needs once again.

## Dependencies
  - Saxon XSLT processor: [saxonica.com/saxon-c/index.xml][link-saxon]
  - pMML2SVG: [pmml2svg.sourceforge.net][link-pmml2svg]
  - librsvg: [wiki.gnome.org/Projects/LibRsvg][link-rsvg]
  - STIX fonts

## Screenshots

![Screenshot 1](http://i.imgur.com/vmtAdzU.png "Screenshot 1")
![Screenshot 2](http://i.imgur.com/0FAZlIU.png "Screenshot 2"")

[link-saxon]: http://www.saxonica.com/saxon-c/index.xml
[link-pmml2svg]: http://pmml2svg.sourceforge.net/
[link-rsvg]: https://wiki.gnome.org/Projects/LibRsvg/
