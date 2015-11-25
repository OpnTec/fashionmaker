/************************************************************************
 **
 **  @file   vtoolcutarc.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2014
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

#ifndef VTOOLCUTARC_H
#define VTOOLCUTARC_H

#include "vtoolcut.h"

/**
 * @brief The VToolCutArc class tool for cutting arc.
 */
class VToolCutArc : public VToolCut
{
    Q_OBJECT
public:
    VToolCutArc(VAbstractPattern *doc, VContainer *data, const quint32 &id, const QString &formula,
                const quint32 &arcId,
                const quint32 &arc1id, const quint32 &arc2id, const QString &color, const Source &typeCreation,
                QGraphicsItem * parent = nullptr);
    virtual void setDialog() Q_DECL_OVERRIDE;
    static VToolCutArc*  Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data);
    static VToolCutArc*  Create(const quint32 _id, const QString &pointName, QString &formula, const quint32 &arcId,
                                const qreal &mx, const qreal &my, const QString &color, VMainGraphicsScene *scene,
                                VAbstractPattern *doc, VContainer *data, const Document &parse,
                                const Source &typeCreation);
    static const QString ToolType;
    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::CutArc)};
    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;
protected:
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void SetVisualization() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolCutArc)
};

#endif // VTOOLCUTARC_H
