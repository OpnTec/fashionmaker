/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 11, 2016
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

#ifndef VTOOLSEAMALLOWANCE_H
#define VTOOLSEAMALLOWANCE_H

#include <QtGlobal>
#include <qcompilerdetection.h>
#include <QObject>
#include <QGraphicsPathItem>

#include "vinteractivetool.h"
#include "../vwidgets/vtextgraphicsitem.h"
#include "../vwidgets/vgrainlineitem.h"

class DialogTool;
class VNoBrushScalePathItem;

struct VToolSeamAllowanceInitData : VAbstractToolInitData
{
    VToolSeamAllowanceInitData()
        : VAbstractToolInitData(),
          detail(),
          width('0'),
          drawName()
    {}

    VPiece detail;
    QString width;
    QString drawName;
};

class VToolSeamAllowance : public VInteractiveTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    virtual ~VToolSeamAllowance() Q_DECL_EQ_DEFAULT;

    static VToolSeamAllowance* Create(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene,
                                      VAbstractPattern *doc, VContainer *data);
    static VToolSeamAllowance* Create(VToolSeamAllowanceInitData &initData);
    static VToolSeamAllowance* Duplicate(const QPointer<DialogTool> &dialog, VMainGraphicsScene *scene,
                                         VAbstractPattern *doc);
    static VToolSeamAllowance* Duplicate(VToolSeamAllowanceInitData &initData);

    static const quint8 pieceVersion;

    static const QString TagCSA;
    static const QString TagRecord;
    static const QString TagIPaths;
    static const QString TagPins;
    static const QString TagPlaceLabels;


    static const QString AttrSeamAllowance;
    static const QString AttrHideMainPath;
    static const QString AttrSeamAllowanceBuiltIn;
    static const QString AttrUnited;
    static const QString AttrFont;
    static const QString AttrTopLeftPin;
    static const QString AttrBottomRightPin;
    static const QString AttrCenterPin;
    static const QString AttrTopPin;
    static const QString AttrBottomPin;

    void RemoveWithConfirm(bool ask);

    static void InsertNode(VPieceNode node, quint32 pieceId, VMainGraphicsScene *scene, VContainer *data,
                           VAbstractPattern *doc);

    static void AddAttributes(VAbstractPattern *doc, QDomElement &domElement, quint32 id, const VPiece &piece);
    static void AddCSARecord(VAbstractPattern *doc, QDomElement &domElement, const CustomSARecord &record);
    static void AddCSARecords(VAbstractPattern *doc, QDomElement &domElement, const QVector<CustomSARecord> &records);
    static void AddInternalPaths(VAbstractPattern *doc, QDomElement &domElement, const QVector<quint32> &paths);
    static void AddPins(VAbstractPattern *doc, QDomElement &domElement, const QVector<quint32> &pins);
    static void AddPlaceLabels(VAbstractPattern *doc, QDomElement &domElement, const QVector<quint32> &placeLabels);
    static void AddPatternPieceData(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece);
    static void AddPatternInfo(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece);
    static void AddGrainline(VAbstractPattern *doc, QDomElement &domElement, const VPiece &piece);

    void Move(qreal x, qreal y);
    void Update(const VPiece &piece);
    void DisconnectOutsideSignals();
    void ConnectOutsideSignals();
    void ReinitInternals(const VPiece &detail, VMainGraphicsScene *scene);
    void RefreshGeometry(bool updateChildren = true);

    virtual int        type() const override {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Piece)};

    virtual QString      getTagName() const override;
    virtual void         ShowVisualization(bool show) override;
    virtual void         GroupVisibility(quint32 object, bool visible) override;
    virtual void         paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                               QWidget *widget) override;
    virtual QRectF       boundingRect() const override;
    virtual QPainterPath shape() const override;
public slots:
    virtual void FullUpdateFromGuiApply() override;
    virtual void FullUpdateFromFile () override;
    void         EnableToolMove(bool move);
    virtual void AllowHover(bool enabled) override;
    virtual void AllowSelecting(bool enabled) override;
    virtual void ResetChildren(QGraphicsItem* pItem);
    virtual void UpdateAll();
    virtual void retranslateUi();
    void         Highlight(quint32 id);
    void         UpdateDetailLabel();
    void         UpdatePatternInfo();
protected slots:
    void UpdateGrainline();
    void SaveMoveDetail(const QPointF &ptPos);
    void SaveResizeDetail(qreal dLabelW, int iFontSize);
    void SaveRotationDetail(qreal dRot);
    void SaveMovePattern(const QPointF& ptPos);
    void SaveResizePattern(qreal dLabelW, int iFontSize);
    void SaveRotationPattern(qreal dRot);
    void SaveMoveGrainline(const QPointF& ptPos);
    void SaveResizeGrainline(qreal dLength);
    void SaveRotateGrainline(qreal dRot, const QPointF& ptPos);
