/************************************************************************
 **
 **  @file   vabstracttool.h
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

#ifndef VABSTRACTTOOL_H
#define VABSTRACTTOOL_H

#include "vdatatool.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/vabstractapplication.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../visualization/visualization.h"

class QDomElement;
class QLineF;
class QPointF;
class QGraphicsScene;
class QGraphicsView;
class QGraphicsItem;
class QRectF;
class Visualization;
class VGraphicsSimpleTextItem;

/**
 * @brief The VAbstractTool abstract class for all tools.
 */
class VAbstractTool: public VDataTool
{
    Q_OBJECT
public:
    VAbstractTool(VAbstractPattern *doc, VContainer *data, quint32 id, QObject *parent = nullptr);
    virtual ~VAbstractTool() Q_DECL_OVERRIDE;
    quint32                 getId() const;

    static const QString AttrInUse;

    static const QStringList    StylesList();
    static Qt::PenStyle         LineStyleToPenStyle(const QString &typeLine);
    static QMap<QString, QIcon> LineStylesPics();

    static const QStringList      Colors();
    static QMap<QString, QString> ColorsList();

    static void             AddRecord(const quint32 id, const Tool &toolType, VAbstractPattern *doc);

    const VContainer        *getData() const;

    QMap<QString, quint32>  PointsList() const;
    virtual QString         getTagName() const =0;
    virtual void            ShowVisualization(bool show) =0;
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void            FullUpdateFromFile()=0;
signals:
    /**
     * @brief toolhaveChange emit if tool create change that need save.
     */
    void                    toolhaveChange();
    /**
     * @brief ChoosedTool emit if object was clicked.
     * @param id object id in container.
     * @param type type of scene object.
     */
    void                    ChoosedTool(quint32 id, SceneObject type);
    /**
     * @brief FullUpdateTree emit if need reparse pattern file.
     */
    void                    LiteUpdateTree(const Document &parse);
protected:
    /** @brief doc dom document container */
    VAbstractPattern         *doc;

    /** @brief id object id. */
    const quint32            id;

    /** @brief baseColor base color for tool. */
    Qt::GlobalColor         baseColor;

    Visualization           *vis;

    /**
     * @brief AddToFile add tag with informations about tool into file.
     */
    virtual void            AddToFile()=0;
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void            RefreshDataInFile()=0;
    /**
     * @brief RemoveReferens decrement value of reference.
     */
    virtual void            RemoveReferens() {}
    virtual void            DeleteTool(bool ask = true);
    static int              ConfirmDeletion();

    template <typename T>
    void AddVisualization();

    virtual void SetVisualization()=0;
    void ToolCreation(const Source &typeCreation);

    static void RefreshLine(QGraphicsEllipseItem *point, VGraphicsSimpleTextItem *namePoint, QGraphicsLineItem *lineName,
                            const qreal radius);
private:
    Q_DISABLE_COPY(VAbstractTool)
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getId return object id.
 * @return id.
 */
inline quint32 VAbstractTool::getId() const
{
    return id;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getData return pointer to data container.
 * @return container.
 */
inline const VContainer *VAbstractTool::getData() const
{
    return &data;
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void VAbstractTool::AddVisualization()
{
    T *visual = new T(getData());
    VMainGraphicsScene *scene = qobject_cast<VMainGraphicsScene *>(qApp->getCurrentScene());
    SCASSERT(scene != nullptr)
    connect(scene, &VMainGraphicsScene::NewFactor, visual, &Visualization::SetFactor);
    scene->addItem(visual);

    vis = visual;
}

#endif // VABSTRACTTOOL_H
