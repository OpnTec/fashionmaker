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

#include <QRect>
#include <QCoreApplication>

class QPrinter;
class QGraphicsItem;

struct PosterData
{
    PosterData()
        : index(0),
          row(0),
          column(0),
          rows(0),
          columns(0),
          rect(){}

    quint32 index; // paper index
    quint32 row; // positions in the greed
    quint32 column;
    quint32 rows;
    quint32 columns;
    QRect rect; // rect section
};

class VPoster
{
    Q_DECLARE_TR_FUNCTIONS(VPoster)
public:
    explicit VPoster(const QPrinter *printer);

    QVector<PosterData> Calc(const QRect &imageRect, int page) const;

    QVector<QGraphicsItem *> Borders(QGraphicsItem *parent, const PosterData &img, int sheets) const;
private:
    const QPrinter *printer;
    quint32 allowence;

    int CountRows(int height) const;
    int CountColumns(int width) const;

    PosterData Cut(int i, int j, const QRect &imageRect) const;

    QRect PageRect() const;

    static qreal ToPixel(qreal val);
};

#endif // VPOSTER_H
