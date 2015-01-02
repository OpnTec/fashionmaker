/************************************************************************
 **
 **  @file   vabstractdetail.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 1, 2015
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

#ifndef VABSTRACTDETAIL_H
#define VABSTRACTDETAIL_H

#include <QSharedDataPointer>

class QString;
class VAbstractDetailData;

/**
 * @brief The VAbstractDetail class abstract class for all details.
 */
class VAbstractDetail
{
public:
    VAbstractDetail();
    VAbstractDetail(const QString &name);
    VAbstractDetail(const VAbstractDetail &detail);
    VAbstractDetail &operator=(const VAbstractDetail &detail);
    ~VAbstractDetail();

    void    Clear();

    QString getName() const;
    void    setName(const QString &value);

    bool    getSeamAllowance() const;
    void    setSeamAllowance(bool value);

    bool    getClosed() const;
    void    setClosed(bool value);

    qreal   getWidth() const;
    void    setWidth(const qreal &value);
private:
    QSharedDataPointer<VAbstractDetailData> d;
};

#endif // VABSTRACTDETAIL_H
