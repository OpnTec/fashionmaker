/************************************************************************
 **
 **  @file   vsimplecurve.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 5, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef VSIMPLECURVE_H
#define VSIMPLECURVE_H

#include <qcompilerdetection.h>
#include <QColor>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QVariant>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "vabstractsimple.h"
#include "vcurvepathitem.h"

class VAbstractCurve;
template <class T> class QSharedPointer;

class VSimpleCurve : public VAbstractSimple, public VCurvePathItem
{
    Q_OBJECT
public:
    VSimpleCurve(quint32 id, const QSharedPointer<VAbstractCurve> &curve, QObject *parent = nullptr);
    virtual ~VSimpleCurve() Q_DECL_EQ_DEFAULT;

    virtual int  type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::SimpleCurve)};

    void RefreshGeometry(const QSharedPointer<VAbstractCurve> &curve);
signals:
    /**
     * @brief Choosed send id when clicked.
     * @param id point id.
     */
    void Choosed(quint32 id);
    void Selected(bool selected, quint32 id);

public slots:
    void CurveChoosed();
    void CurveSelected(bool selected);

protected:
    virtual void     mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
    virtual void     mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) override;
    virtual void     hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) override;
    virtual void     hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) override;
    virtual QVariant itemChange ( GraphicsItemChange change, const QVariant &value ) override;
    virtual void     contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) override;
    virtual void     keyReleaseEvent ( QKeyEvent * event ) override;
    virtual void     ScalePenWidth() override;

private:
    Q_DISABLE_COPY(VSimpleCurve)

    QSharedPointer<VAbstractCurve> m_curve;
    bool m_isHovered;
};

#endif // VSIMPLECURVE_H
