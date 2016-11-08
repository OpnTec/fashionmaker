/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#ifndef VTOOLSEAMALLOWANCE_H
#define VTOOLSEAMALLOWANCE_H

#include <QtGlobal>
#include <qcompilerdetection.h>
#include <QObject>

#include "../vwidgets/vnobrushscalepathitem.h"
#include "vabstracttool.h"

class VMainGraphicsScene;
class DialogTool;
class VPiece;

class VToolSeamAllowance : public VAbstractTool, public VNoBrushScalePathItem
{
    Q_OBJECT
public:
    virtual ~VToolSeamAllowance();

    static VToolSeamAllowance* Create(DialogTool *m_dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                      VContainer *data);
    static VToolSeamAllowance* Create(quint32 id, const VPiece &newPiece, VMainGraphicsScene *scene,
                                      VAbstractPattern *doc, VContainer *data, const Document &parse,
                                      const Source &typeCreation, const QString &m_drawName = QString());

    static const quint8 pieceVersion;

    static const QString TagNodes;
    static const QString TagNode;

    static const QString AttrVersion;
    static const QString AttrNodeReverse;

    static const QString NodeArc;
    static const QString NodePoint;
    static const QString NodeSpline;
    static const QString NodeSplinePath;

    static void AddNode(VAbstractPattern *doc, QDomElement &domElement, const VPieceNode &node);
    static void AddNodes(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece);

    virtual int        type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Piece)};

    virtual QString    getTagName() const Q_DECL_OVERRIDE;
    virtual void       ShowVisualization(bool show) Q_DECL_OVERRIDE;
    virtual void       GroupVisibility(quint32 object, bool visible) Q_DECL_OVERRIDE;
public slots:
    virtual void       FullUpdateFromFile () Q_DECL_OVERRIDE;
    virtual void       FullUpdateFromGuiOk(int result);
    void               EnableToolMove(bool move);
    virtual void       AllowHover(bool enabled) Q_DECL_OVERRIDE;
    virtual void       AllowSelecting(bool enabled) Q_DECL_OVERRIDE;
    void               Highlight(quint32 id);
protected:
    virtual void       AddToFile () Q_DECL_OVERRIDE;
    virtual void       RefreshDataInFile() Q_DECL_OVERRIDE;
    virtual QVariant   itemChange ( GraphicsItemChange change, const QVariant &value ) Q_DECL_OVERRIDE;
    virtual void       mousePressEvent( QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;
    virtual void       mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void       hoverMoveEvent( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void       hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void       hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) Q_DECL_OVERRIDE;
    virtual void       contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void       keyReleaseEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    virtual void       SetVisualization() Q_DECL_OVERRIDE {}

private:
    Q_DISABLE_COPY(VToolSeamAllowance)

    /** @brief dialog dialog options. */
    DialogTool         *m_dialog;

    /** @brief sceneDetails pointer to the scene. */
    VMainGraphicsScene *m_sceneDetails;
    QString             m_drawName;

    void SetDialog();

    template<typename T>
    static quint32 CreateNode(VContainer *data, const quint32 &id);

    VToolSeamAllowance(VAbstractPattern *doc, VContainer *data, const quint32 &id, const Source &typeCreation,
                       VMainGraphicsScene *scene, const QString &m_drawName, QGraphicsItem * parent = nullptr);

    void               RefreshGeometry ();

    template <typename Tool>
    Tool*              InitTool(VMainGraphicsScene *scene, const VPieceNode &node);

    virtual void       DeleteTool(bool ask = true) Q_DECL_OVERRIDE;
};

#endif // VTOOLSEAMALLOWANCE_H
