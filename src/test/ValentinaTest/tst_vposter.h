/************************************************************************
 **
 **  @file   tst_vposter.h
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

#ifndef TST_VPOSTER_H
#define TST_VPOSTER_H

#include <QObject>

class QPrinter;

class TST_VPoster : public QObject
{
    Q_OBJECT

public:
    explicit TST_VPoster(QObject *parent = nullptr);

signals:

private slots:
    void BigPoster();
    void SmallPoster();

private:
    qreal ToPixel(qreal val, const QPrinter &printer) const;
    QRect PageRect(const QPrinter &printer) const;
};

#endif // TST_VPOSTER_H
