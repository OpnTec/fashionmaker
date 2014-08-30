/************************************************************************
 **
 **  @file   vtoolendline.h
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

#ifndef VTOOLENDLINE_H
#define VTOOLENDLINE_H

#include "vtoollinepoint.h"

/**
 * @brief The VToolEndLine class tool for creation point on the line end.
 */
class VToolEndLine : public VToolLinePoint
{
    Q_OBJECT
public:

    VToolEndLine(VPattern *doc, VContainer *data, const quint32 &id, const QString &typeLine,
                 const QString &formulaLength, const QString &formulaAngle, const quint32 &basePointId,
                 const Source &typeCreation, QGraphicsItem * parent = nullptr);
    virtual void setDialog();
    static VToolEndLine *Create(DialogTool *dialog, VMainGraphicsScene  *scene, VPattern *doc, VContainer *data);
    static VToolEndLine *Create(const quint32 _id, const QString &pointName, const QString &typeLine,
                                QString &formulaLength, QString &formulaAngle, const quint32 &basePointId,
                                const qreal &mx, const qreal &my, VMainGraphicsScene  *scene, VPattern *doc,
                                VContainer *data, const Document &parse, const Source &typeCreation);
    static const QString ToolType;
    virtual int  type() const {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::EndLine)};

    VFormula     getFormulaAngle() const;
    void         setFormulaAngle(const VFormula &value);

public slots:
    virtual void FullUpdateFromFile();
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void SaveDialog(QDomElement &domElement);
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj);
private:
    QString formulaAngle;
};

#endif // VTOOLENDLINE_H
