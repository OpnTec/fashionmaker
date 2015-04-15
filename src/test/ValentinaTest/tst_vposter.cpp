/************************************************************************
 **
 **  @file   tst_vposter.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 4, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "tst_vposter.h"
#include "../../libs/vlayout/vposter.h"

#include <QImage>
#include <QPrinter>
#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VPoster::TST_VPoster(QObject *parent) :
    QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VPoster::BigPoster()
{
    QPrinter printer;
    printer.setResolution(96);// By default
    printer.setPaperSize(QPrinter::A4);

    const QImage image(2622, 3178, QImage::Format_RGB32); // Little bit bigger than A1
    VPoster posterazor(&printer);
    const QVector<QImage> poster = posterazor.Generate(image, 1, 1);

    QCOMPARE(poster.size(), 16);

    for (int i=0; i < poster.size(); i++)
    {
        QCOMPARE(poster.at(i).rect().size(), PageRect(printer).size());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_VPoster::SmallPoster()
{
    QPrinter printer;
    printer.setResolution(96);// By default
    printer.setPaperSize(QPrinter::A4);

    const QImage image(700, 1000, QImage::Format_RGB32); // Little bit less than A4
    VPoster posterazor(&printer);
    const QVector<QImage> poster = posterazor.Generate(image, 1, 1);

    QCOMPARE(poster.size(), 1);

    QCOMPARE(poster.at(0).rect().size(), PageRect(printer).size());
}

//---------------------------------------------------------------------------------------------------------------------
QRect TST_VPoster::PageRect(const QPrinter &printer) const
{
    // Because the Point unit is defined to be 1/72th of an inch
    // we can't use method pageRect(QPrinter::Point). Our dpi different can be different.
    // We convert value yourself to pixels.
    const QRectF rect = printer.pageRect(QPrinter::Millimeter);
    QRect pageRect(qFloor(ToPixel(rect.x(), printer)), qFloor(ToPixel(rect.y(), printer)),
                   qFloor(ToPixel(rect.width(), printer)), qFloor(ToPixel(rect.height(), printer)));
    return pageRect;
}

//---------------------------------------------------------------------------------------------------------------------
qreal TST_VPoster::ToPixel(qreal val, const QPrinter &printer) const
{
    return val / 25.4 * printer.resolution(); // Mm to pixels with current dpi.
}
