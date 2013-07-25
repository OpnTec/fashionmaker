#ifndef OPTIONS_H
#define OPTIONS_H

#define PrintDPI 96
#define PaperSize 50000
#define WidthMainLine 0.8*PrintDPI/25.4
#define widthHairLine WidthMainLine/3

namespace Scene{
    enum Type
    {
        Point,
        Line,
        Spline,
        Arc
    };
}

#endif // OPTIONS_H
