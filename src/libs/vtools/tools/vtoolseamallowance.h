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
#include <QGraphicsPathItem>

#include "vabstracttool.h"
#include "../vwidgets/vtextgraphicsitem.h"
#include "../vwidgets/vgrainlineitem.h"

class VMainGraphicsScene;
class DialogTool;
class VPiece;
class VNoBrushScalePathItem;

class VToolSeamAllowance : public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    virtual ~VToolSeamAllowance();

    static VToolSeamAllowance* Create(DialogTool *m_dialog, VMainGraphicsScene *scene, VAbstractPattern *doc,
                                      VContainer *data);
    static VToolSeamAllowance* Create(quint32 id, VPiece newPiece, QString &width, VMainGraphicsScene *scene,
                                      VAbstractPattern *doc, VContainer *data, const Document &parse,
                                      const Source &typeCreation, const QString &m_drawName = QString());

    static const quint8 pieceVersion;

    static const QString TagCSA;
    static const QString TagRecord;
    static const QString TagIPaths;

    static const QString AttrVersion;
    static const QString AttrForbidFlipping;
    static const QString AttrSeamAllowance;
    static const QString AttrHeight;
    static const QString AttrUnited;
    static const QString AttrFont;

    void Remove(bool ask);

    static void AddAttributes(VAbstractPattern *doc, QDomElement &domElement, quint32 id, const VPiece &piece);
    static void AddCSARecord(VAbstractPattern *doc, QDomElement &domElement, const CustomSARecord &record);
    static void AddCSARecords(VAbstractPattern *doc, QDomElement &domElement, const QVector<CustomSARecord> &records);
    static void AddInternalPaths(VAbstractPattern *doc, QDomElement &domElement, const QVector<quint32> &paths);
    static void AddPatternPieceData(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece);
    static void AddPatternInfo(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece);
    static void AddGrainline(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece);

    virtual int        type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Piece)};

    virtual QString    getTagName() const Q_DECL_OVERRIDE;
    virtual void       ShowVisualization(bool show) Q_DECL_OVERRIDE;
    virtual void       GroupVisibility(quint32 object, bool visible) Q_DECL_OVERRIDE;
public slots:
    virtual void FullUpdateFromFile () Q_DECL_OVERRIDE;
    virtual void FullUpdateFromGuiOk(int result);
    void         FullUpdateFromGuiApply();
    void         EnableToolMove(bool move);
    virtual void AllowHover(bool enabled) Q_DECL_OVERRIDE;
    virtual void AllowSelecting(bool enabled) Q_DECL_OVERRIDE;
    virtual void ResetChildren(QGraphicsItem* pItem);
    virtual void UpdateAll();
    virtual void retranslateUi();
    void         Highlight(quint32 id);
protected slots:
    virtual void UpdateLabel();
    virtual void UpdatePatternInfo();
    virtual void UpdateGrainline();
    virtual void SaveMoveDetail(const QPointF &ptPos);
    virtual void SaveResizeDetail(qreal dLabelW, int iFontSize);
    virtual void SaveRotationDetail(qreal dRot);
    virtual void SaveMovePattern(const QPointF& ptPos);
    virtual void SaveResizePattern(qreal dLabelW, int iFontSize);
    virtual void SaveRotationPattern(qreal dRot);
    virtual void SaveMoveGrainline(const QPointF& ptPos);
    virtual void SaveResizeGrainline(qreal dLength);
    virtual void SaveRotateGrainline(qreal dRot, const QPointF& ptPos);
protected:
    virtual void       paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                             QWidget *widget) Q_DECL_OVERRIDE;
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
    virtual void       DeleteTool(bool ask = true) Q_DECL_OVERRIDE;
    virtual void       ToolCreation(const Source &typeCreation) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VToolSeamAllowance)

    /** @brief dialog dialog options. */
    QPointer<DialogTool> m_dialog;

    /** @brief sceneDetails pointer to the scene. */
    VMainGraphicsScene *m_sceneDetails;
    QString             m_drawName;

    VNoBrushScalePathItem *m_seamAllowance;
    VTextGraphicsItem     *m_dataLabel;
    VTextGraphicsItem     *m_patternInfo;
    VGrainlineItem        *m_grainLine;

    void SetDialog();

    VToolSeamAllowance(VAbstractPattern *doc, VContainer *data, const quint32 &id, const Source &typeCreation,
                       VMainGraphicsScene *scene, const QString &m_drawName, QGraphicsItem * parent = nullptr);

    void RefreshGeometry();
    void SaveDialogChange();

    void InitNodes(const VPiece &detail, VMainGraphicsScene *scene);
    void InitCSAPaths(const VPiece &detail);
    void InitInternalPaths(const VPiece &detail);

    template <typename Tool>
    Tool*              InitTool(VMainGraphicsScene *scene, quint32 toolId);
};

#endif // VTOOLSEAMALLOWANCE_H
