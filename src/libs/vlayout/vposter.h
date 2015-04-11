/************************************************************************
 **
 **  @file   vposter.h
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

#ifndef VPOSTER_H
#define VPOSTER_H

#include <QImage>
#include <QRect>
#include <QCoreApplication>

class QPrinter;

class VPoster
{
    Q_DECLARE_TR_FUNCTIONS(VPoster)
public:
    VPoster(const QPrinter *printer);

    QVector<QImage> Generate(const QImage &image, int page, int pages = 1) const;
private:
    const QPrinter *printer;
    unsigned int allowence;

    int CountRows(int height) const;
    int CountColomns(int width) const;

    QImage Cut(int i, int j, const QImage &image) const;
    QImage Borders(int rows, int colomns, int i, int j, QImage &image, int page, int pages) const;

    QRect PageRect() const;

    qreal ToPixel(qreal val) const;
};

#endif // VPOSTER_H
