/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   17 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef TST_VABSTRACTPIECE_H
#define TST_VABSTRACTPIECE_H

#include "../vmisc/abstracttest.h"

class VSAPoint;

class TST_VAbstractPiece : public AbstractTest
{
    Q_OBJECT
public:
    explicit TST_VAbstractPiece(QObject *parent = nullptr);

private slots:
    void EquidistantRemoveLoop_data();
    void EquidistantRemoveLoop() const;
private:
    QVector<VSAPoint> InputPointsCase1() const;
    QVector<QPointF>  OutputPointsCase1() const;

    QVector<VSAPoint> InputPointsCase2() const;
    QVector<QPointF>  OutputPointsCase2() const;

    QVector<VSAPoint> InputPointsCase3() const;
    QVector<QPointF>  OutputPointsCase3() const;
};

#endif // TST_VABSTRACTPIECE_H
