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
#include <QMessageBox>

/**
 * @brief The VAbstractTool abstract class for all tools.
 */
class VAbstractTool: public VDataTool
{
    Q_OBJECT
public:
    /**
     * @brief VAbstractTool container.
     * @param doc dom document container.
     * @param data container with data.
     * @param id object id in container.
     * @param parent parent object.
     */
    VAbstractTool(VPattern *doc, VContainer *data, quint32 id, QObject *parent = nullptr);
    virtual ~VAbstractTool(){}
    /**
     * @brief NewSceneRect calculate scene rect what contains all items and doesn't less that size of scene view.
     * @param sc scene.
     * @param view view.
     */
    static void             NewSceneRect(QGraphicsScene *sc, QGraphicsView *view);
    /**
     * @brief LineIntersectRect find point intersection line and rect.
     * @param rec rect.
     * @param line line.
     * @return point intersection.
     */
    static QPointF          LineIntersectRect(QRectF rec, QLineF line);
    /**
     * @brief LineIntersectCircle find point intersection line and circle.
     * @param center arc center.
     * @param radius arc radius.
     * @param line line
     * @param p1 first intersection point.
     * @param p2 second intersection point.
     * @return 0 - intersection doesn't exist, 1 - one intersection point, 2 - two intersection points.
     */
    static qint32           LineIntersectCircle(const QPointF &center, qreal radius, const QLineF &line, QPointF &p1,
                                                QPointF &p2);
    /**
     * @brief ClosestPoint find point projection of point onto line.
     * @param line line.
     * @return point on line or extended line if origin size too small.
     */
    static QPointF          ClosestPoint(const QLineF &line, const QPointF &point);
    static QPointF          addVector (const QPointF &p, const QPointF &p1, const QPointF &p2, qreal k);
    /**
     * @brief getId return object id.
     * @return id.
     */
    quint32                 getId() const;
    /**
     * @brief LineCoefficients coefficient for equation of segment. Segment equestion ax+by+c=0.
     * @param line line
     * @param a a value
     * @param b b value
     * @param c c value
     */
    static void             LineCoefficients(const QLineF &line, qreal *a, qreal *b, qreal *c);
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
    static const QString    TypeLineNone;
    static const QString    TypeLineLine;
    static const QString    TypeLineDashLine;
    static const QString    TypeLineDotLine;
    static const QString    TypeLineDashDotLine;
    static const QString    TypeLineDashDotDotLine;
    /**
     * @brief Styles return list of all line styles.
     * @return list of all line styles.
     */
    static const QStringList Styles();
    /**
     * @brief AddRecord add record about tool in history.
     * @param id object id in container
     * @param toolType tool type
     * @param doc dom document container
     */
    static void  AddRecord(const quint32 id, const Valentina::Tools &toolType, VPattern *doc);
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
    void                    ChoosedTool(quint32 id, Valentina::Scenes type);
    /**
     * @brief FullUpdateTree emit if need reparse pattern file.
     */
    void                    FullUpdateTree();
protected:
    /**
     * @brief doc dom document container
     */
    VPattern                *doc;
    /**
     * @brief id object id.
     */
    const quint32            id;
    /**
     * @brief baseColor base color for tool.
     */
    Qt::GlobalColor         baseColor;
    /**
     * @brief currentColor current tool color.
     */
    Qt::GlobalColor         currentColor;
    /**
     * @brief typeLine line type.
     */
    QString                 typeLine;
    /**
     * @brief AddToFile add tag with informations about tool into file.
     */
    virtual void            AddToFile()=0;
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void            RefreshDataInFile()=0;
    /**
     * @brief getData return pointer to data container.
     * @return container.
     */
    const VContainer        *getData() const;
    /**
     * @brief RemoveReferens decrement value of reference.
     */
    virtual void            RemoveReferens(){}
    /**
     * @brief RemoveAllChild remove all child from file.
     * @param domElement tag in xml tree.
     */
    void                    RemoveAllChild(QDomElement &domElement);
    /**
     * @brief DeleteTool full delete object form scene and file.
     * @param tool tool
     */
    virtual void            DeleteTool(QGraphicsItem *tool);
    /**
     * @brief LineStyle return pen style for current line style.
     * @return pen style.
     */
    Qt::PenStyle            LineStyle();
private:
    Q_DISABLE_COPY(VAbstractTool)
};

inline quint32 VAbstractTool::getId() const
{
    return id;
}

inline const VContainer *VAbstractTool::getData() const
{
    return &data;
}
#endif // VABSTRACTTOOL_H
