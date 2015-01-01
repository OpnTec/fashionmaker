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

#ifndef VABSTRACTTOOL_H
#define VABSTRACTTOOL_H

#include "vdatatool.h"
#include "../xml/vpattern.h"

class QDomElement;
class QLineF;
class QPointF;
class QGraphicsScene;
class QGraphicsView;
class QGraphicsItem;
class QRectF;
class Visualization;

/**
 * @brief The VAbstractTool abstract class for all tools.
 */
class VAbstractTool: public VDataTool
{
    Q_OBJECT
public:
    VAbstractTool(VPattern *doc, VContainer *data, quint32 id, QObject *parent = nullptr);
    virtual ~VAbstractTool();
    static void             NewSceneRect(QGraphicsScene *sc, QGraphicsView *view);
    quint32                 getId() const;
    static const QString    AttrType;
    static const QString    AttrMx;
    static const QString    AttrMy;
    static const QString    AttrName;
    static const QString    AttrX;
    static const QString    AttrY;
    static const QString    AttrTypeLine;
    static const QString    AttrLength;
    static const QString    AttrBasePoint;
    static const QString    AttrFirstPoint;
    static const QString    AttrSecondPoint;
    static const QString    AttrThirdPoint;
    static const QString    AttrCenter;
    static const QString    AttrRadius;
    static const QString    AttrAngle;
    static const QString    AttrAngle1;
    static const QString    AttrAngle2;
    static const QString    AttrP1Line;
    static const QString    AttrP2Line;
    static const QString    AttrP1Line1;
    static const QString    AttrP2Line1;
    static const QString    AttrP1Line2;
    static const QString    AttrP2Line2;
    static const QString    AttrPShoulder;
    static const QString    AttrPoint1;
    static const QString    AttrPoint4;
    static const QString    AttrKAsm1;
    static const QString    AttrKAsm2;
    static const QString    AttrKCurve;
    static const QString    AttrPathPoint;
    static const QString    AttrPSpline;
    static const QString    AttrAxisP1;
    static const QString    AttrAxisP2;
    static const QString    AttrCurve;
    static const QString    TypeLineNone;
    static const QString    TypeLineLine;
    static const QString    TypeLineDashLine;
    static const QString    TypeLineDotLine;
    static const QString    TypeLineDashDotLine;
    static const QString    TypeLineDashDotDotLine;
    static const QStringList Styles();
    static void             AddRecord(const quint32 id, const Tool &toolType, VPattern *doc);
    static Qt::PenStyle     LineStyle(const QString &typeLine);
    const VContainer        *getData() const;

    QString                 getLineType() const;
    virtual void            setTypeLine(const QString &value);
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
    VPattern                *doc;

    /** @brief id object id. */
    const quint32            id;

    /** @brief baseColor base color for tool. */
    Qt::GlobalColor         baseColor;

    /** @brief currentColor current tool color. */
    Qt::GlobalColor         currentColor;

    /** @brief typeLine line type. */
    QString                 typeLine;

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
    virtual void            RemoveReferens(){}
    virtual void            DeleteTool(bool ask = true);
    static int              ConfirmDeletion();
    void                    SaveOption(QSharedPointer<VGObject> &obj);
    virtual void            SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj)=0;
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
#endif // VABSTRACTTOOL_H
