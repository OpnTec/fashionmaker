#ifndef OPTIONS_H
#define OPTIONS_H

#define PrintDPI 96
#define PaperSize 50000
#define widthMainLine 0.8*PrintDPI/25.4
#define widthHairLine widthMainLine/3

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