protected:
    virtual void       AddToFile () override;
    virtual void       RefreshDataInFile() override;
    virtual QVariant   itemChange ( GraphicsItemChange change, const QVariant &value ) override;
    virtual void       mousePressEvent( QGraphicsSceneMouseEvent * event) override;
    virtual void       mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) override;
    virtual void       hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) override;
    virtual void       hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) override;
    virtual void       contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) override;
    virtual void       keyReleaseEvent(QKeyEvent * event) override;
    virtual void       SetVisualization() override {}
    virtual void       DeleteToolWithConfirm(bool ask = true) override;
    virtual void       ToolCreation(const Source &typeCreation) override;
    virtual void       SetDialog() final;
    virtual void       SaveDialogChange(const QString &undoText = QString()) final;

private slots:
    void ShowOptions();
    void ToggleInLayout(bool checked);
    void ToggleForbidFlipping(bool checked);
    void ToggleForceFlipping(bool checked);
    void DeleteFromMenu();
    void ToggleExcludeState(quint32 id);
    void ToggleNodePointAngleType(quint32 id, PieceNodeAngle type);
    void ToggleNodePointPassmark(quint32 id, bool toggle);

private:
    Q_DISABLE_COPY(VToolSeamAllowance)

    QPainterPath m_mainPath; // Must be first to prevent crash
    QRectF       m_mainPathRect;

    /** @brief sceneDetails pointer to the scene. */
    VMainGraphicsScene *m_sceneDetails;
    QString             m_drawName;

    VNoBrushScalePathItem *m_seamAllowance;
    VTextGraphicsItem     *m_dataLabel;
    VTextGraphicsItem     *m_patternInfo;
    VGrainlineItem        *m_grainLine;
    QGraphicsPathItem     *m_passmarks;
    QGraphicsPathItem     *m_placeLabels;

    bool m_acceptHoverEvents;

    VToolSeamAllowance(const VToolSeamAllowanceInitData &initData, QGraphicsItem * parent = nullptr);

    void UpdateExcludeState();
    void UpdateInternalPaths();

    VPieceItem::MoveTypes FindLabelGeometry(const VPatternLabelData &labelData, qreal &rotationAngle, qreal &labelWidth,
                                            qreal &labelHeight, QPointF &pos);
    VPieceItem::MoveTypes FindGrainlineGeometry(const VGrainlineData &geom, qreal &length, qreal &rotationAngle,
                                                QPointF &pos);

    void InitNodes(const VPiece &detail, VMainGraphicsScene *scene);
    static void InitNode(const VPieceNode &node, VMainGraphicsScene *scene, VContainer *data, VAbstractPattern *doc,
                         VToolSeamAllowance *parent);
    void InitCSAPaths(const VPiece &detail) const;
    void InitInternalPaths(const VPiece &detail);
    void InitSpecialPoints(const QVector<quint32> &points) const;

    bool PrepareLabelData(const VPatternLabelData &labelData, VTextGraphicsItem *labelItem, QPointF &pos,
                          qreal &labelAngle);
    void UpdateLabelItem(VTextGraphicsItem *labelItem, QPointF pos, qreal labelAngle);

    QList<VToolSeamAllowance *> SelectedTools() const;

    static void AddPointRecords(VAbstractPattern *doc, QDomElement &domElement, const QVector<quint32> &records,
                                const QString &tag);

    static QVector<VPieceNode> DuplicateNodes(const VPiecePath &path, const VToolSeamAllowanceInitData &initData,
                                              QMap<quint32, quint32> &replacements);
    static quint32             DuplicateNode(const VPieceNode &node, const VToolSeamAllowanceInitData &initData);

    static quint32 DuplicatePiecePath(quint32 id, const VToolSeamAllowanceInitData &initData);

    static QVector<CustomSARecord> DuplicateCustomSARecords(const QVector<CustomSARecord> &records,
                                                            const VToolSeamAllowanceInitData &initData,
                                                            const QMap<quint32, quint32> &replacements);

    static QVector<quint32> DuplicateInternalPaths(const QVector<quint32> &iPaths,
                                                   const VToolSeamAllowanceInitData &initData);
    static QVector<quint32> DuplicatePins(const QVector<quint32> &pins, const VToolSeamAllowanceInitData &initData);
    static QVector<quint32> DuplicatePlaceLabels(const QVector<quint32> &placeLabels,
                                                 const VToolSeamAllowanceInitData &initData);
};

#endif // VTOOLSEAMALLOWANCE_H
