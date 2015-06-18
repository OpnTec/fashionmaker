/************************************************************************
 **
 **  @file   vpattern.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 2, 2014
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

#ifndef VPATTERN_H
#define VPATTERN_H

#include "../libs/ifc/xml/vabstractpattern.h"
#include "../libs/ifc/xml/vtoolrecord.h"
#include "../libs/vpatterndb/vcontainer.h"

class VDataTool;
class VMainGraphicsScene;

/**
 * @brief The VPattern class working with pattern file.
 */
class VPattern : public VAbstractPattern
{
    Q_OBJECT
public:
    VPattern(VContainer *data, Draw *mode, VMainGraphicsScene *sceneDraw, VMainGraphicsScene *sceneDetail,
             QObject *parent = nullptr);

    virtual void   CreateEmptyFile(const QString &tablePath);

    void           Parse(const Document &parse);

    void           setCurrentData();
    virtual void   UpdateToolData(const quint32 &id, VContainer *data);

    virtual void   IncrementReferens(quint32 id) const;
    virtual void   DecrementReferens(quint32 id) const;

    quint32        SPointActiveDraw();

    virtual bool   SaveDocument(const QString &fileName, QString &error) const;

    QRectF         ActiveDrawBoundingRect() const;

    QString        GetAuthor() const;
    void           SetAuthor(const QString &text);

    virtual QString GenerateLabel(const LabelType &type)const;

public slots:
    void           LiteParseTree(const Document &parse);

protected:
    virtual void   customEvent(QEvent * event);

private:
    Q_DISABLE_COPY(VPattern)

    /** @brief data container with data. */
    VContainer     *data;

    /** @brief mode current draw mode. */
    Draw           *mode;

    VMainGraphicsScene *sceneDraw;
    VMainGraphicsScene *sceneDetail;

    void           ParseDrawElement(const QDomNode& node, const Document &parse);
    void           ParseDrawMode(const QDomNode& node, const Document &parse, const Draw &mode);
    void           ParseDetailElement(const QDomElement &domElement,
                                      const Document &parse);
    void           ParseDetails(const QDomElement &domElement, const Document &parse);
    void           ParsePointElement(VMainGraphicsScene *scene, QDomElement &domElement,
                                     const Document &parse, const QString &type);
    void           ParseLineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                    const Document &parse);
    void           ParseSplineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                      const Document &parse, const QString& type);
    void           ParseArcElement(VMainGraphicsScene *scene, QDomElement &domElement,
                                   const Document &parse, const QString& type);
    void           ParseToolsElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                     const Document &parse, const QString& type);
    void           ParseIncrementsElement(const QDomNode& node);
    void           PrepareForParse(const Document &parse);
    void           UpdateMeasurements();
    void           ToolsCommonAttributes(const QDomElement &domElement, quint32 &id);
    void           PointsCommonAttributes(const QDomElement &domElement, quint32 &id, QString &name, qreal &mx,
                                          qreal &my, QString &typeLine, QString &lineColor);
    void           PointsCommonAttributes(const QDomElement &domElement, quint32 &id, QString &name, qreal &mx,
                                          qreal &my);
    void           PointsCommonAttributes(const QDomElement &domElement, quint32 &id, qreal &mx, qreal &my);
    void           SplinesCommonAttributes(const QDomElement &domElement, quint32 &id, quint32 &idObject,
                                           quint32 &idTool);
    template <typename T>
    QRectF ToolBoundingRect(const QRectF &rec, const quint32 &id) const;
    void           ParseCurrentPP();
    QString        GetLabelBase(unsigned int index)const;
};

#endif // VPATTERN_H
