/************************************************************************
 **
 **  @file   vposter.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 4, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#include <QCoreApplication>
#include <QRect>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "../vlayout/vtextmanager.h"

class QGraphicsItem;
class QPrinter;
template <class T> class QVector;
struct VWatermarkData;

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
    quint32 row; // positions in the grid
    quint32 column;
    quint32 rows;
    quint32 columns;
    QRect rect; // rect section
};

Q_DECLARE_TYPEINFO(PosterData, Q_MOVABLE_TYPE);

// cppcheck-suppress noConstructor
class VPoster
{
    Q_DECLARE_TR_FUNCTIONS(VPoster)
public:
    explicit VPoster(const QPrinter *printer);

    QVector<PosterData> Calc(const QRect &imageRect, int page, PageOrientation orientation) const;

    QVector<QGraphicsItem *> Tile(QGraphicsItem *parent, const PosterData &img, int sheets,
                                  const VWatermarkData &watermarkData, const QString &watermarkPath) const;

private:
    const QPrinter *printer;
    /**
     * @brief allowance is the width of the strip that holds the tiled
     * grid information and that is used for the gluing.
     */
    quint32 allowance;

    int CountRows(int height, PageOrientation orientation) const;
    int CountColumns(int width, PageOrientation orientation) const;

    PosterData Cut(int i, int j, const QRect &imageRect, PageOrientation orientation) const;

    QRect PageRect() const;

    void Ruler(QVector<QGraphicsItem *> &data, QGraphicsItem *parent, QRect rec) const;

    QVector<QGraphicsItem *> Borders(QGraphicsItem *parent, const PosterData &img, int sheets) const;

    QVector<QGraphicsItem *> TextWatermark(QGraphicsItem *parent, const PosterData &img,
                                           const VWatermarkData &watermarkData) const;
    QVector<QGraphicsItem *> ImageWatermark(QGraphicsItem *parent, const PosterData &img,
                                            const VWatermarkData &watermarkData, const QString &watermarkPath) const;
};

#endif // VPOSTER_H
