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
 **  Copyright (C) 2013 Valentina project
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

#include "vabstractspline.h"

class VFormula;

/**
 * @brief The VToolArc class tool for creation arc.
 */
class VToolArc :public VAbstractSpline
{
    Q_OBJECT
public:
    VToolArc(VPattern *doc, VContainer *data, quint32 id, const Source &typeCreation, QGraphicsItem * parent = nullptr);
    virtual void     setDialog();
    static VToolArc* Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
    static VToolArc* Create(const quint32 _id, const quint32 &center, QString &radius, QString &f1, QString &f2,
                            VMainGraphicsScene  *scene, VPattern *doc, VContainer *data, const Document &parse,
                            const Source &typeCreation);
    static const QString TagName;
    static const QString ToolType;
    virtual int      type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Arc)};
    virtual QString  getTagName() const;

    quint32          getCenter() const;
    void             setCenter(const quint32 &value);

    VFormula         getFormulaRadius() const;
    void             setFormulaRadius(const VFormula &value);

    VFormula         getFormulaF1() const;
    void             setFormulaF1(const VFormula &value);

    VFormula         getFormulaF2() const;
    void             setFormulaF2(const VFormula &value);

    virtual void     ShowVisualization(bool show);
protected:
    virtual void     contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void     RemoveReferens();
    virtual void     SaveDialog(QDomElement &domElement);
    virtual void     SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
private:
    void             RefreshGeometry();
};

#endif // VTOOLARC_H
