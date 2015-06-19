/************************************************************************
 **
 **  @file   vtoolarcwithlength.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 6, 2015
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

#ifndef VTOOLARCWITHLENGTH_H
#define VTOOLARCWITHLENGTH_H

#include "vabstractspline.h"

class VFormula;

class VToolArcWithLength : public VAbstractSpline
{
    Q_OBJECT
public:
    VToolArcWithLength(VAbstractPattern *doc, VContainer *data, quint32 id, const QString &color,
                       const Source &typeCreation,
                       QGraphicsItem * parent = nullptr);

    virtual void     setDialog();
    static VToolArcWithLength* Create(DialogTool *dialog, VMainGraphicsScene  *scene, VAbstractPattern *doc,
                                      VContainer *data);
    static VToolArcWithLength* Create(const quint32 _id, const quint32 &center, QString &radius, QString &f1,
                                      QString &length, const QString &color, VMainGraphicsScene  *scene,
                                      VAbstractPattern *doc,
                                      VContainer *data, const Document &parse, const Source &typeCreation);
    static const QString TagName;
    static const QString ToolType;
    virtual int      type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::ArcWithLength)};
    virtual QString  getTagName() const;

    quint32          getCenter() const;
    void             setCenter(const quint32 &value);

    VFormula         GetFormulaRadius() const;
    void             SetFormulaRadius(const VFormula &value);

    VFormula         GetFormulaF1() const;
    void             SetFormulaF1(const VFormula &value);

    VFormula         GetFormulaLength() const;
    void             SetFormulaLength(const VFormula &value);

    virtual void     ShowVisualization(bool show);
protected:
    virtual void     contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void     RemoveReferens();
    virtual void     SaveDialog(QDomElement &domElement);
    virtual void     SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
    virtual void     SetVisualization();
private:
    void             RefreshGeometry();

};

#endif // VTOOLARCWITHLENGTH_H
