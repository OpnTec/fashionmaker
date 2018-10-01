/************************************************************************
 **
 **  @file   vposter.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 4, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include "vposter.h"

#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QPixmap>
#include <QPrinter>
#include <QRectF>
#include <QString>
#include <QVector>
#include <Qt>
#include <QDebug>

#include "../vmisc/vmath.h"
#include "../vmisc/def.h"

//---------------------------------------------------------------------------------------------------------------------
VPoster::VPoster(const QPrinter *printer)
    :printer(printer), allowance(static_cast<quint32>(qRound(10./25.4*PrintDPI)))//1 cm
{
}

//---------------------------------------------------------------------------------------------------------------------
QVector<PosterData> VPoster::Calc(const QRect &imageRect, int page, PageOrientation orientation) const
{
    QVector<PosterData> poster;

    if (printer == nullptr)
    {
        return poster;
    }

    const int rows = CountRows(imageRect.height(), orientation);
    const int columns = CountColumns(imageRect.width(), orientation);

    for (int i=0; i < rows; i++)
    {
        for (int j=0; j< columns; j++)
        {
            PosterData data = Cut(i, j, imageRect, orientation);
            data.index = static_cast<quint32>(page);
            data.rows = static_cast<quint32>(rows);
            data.columns = static_cast<quint32>(columns);
            poster.append(data);
        }
    }

    return poster;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QGraphicsItem *> VPoster::Borders(QGraphicsItem *parent, const PosterData &img, int sheets) const
{
    QVector<QGraphicsItem *> data;
    QPen pen(Qt::NoBrush, 1, Qt::DashLine);
    pen.setColor(Qt::black);

    if (img.columns == 1 && img.rows == 1)
    {
        return data;
    }

    const QRect rec = img.rect;
    if (img.column != 0)
    {// Left border
        auto *line = new QGraphicsLineItem(parent);
        line->setPen(pen);
        line->setLine(rec.x(), rec.y(), rec.x(), rec.y() + rec.height());
        data.append(line);

        auto *scissors = new QGraphicsPixmapItem(QPixmap("://scissors_vertical.png"), parent);
        scissors->setPos(rec.x(), rec.y() + rec.height()-static_cast<int>(allowance));
        data.append(scissors);
    }

    if (img.column != img.columns-1)
    {// Right border
        auto *line = new QGraphicsLineItem(parent);
        line->setPen(pen);
        line->setLine(rec.x() + rec.width()-static_cast<int>(allowance), rec.y(),
                      rec.x() + rec.width()-static_cast<int>(allowance), rec.y() + rec.height());
        data.append(line);
    }

    if (img.row != 0)
    {// Top border
        auto *line = new QGraphicsLineItem(parent);
        line->setPen(pen);
        line->setLine(rec.x(), rec.y(), rec.x() + rec.width(), rec.y());
        data.append(line);

        auto *scissors = new QGraphicsPixmapItem(QPixmap("://scissors_horizontal.png"), parent);
        scissors->setPos(rec.x() + rec.width()-static_cast<int>(allowance), rec.y());
        data.append(scissors);
    }

    if (img.rows*img.columns > 1)
    { // Don't show bottom border if only one page need
        // Bottom border (mandatory)
        auto *line = new QGraphicsLineItem(parent);
        line->setPen(pen);
        line->setLine(rec.x(), rec.y() + rec.height()-static_cast<int>(allowance),
                      rec.x() + rec.width(), rec.y() + rec.height()-static_cast<int>(allowance));
        data.append(line);

        if (img.row == img.rows-1)
        {
            auto *scissors = new QGraphicsPixmapItem(QPixmap("://scissors_horizontal.png"), parent);
            scissors->setPos(rec.x() + rec.width()-static_cast<int>(allowance),
                             rec.y() + rec.height()-static_cast<int>(allowance));
            data.append(scissors);
        }
    }

    // Labels
    auto *labels = new QGraphicsTextItem(parent);

    const int layoutX = 15;
    const int layoutY = 5;
    labels->setPos(rec.x() + layoutX, rec.y() + rec.height()-static_cast<int>(allowance)+layoutY);
    labels->setTextWidth(rec.width()-(static_cast<int>(allowance)+layoutX));

    const QString grid = tr("Grid ( %1 , %2 )").arg(img.row+1).arg(img.column+1);
    const QString page = tr("Page %1 of %2").arg(img.row*(img.columns)+img.column+1).arg(img.rows*img.columns);

    QString sheet;
    if (sheets > 1)
    {
        sheet = tr("Sheet %1 of %2").arg(img.index+1).arg(sheets);
    }

    labels->setHtml(QString("<table width='100%'>"
                            "<tr>"
                            "<td>%1</td><td align='center'>%2</td><td align='right'>%3</td>"
                            "</tr>"
                            "</table>")
                    .arg(grid, page, sheet));

    data.append(labels);

    return data;
}

//---------------------------------------------------------------------------------------------------------------------
int VPoster::CountRows(int height, PageOrientation orientation) const
{
    const qreal imgLength = height;
    qreal pageLength = 0;

    if(orientation == PageOrientation::Landscape)
    {
        pageLength = PageRect().width();
    }
    else
    {
        pageLength = PageRect().height();
    }

    if (pageLength >= imgLength)
    {
        return 1;
    }
    else
    {
        return qCeil(imgLength/(pageLength - static_cast<int>(allowance)));
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VPoster::CountColumns(int width, PageOrientation orientation) const
{
    const qreal imgLength = width;
    qreal pageLength = 0;

    if(orientation == PageOrientation::Landscape)
    {
        pageLength = PageRect().height();
    }
    else
    {
        pageLength = PageRect().width();
    }

    if (pageLength >= imgLength)
    {
        return 1;
    }
    else
    {
        return qCeil(imgLength/(pageLength-static_cast<int>(allowance)));
    }
}

//---------------------------------------------------------------------------------------------------------------------
PosterData VPoster::Cut(int i, int j, const QRect &imageRect, PageOrientation orientation) const
{
    Q_UNUSED(imageRect)

    int pageLengthX, pageLengthY;

    if(orientation == PageOrientation::Landscape)
    {
        pageLengthX = PageRect().height();
        pageLengthY = PageRect().width();
    }
    else
    {
        pageLengthX = PageRect().width();
        pageLengthY = PageRect().height();
    }

    const int x = j*pageLengthX - j*static_cast<int>(allowance);
    const int y = i*pageLengthY - i*static_cast<int>(allowance);

    SCASSERT(x <= imageRect.width())
    SCASSERT(y <= imageRect.height())

    PosterData data;
    data.row = static_cast<quint32>(i);
    data.column = static_cast<quint32>(j);
    data.rect = QRect(x, y, pageLengthX, pageLengthY);

    return data;
}

//---------------------------------------------------------------------------------------------------------------------
QRect VPoster::PageRect() const
{
    // Because the Point unit is defined to be 1/72th of an inch
    // we can't use method pageRect(QPrinter::Point). Our dpi value can be different.
    // We convert value yourself to pixels.
    const QRectF rect = printer->pageRect(QPrinter::Millimeter);

    if(printer->fullPage())
    {
        #if QT_VERSION >= QT_VERSION_CHECK(5, 3, 0)
            QMarginsF pMargins = printer->pageLayout().margins();
            QRectF newRect = rect.marginsRemoved(pMargins);
            const QRect pageRectFP(0, 0, qFloor(ToPixel(newRect.width())), qFloor(ToPixel(newRect.height())));
            return pageRectFP;
        #else
            qreal left = 0 , top = 0, right = 0, bottom = 0;
            printer->getPageMargins(&left, &top, &right, &bottom, QPrinter::Millimeter);
            qreal newWidth = rect.width()-left-right;
            qreal newHeight = rect.height()-top-bottom;
            const QRect pageRectFP(0, 0, qFloor(ToPixel(newWidth)), qFloor(ToPixel(newHeight)));
            return pageRectFP;
        #endif
    }
    else
    {
        const QRect pageRect(0, 0, qFloor(ToPixel(rect.width())), qFloor(ToPixel(rect.height())));
        return pageRect;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPoster::ToPixel(qreal val)
{
    return val / 25.4 * PrintDPI; // Mm to pixels with current dpi.
}
