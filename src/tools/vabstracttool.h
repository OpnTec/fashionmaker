/************************************************************************
 **
 **  @file   vabstracttool.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
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
#include "../xml/vdomdocument.h"

/**
 * @brief The VAbstractTool class
 */
class VAbstractTool: public VDataTool
{
    Q_OBJECT
public:
                            /**
                             * @brief VAbstractTool
                             * @param doc dom document container
                             * @param data
                             * @param id
                             * @param parent
                             */
                            VAbstractTool(VDomDocument *doc, VContainer *data, qint64 id);
    virtual                 ~VAbstractTool() {}
    /**
     * @brief LineIntersectRect
     * @param rec
     * @param line
     * @return
     */
    static QPointF          LineIntersectRect(QRectF rec, QLineF line);
    /**
     * @brief LineIntersectCircle
     * @param center
     * @param radius
     * @param line
     * @param p1
     * @param p2
     * @return
     */
    static qint32           LineIntersectCircle(const QPointF &center, qreal radius, const QLineF &line, QPointF &p1,
                                                QPointF &p2);
    /**
     * @brief ClosestPoint
     * @param line
     * @param p
     * @return
     */
    static QPointF          ClosestPoint(const QLineF &line, const QPointF &p);
    /**
     * @brief addVector
     * @param p
     * @param p1
     * @param p2
     * @param k
     * @return
     */
    static QPointF          addVector (const QPointF &p, const QPointF &p1, const QPointF &p2, qreal k);
    /**
     * @brief getId
     * @return
     */
    inline qint64           getId() const {return id;}
    /**
     * @brief LineCoefficients
     * @param line
     * @param a
     * @param b
     * @param c
     */
    static void             LineCoefficients(const QLineF &line, qreal *a, qreal *b, qreal *c);
    /**
     * @brief AttrId
     */
    static const QString    AttrId;
    /**
     * @brief AttrType
     */
    static const QString    AttrType;
    /**
     * @brief AttrMx
     */
    static const QString    AttrMx;
    /**
     * @brief AttrMy
     */
    static const QString    AttrMy;
    /**
     * @brief AttrName
     */
    static const QString    AttrName;
    /**
     * @brief AttrX
     */
    static const QString    AttrX;
    /**
     * @brief AttrY
     */
    static const QString    AttrY;
    /**
     * @brief AttrTypeLine
     */
    static const QString    AttrTypeLine;
    /**
     * @brief AttrLength
     */
    static const QString    AttrLength;
    /**
     * @brief AttrBasePoint
     */
    static const QString    AttrBasePoint;
    /**
     * @brief AttrFirstPoint
     */
    static const QString    AttrFirstPoint;
    /**
     * @brief AttrSecondPoint
     */
    static const QString    AttrSecondPoint;
    /**
     * @brief AttrThirdPoint
     */
    static const QString    AttrThirdPoint;
    /**
     * @brief AttrCenter
     */
    static const QString    AttrCenter;
    /**
     * @brief AttrRadius
     */
    static const QString    AttrRadius;
    /**
     * @brief AttrAngle
     */
    static const QString    AttrAngle;
    /**
     * @brief AttrAngle1
     */
    static const QString    AttrAngle1;
    /**
     * @brief AttrAngle2
     */
    static const QString    AttrAngle2;
    /**
     * @brief AttrP1Line
     */
    static const QString    AttrP1Line;
    /**
     * @brief AttrP2Line
     */
    static const QString    AttrP2Line;
    /**
     * @brief AttrP1Line1
     */
    static const QString    AttrP1Line1;
    /**
     * @brief AttrP2Line1
     */
    static const QString    AttrP2Line1;
    /**
     * @brief AttrP1Line2
     */
    static const QString    AttrP1Line2;
    /**
     * @brief AttrP2Line2
     */
    static const QString    AttrP2Line2;
    /**
     * @brief AttrPShoulder
     */
    static const QString    AttrPShoulder;
    /**
     * @brief AttrPoint1
     */
    static const QString    AttrPoint1;
    /**
     * @brief AttrPoint4
     */
    static const QString    AttrPoint4;
    /**
     * @brief AttrKAsm1
     */
    static const QString    AttrKAsm1;
    /**
     * @brief AttrKAsm2
     */
    static const QString    AttrKAsm2;
    /**
     * @brief AttrKCurve
     */
    static const QString    AttrKCurve;
    /**
     * @brief AttrPathPoint
     */
    static const QString    AttrPathPoint;
    /**
     * @brief AttrPSpline
     */
    static const QString    AttrPSpline;
    /**
     * @brief AttrAxisP1
     */
    static const QString    AttrAxisP1;
    /**
     * @brief AttrAxisP2
     */
    static const QString    AttrAxisP2;
    /**
     * @brief TypeLineNone
     */
    static const QString    TypeLineNone;
    /**
     * @brief TypeLineLine
     */
    static const QString    TypeLineLine;
    /**
     * @brief AddRecord
     * @param id
     * @param toolType
     * @param doc dom document container
     */
    static void  AddRecord(const qint64 id, const Tool::Tools &toolType, VDomDocument *doc);
public slots:
    /**
     * @brief FullUpdateFromFile
     */
    virtual void            FullUpdateFromFile()=0;
signals:
    /**
     * @brief toolhaveChange
     */
    void                    toolhaveChange();
    /**
     * @brief ChoosedTool
     * @param id
     * @param type
     */
    void                    ChoosedTool(qint64 id, Scene::Scenes type);
    /**
     * @brief FullUpdateTree
     */
    void                    FullUpdateTree();
    /**
     * @brief RemoveTool
     * @param tool
     */
    void                    RemoveTool(QGraphicsItem *tool);
protected:
    /**
     * @brief doc dom document container
     */
    VDomDocument            *doc;
    /**
     * @brief id
     */
    const qint64            id;
    /**
     * @brief baseColor
     */
    const Qt::GlobalColor   baseColor;
    /**
     * @brief currentColor
     */
    Qt::GlobalColor         currentColor;
    /**
     * @brief AddToFile
     */
    virtual void            AddToFile()=0;
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void            RefreshDataInFile()=0;
    /**
     * @brief getData
     * @return
     */
    inline const VContainer *getData() const {return &data;}
    /**
     * @brief RemoveReferens
     */
    virtual void            RemoveReferens(){}
    /**
     * @brief RemoveAllChild
     * @param domElement
     */
    void                    RemoveAllChild(QDomElement &domElement);
    template <typename T>
    /**
     * @brief AddAttribute
     * @param domElement
     * @param name
     * @param value
     */
    void AddAttribute(QDomElement &domElement, const QString &name, const T &value)
    {
        QDomAttr domAttr = doc->createAttribute(name);
        domAttr.setValue(QString().setNum(value));
        domElement.setAttributeNode(domAttr);
    }
private:
    Q_DISABLE_COPY(VAbstractTool)
};

template <>
inline void VAbstractTool::AddAttribute<QString>(QDomElement &domElement, const QString &name, const QString &value)
{
    QDomAttr domAttr = doc->createAttribute(name);
    domAttr.setValue(value);
    domElement.setAttributeNode(domAttr);
}
#endif // VABSTRACTTOOL_H
