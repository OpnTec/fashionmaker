/************************************************************************
 **
 **  @file   vabstractdetail_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VABSTRACTDETAIL_P_H
#define VABSTRACTDETAIL_P_H

#include <QSharedData>
#include <QString>

#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

class VAbstractDetailData : public QSharedData
{
public:
    VAbstractDetailData()
        :name(QString()), seamAllowance(false), closed(true), width(0), forbidFlipping(false)
    {}

    explicit VAbstractDetailData(const QString &name)
        :name(name), seamAllowance(false), closed(true), width(0), forbidFlipping(false)
    {}

    VAbstractDetailData(const VAbstractDetailData &detail)
        :QSharedData(detail), name(detail.name), seamAllowance(detail.seamAllowance), closed(detail.closed),
          width(detail.width), forbidFlipping(detail.forbidFlipping)
    {}

    ~VAbstractDetailData() {}

    /** @brief name detail name. */
    QString        name;
    /** @brief seamAllowance status seamAllowance detail. */
    bool           seamAllowance;
    /** @brief closed status equdistant detail. */
    bool           closed;
    /** @brief width value seamAllowance in mm. */
    qreal          width;
    /** @brief forbidFlipping forbid piece be mirrored in a layout. */
    bool           forbidFlipping;

private:
    VAbstractDetailData &operator=(const VAbstractDetailData &) Q_DECL_EQ_DELETE;
};

QT_WARNING_POP

#endif // VABSTRACTDETAIL_P_H
