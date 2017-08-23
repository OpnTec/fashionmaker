/************************************************************************
 **
 **  @file   vtoolarc.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

/**
 * @brief The VToolArc class tool for creation arc.
 */
class VToolArc :public VAbstractSpline
{
    Q_OBJECT
public:
    virtual void     setDialog() Q_DECL_OVERRIDE;
    static VToolArc* Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene  *scene, VAbstractPattern *doc,
                            VContainer *data);
    static VToolArc* Create(const quint32 _id, const quint32 &center, QString &radius, QString &f1, QString &f2,
                            const QString &color, const QString &penStyle, VMainGraphicsScene  *scene,
                            VAbstractPattern *doc, VContainer *data, const Document &parse, const Source &typeCreation);

    static const QString ToolType;
    virtual int      type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Arc)};
    virtual QString  getTagName() const Q_DECL_OVERRIDE;

    QString CenterPointName() const;

    quint32          getCenter() const;
    void             setCenter(const quint32 &value);

    VFormula         GetFormulaRadius() const;
    void             SetFormulaRadius(const VFormula &value);

    VFormula         GetFormulaF1() const;
    void             SetFormulaF1(const VFormula &value);

    VFormula         GetFormulaF2() const;
    void             SetFormulaF2(const VFormula &value);

    virtual void     ShowVisualization(bool show) Q_DECL_OVERRIDE;
protected:
    virtual void     contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void     RemoveReferens() Q_DECL_OVERRIDE;
    virtual void     SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void     SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void     SetVisualization() Q_DECL_OVERRIDE;
    virtual QString  MakeToolTip() const Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolArc)

    VToolArc(VAbstractPattern *doc, VContainer *data, quint32 id, const Source &typeCreation,
             QGraphicsItem * parent = nullptr);
};

#endif // VTOOLARC_H
