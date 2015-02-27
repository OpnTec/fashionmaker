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

#include "../libs/ifc/xml/vdomdocument.h"
#include "vtoolrecord.h"
#include "../container/vcontainer.h"

class VDataTool;
class VMainGraphicsScene;

enum class Document : char { LiteParse, LitePPParse, FullParse };
enum class LabelType : char {NewPatternPiece, NewLabel};

/**
 * @brief The VPattern class working with pattern file.
 */
class VPattern : public QObject, public VDomDocument
{
    Q_OBJECT
public:
    VPattern(VContainer *data, Draw *mode, VMainGraphicsScene *sceneDraw, VMainGraphicsScene *sceneDetail,
             QObject *parent = nullptr);

    void           CreateEmptyFile(const QString &tablePath);
    void           ChangeActivPP(const QString& name, const Document &parse = Document::FullParse);
    QString        GetNameActivPP() const;
    bool           GetActivDrawElement(QDomElement &element) const;
    bool           appendPP(const QString& name);
    bool           ChangeNamePP(const QString& oldName, const QString &newName);
    QDomElement    GetPPElement(const QString &name);
    bool           CheckExistNamePP(const QString& name) const;
    int            CountPP() const;

    void           Parse(const Document &parse);
    QHash<quint32, VDataTool*>* getTools();
    VDataTool*     getTool(const quint32 &id);
    QVector<VToolRecord> *getHistory();
    QVector<VToolRecord> getLocalHistory() const;
    quint32        getCursor() const;
    void           setCursor(const quint32 &value);
    void           setCurrentData();
    void           AddTool(const quint32 &id, VDataTool *tool);
    void           UpdateToolData(const quint32 &id, VContainer *data);
    void           IncrementReferens(quint32 id) const;
    void           DecrementReferens(quint32 id) const;
    quint32        SPointActiveDraw();
    bool           isPatternModified() const;
    void           setPatternModified(bool value);
    bool           GetActivNodeElement(const QString& name, QDomElement& element) const;
    QString        MPath() const;
    void           SetPath(const QString &path);
    Unit           MUnit() const;
    MeasurementsType MType() const;
    static const QString TagPattern;
    static const QString TagCalculation;
    static const QString TagModeling;
    static const QString TagDetails;
    static const QString TagAuthor;
    static const QString TagDescription;
    static const QString TagNotes;
    static const QString TagMeasurements;
    static const QString TagIncrements;
    static const QString TagIncrement;
    static const QString TagDraw;
    static const QString TagPoint;
    static const QString TagLine;
    static const QString TagSpline;
    static const QString TagArc;
    static const QString TagTools;
    static const QString TagGradation;
    static const QString TagHeights;
    static const QString TagSizes;

    static const QString AttrName;
    static const QString AttrType;
    static const QString AttrPath;

    static const QString AttrAll;

    static const QString AttrH92;
    static const QString AttrH98;
    static const QString AttrH104;
    static const QString AttrH110;
    static const QString AttrH116;
    static const QString AttrH122;
    static const QString AttrH128;
    static const QString AttrH134;
    static const QString AttrH140;
    static const QString AttrH146;
    static const QString AttrH152;
    static const QString AttrH158;
    static const QString AttrH164;
    static const QString AttrH170;
    static const QString AttrH176;
    static const QString AttrH182;
    static const QString AttrH188;
    static const QString AttrH194;

    static const QString AttrS22;
    static const QString AttrS24;
    static const QString AttrS26;
    static const QString AttrS28;
    static const QString AttrS30;
    static const QString AttrS32;
    static const QString AttrS34;
    static const QString AttrS36;
    static const QString AttrS38;
    static const QString AttrS40;
    static const QString AttrS42;
    static const QString AttrS44;
    static const QString AttrS46;
    static const QString AttrS48;
    static const QString AttrS50;
    static const QString AttrS52;
    static const QString AttrS54;
    static const QString AttrS56;

    static const QString IncrementName;
    static const QString IncrementBase;
    static const QString IncrementKsize;
    static const QString IncrementKgrowth;
    static const QString IncrementDescription;

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

    quint32              SiblingNodeId(const quint32 &nodeId) const;
signals:
    /**
     * @brief ChangedActivDraw change active pattern peace.
     * @param newName new pattern peace name.
     */
    void           ChangedActivPP(const QString &newName);
    /**
     * @brief ChangedNameDraw save new name pattern peace.
     * @param oldName old name.
     * @param newName new name.
     */
    void           ChangedNameDraw(const QString &oldName, const QString &newName);
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    void           FullUpdateFromFile();
    /**
     * @brief patternChanged emit if we have unsaved change.
     */
    void           patternChanged(bool saved);
    /**
     * @brief ShowTool highlight tool.
     * @param id tool id.
     * @param enable enable or disable highlight.
     */
    void           ShowTool(quint32 id, bool enable);
    /**
     * @brief ChangedCursor change cursor position.
     * @param id tool id.
     */
    void           ChangedCursor(quint32 id);
    void           ClearMainWindow();
    void           UndoCommand();
    void           SetEnabledGUI(bool enabled);
    void           CheckLayout();
    void           SetCurrentPP(const QString &patterPiece);
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

    /** @brief nameActivDraw name current pattern peace. */
    QString        nameActivPP;

    /** @brief tools list with pointer on tools. */
    QHash<quint32, VDataTool*> tools;

    /** @brief history history records. */
    QVector<VToolRecord> history;

    /** @brief cursor cursor keep id tool after which we will add new tool in file. */
    quint32        cursor;

    /** @brief patternPieces list of patern pieces names for combobox*/
    QStringList    patternPieces;

    /** @brief mode current draw mode. */
    Draw    *mode;

    VMainGraphicsScene *sceneDraw;
    VMainGraphicsScene *sceneDetail;

    void           SetActivPP(const QString& name);
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
    void           ToolExists(const quint32 &id) const;
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetNameActivPP return current pattern piece name.
 * @return pattern piece name.
 */
inline QString VPattern::GetNameActivPP() const
{
    return nameActivPP;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTools return list of tools pointers.
 * @return list.
 */
inline QHash<quint32, VDataTool *> *VPattern::getTools()
{
    return &tools;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getHistory return list with list of history records.
 * @return list of history records.
 */
inline QVector<VToolRecord> *VPattern::getHistory()
{
    return &history;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getCursor return cursor.
 * @return cursor.
 */
inline quint32 VPattern::getCursor() const
{
    return cursor;
}

//---------------------------------------------------------------------------------------------------------------------
inline QStringList VPattern::getPatternPieces() const
{
    return patternPieces;
}

#endif // VPATTERN_H
