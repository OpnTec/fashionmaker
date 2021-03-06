/************************************************************************
 **
 **  @file   vtoolarc.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VTOOLARC_H
#define VTOOLARC_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vabstractspline.h"

class VFormula;
template <class T> class QSharedPointer;

struct VToolArcInitData : VAbstractSplineInitData
{
    VToolArcInitData()
        : VAbstractSplineInitData(),
          center(NULL_ID),
          radius('0'),
          f1('0'),
          f2('0')
    {}

    quint32 center;
    QString radius;
    QString f1;
    QString f2;
};

/**
 * @brief The VToolArc class tool for creation arc.
 */
class VToolArc :public VToolAbstractArc
{
    Q_OBJECT
public:
    virtual void     setDialog() override;
    static VToolArc* Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene  *scene, VAbstractPattern *doc,
                            VContainer *data);
    static VToolArc* Create(VToolArcInitData &initData);

    static const QString ToolType;
    virtual int      type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Arc)};
    virtual QString  getTagName() const override;

    VFormula         GetFormulaRadius() const;
    void             SetFormulaRadius(const VFormula &value);

    VFormula         GetFormulaF1() const;
    void             SetFormulaF1(const VFormula &value);

    VFormula         GetFormulaF2() const;
    void             SetFormulaF2(const VFormula &value);

    qreal            GetApproximationScale() const;
    void             SetApproximationScale(qreal value);

    virtual void     ShowVisualization(bool show) override;
protected slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
protected:
    virtual void     RemoveReferens() override;
    virtual void     SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                                QList<quint32> &newDependencies) override;
    virtual void     SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual void     SetVisualization() override;
    virtual QString  MakeToolTip() const override;
private:
    Q_DISABLE_COPY(VToolArc)

    VToolArc(const VToolArcInitData &initData, QGraphicsItem * parent = nullptr);
    virtual ~VToolArc()=default;
};

#endif // VTOOLARC_H
