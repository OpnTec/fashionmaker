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

enum class LabelType : char {NewPatternPiece, NewLabel};
enum class CrossCirclesPoint : char {FirstPoint = 1, SecondPoint = 2};

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
    void           UpdateToolData(const quint32 &id, VContainer *data);

    virtual void   IncrementReferens(quint32 id) const;
    virtual void   DecrementReferens(quint32 id) const;

    quint32        SPointActiveDraw();

    virtual bool   SaveDocument(const QString &fileName, QString &error) const;
    QStringList    getPatternPieces() const;
    QRectF         ActiveDrawBoundingRect() const;

    QMap<GHeights, bool> GetGradationHeights() const;
    void                 SetGradationHeights(const QMap<GHeights, bool> &options);

    QMap<GSizes, bool>   GetGradationSizes() const;
    void                 SetGradationSizes(const QMap<GSizes, bool> &options);

    QString              GetAuthor() const;
    void                 SetAuthor(const QString &text);

    QString              GetDescription() const;
    void                 SetDescription(const QString &text);

    QString              GetNotes() const;
    void                 SetNotes(const QString &text);

    QString              GetVersion() const;
    void                 SetVersion();

    QString              GenerateLabel(const LabelType &type)const;

public slots:
    void           LiteParseTree(const Document &parse);
    void           haveLiteChange();
    void           ShowHistoryTool(quint32 id, bool enable);
    void           NeedFullParsing();
    void           ClearScene();
protected:
    virtual void   customEvent(QEvent * event);
private:
    Q_DISABLE_COPY(VPattern)

    /** @brief data container with data. */
    VContainer     *data;

    /** @brief patternPieces list of patern pieces names for combobox*/
    QStringList    patternPieces;

    /** @brief mode current draw mode. */
    Draw    *mode;

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
    void           CheckTagExists(const QString &tag);
    QString        GetLabelBase(unsigned int index)const;
};

//---------------------------------------------------------------------------------------------------------------------
inline QStringList VPattern::getPatternPieces() const
{
    return patternPieces;
}

#endif // VPATTERN_H
