/************************************************************************
 **
 **  @file   vposter.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 4, 2015
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

#include "vposter.h"
#include <QPainter>
#include <QPrinter>
#include <QtMath>

#include "../../utils/def.h"

//---------------------------------------------------------------------------------------------------------------------
VPoster::VPoster(const QPrinter *printer)
    :printer(printer), allowence(38)//1 cm
{
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QImage> VPoster::Generate(const QImage &image, int page, int sheets) const
{
    QVector<QImage> poster;

    if (printer == nullptr)
    {
        return poster;
    }

    const int rows = CountRows(image.rect().height());
    const int colomns = CountColomns(image.rect().width());

    for (int i=0; i < rows; i++)
    {
        for (int j=0; j< colomns; j++)
        {
            QImage img = Cut(i, j, image);
            img = Borders(rows, colomns, i, j, img, page, sheets);
            poster.append(img);
        }
    }

    return poster;
}

//---------------------------------------------------------------------------------------------------------------------
int VPoster::CountRows(int height) const
{
    const qreal imgLength = height;
    const qreal pageLength = PageRect().height();

    // Example
    // ― ―
    // * *
    // * *
    // * *
    // * * ―
    // ― ― *
    // *   *
    // *   *
    // *   * ―
    // *   ― *
    // —     *
    // *     *
    // *     * ―
    // *     ― *
    // *       *
    // —       *
    // *       * ―
    // *       ― * <-(2)
    // *       + *
    // *       + *
    // —       + * ― <-(4)
    // ^       ^ ― *
    //(3)     (1)  *
    //             *
    //             *
    //             ―

    const int pCount = qCeil(imgLength/pageLength);// Pages count without allowence (or allowence = 0) (3)

    // Calculate how many pages will be after using allowence.
    // We know start pages count. This number not enought because
    // each n-1 pages add (n-1)*allowence length to page (1).
    const qreal addionalLength = (pCount-1)*allowence;

    // Calculate additional length form pages that will cover this length (2).
    // In the end add page length (3).
    // Bottom page have mandatory border (4)
    return qCeil((addionalLength +
                         qCeil(addionalLength/pageLength)*allowence + allowence + imgLength)/pageLength);
}

//---------------------------------------------------------------------------------------------------------------------
int VPoster::CountColomns(int width) const
{
    const qreal imgLength = width;
    const qreal pageLength = PageRect().width();

    // Example
    // |----|----|----|----| <- (3)
    // |----|+++++++++++++++
    //     |----|+++++++++++
    //         |----|+++++++
    //             |----|+++ <- (1)
    //                 |----|
    //                  ^
    //                 (2)
    const int pCount = qCeil(imgLength/pageLength);// Pages count without allowence (or allowence = 0) (3)

    // Calculate how many pages will be after using allowence.
    // We know start pages count. This number not enought because
    // each n-1 pages add (n-1)*allowence length to page (1).
    const qreal addionalLength = (pCount-1)*allowence;

    // Calculate additional length form pages that will cover this length (2).
    // In the end add page length (3).
    return qCeil((addionalLength + qCeil(addionalLength/pageLength)*allowence + imgLength)/pageLength);
}

//---------------------------------------------------------------------------------------------------------------------
QImage VPoster::Cut(int i, int j, const QImage &image) const
{
    const int x = j*PageRect().width()  - j*allowence;
    const int y = i*PageRect().height() - i*allowence;

    SCASSERT(x <= image.rect().width());
    SCASSERT(y <= image.rect().height());

    QRect copyRect(x, y, PageRect().width(), PageRect().height());

    if (not image.rect().contains(copyRect))
    {
        // Create full page with white background
        QImage fullPage(copyRect.size(), image.format());
        fullPage.fill(Qt::white);

        // Real size that we can copy from image.
        // Because in areas beyond the image, pixels are set to 0 by copy() method.
        // For 32-bit RGB images, this means black.
        copyRect = image.rect().intersected(copyRect);

        QPainter painter(&fullPage);
        painter.drawImage(QPointF(), image.copy( copyRect));
        painter.end();

        return fullPage;
    }
    else
    {
        return image.copy(copyRect);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QImage VPoster::Borders(int rows, int colomns, int i, int j, QImage &image, int page, int sheets) const
{
    QPainter painter(&image);

    QPen pen = QPen(Qt::NoBrush, 1, Qt::DashLine);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    if (j != 0 && PageRect().x() > 0)
    {// Left border
        painter.drawLine(QLine(0, 0, 0, image.rect().height()));
        painter.drawImage(QPoint(0, image.rect().height()-allowence), QImage("://scissors_vertical.png"));
    }

    if (j != colomns-1)
    {// Right border
        painter.drawLine(QLine(image.rect().width()-allowence, 0,
                               image.rect().width()-allowence, image.rect().height()));
    }

    if (i != 0 && PageRect().y() > 0)
    {// Top border
        painter.drawLine(QLine(0, 0, image.rect().width(), 0));
        painter.drawImage(QPoint(image.rect().width()-allowence, 0), QImage("://scissors_horizontal.png"));
    }

    if (rows*colomns > 1)
    { // Don't show bottom border if only one page need
        // Bottom border (mandatory)
        painter.drawLine(QLine(0, image.rect().height()-allowence,
                               image.rect().width(), image.rect().height()-allowence));
        if (i == rows-1)
        {
            painter.drawImage(QPoint(image.rect().width()-allowence, image.rect().height()-allowence),
                              QImage("://scissors_horizontal.png"));
        }
    }

    // Labels
    const int layoutX = 15;
    const int layoutY = 5;
    QRect labels(layoutX, image.rect().height()-allowence+layoutY,
                 image.rect().width()-(allowence+layoutX), allowence-layoutY);
    painter.drawText(labels, Qt::AlignLeft, tr("Grid ( %1 , %2 )").arg(i).arg(j));
    painter.drawText(labels, Qt::AlignHCenter, tr("Page %1 of %2").arg(i*(colomns)+j+1).arg(rows*colomns));
    if (sheets > 1)
    {
        painter.drawText(labels, Qt::AlignRight, tr("Sheet %1 of %2").arg(page).arg(sheets));
    }

    painter.end();
    return image;
}

//---------------------------------------------------------------------------------------------------------------------
QRect VPoster::PageRect() const
{
    // Because the Point unit is defined to be 1/72th of an inch
    // we can't use method pageRect(QPrinter::Point). Our dpi different can be different.
    // We convert value yourself to pixels.
    const QRectF rect = printer->pageRect(QPrinter::Millimeter);
    QRect pageRect(qFloor(ToPixel(rect.x())), qFloor(ToPixel(rect.y())), qFloor(ToPixel(rect.width())),
                   qFloor(ToPixel(rect.height())));
    return pageRect;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VPoster::ToPixel(qreal val) const
{
    return val / 25.4 * printer->resolution(); // Mm to pixels with current dpi.
}
