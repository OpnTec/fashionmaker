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

#ifndef VTOOLCUT_H
#define VTOOLCUT_H

#include "vtoolpoint.h"
#include "../../visualization/vsimplecurve.h"
#include "vabstractspline.h"

class VFormula;

class VToolCut : public VToolPoint
{
    Q_OBJECT
public:
    VToolCut(VPattern *doc, VContainer *data, const quint32 &id, const QString &formula, const quint32 &curveCutId,
             const quint32 &curve1id, const quint32 &curve2id, const QString &color, QGraphicsItem * parent = nullptr);
    virtual int   type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Cut)};

    VFormula GetFormula() const;
    void     SetFormula(const VFormula &value);

    quint32 getCurveCutId() const;
    void    setCurveCutId(const quint32 &value);

public slots:
    virtual void  ChangedActivDraw(const QString &newName);
    virtual void  CurveChoosed(quint32 id)=0;
    void          HoverPath(quint32 id, SimpleCurvePoint curvePosition, PathDirection direction);
    void          Disable(bool disable);
    void          DetailsMode(bool mode);
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

    bool          detailsMode;

    virtual void  RefreshCurve(VSimpleCurve *curve, quint32 curveId, SimpleCurvePoint curvePosition,
                               PathDirection direction = PathDirection::Hide)=0;
    void          RefreshGeometry();
    virtual void  RemoveReferens();
    void          FullUpdateCurveFromFile(const QString &attrCurve);

    template <typename T>
    void ShowToolVisualization(bool show)
    {
        if (show)
        {
            if (vis == nullptr)
            {
                AddVisualization<T>();
                SetVisualization();
            }
            else
            {
                if (T *visual = qobject_cast<T *>(vis))
                {
                    visual->show();
                }
            }
        }
        else
        {
            delete vis;
            vis = nullptr;
        }
        if (VAbstractSpline *parentCurve = qobject_cast<VAbstractSpline *>(doc->getTool(curveCutId)))
        {
            if (detailsMode)
            {
                parentCurve->ShowHandles(detailsMode);
            }
            else
            {
                parentCurve->ShowHandles(show);
            }
        }
    }
private:
    Q_DISABLE_COPY(VToolCut)
};

#endif // VTOOLCUT_H
