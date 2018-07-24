/************************************************************************
 **
 **  @file   vtoolellipticalarc.h
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   20 10, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef VTOOLELLIPTICALARC_H
#define VTOOLELLIPTICALARC_H

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

struct VToolEllipticalArcInitData : VAbstractSplineInitData
{
    VToolEllipticalArcInitData()
        : VAbstractSplineInitData(),
          center(NULL_ID),
          radius1('0'),
          radius2('0'),
          f1('0'),
          f2('0'),
          rotationAngle('0')
    {}

    quint32 center;
    QString radius1;
    QString radius2;
    QString f1;
    QString f2;
    QString rotationAngle;
};

/**
 * @brief The VToolEllipticalArc class tool for creation elliptical arc.
 */
class VToolEllipticalArc : public VToolAbstractArc
{
    Q_OBJECT
public:
    virtual void     setDialog() override;
    static VToolEllipticalArc* Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene  *scene,
                                      VAbstractPattern *doc, VContainer *data);
    static VToolEllipticalArc* Create(VToolEllipticalArcInitData &initData);
    static const QString ToolType;
    virtual int      type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::EllipticalArc)};
    virtual QString  getTagName() const override;

    VFormula         GetFormulaRadius1() const;
    void             SetFormulaRadius1(const VFormula &value);

    VFormula         GetFormulaRadius2() const;
    void             SetFormulaRadius2(const VFormula &value);

    VFormula         GetFormulaF1() const;
    void             SetFormulaF1(const VFormula &value);

    VFormula         GetFormulaF2() const;
    void             SetFormulaF2(const VFormula &value);

    VFormula         GetFormulaRotationAngle() const;
    void             SetFormulaRotationAngle(const VFormula &value);

    virtual void     ShowVisualization(bool show) override;
protected slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) override;
protected:
    virtual void    RemoveReferens() override;
    virtual void    SaveDialog(QDomElement &domElement, QList<quint32> &oldDependencies,
                               QList<quint32> &newDependencies) override;
    virtual void    SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) override;
    virtual void    SetVisualization() override;
    virtual QString MakeToolTip() const override;

private:
    Q_DISABLE_COPY(VToolEllipticalArc)
    VToolEllipticalArc(const VToolEllipticalArcInitData &initData, QGraphicsItem *parent = nullptr);
    virtual ~VToolEllipticalArc()=default;
};

#endif // VTOOLELLIPTICALARC_H
