/************************************************************************
 **
 **  @file   vtoolcut.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef VTOOLCUT_H
#define VTOOLCUT_H

#include "vtoolpoint.h"
#include "../../visualization/vsimplecurve.h"

class VToolCut : public VToolPoint
{
    Q_OBJECT
public:
    VToolCut(VPattern *doc, VContainer *data, const quint32 &id, const QString &formula, const quint32 &curveCutId,
             const quint32 &curve1id, const quint32 &curve2id, QGraphicsItem * parent = nullptr);
public slots:
    virtual void  ChangedActivDraw(const QString &newName);
    virtual void  CurveChoosed(quint32 id)=0;
protected:
    /** @brief formula keep formula of length */
    QString       formula;

    /** @brief firstCurve first curve after cutting. */
    VSimpleCurve  *firstCurve;

    /** @brief secondCurve second curve after cutting. */
    VSimpleCurve  *secondCurve;

    quint32       curveCutId;
    quint32       curve1id;
    quint32       curve2id;

    virtual void  RefreshCurve(VSimpleCurve *curve, quint32 curveId, SimpleCurvePoint tr)=0;
    void          RefreshGeometry();
    virtual void  RemoveReferens();
    void          FullUpdateCurveFromFile(const QString &attrCurve);
private:
    Q_DISABLE_COPY(VToolCut)
};

#endif // VTOOLCUT_H
