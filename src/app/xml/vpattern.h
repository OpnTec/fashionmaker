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

#ifndef VPATTERN_H
#define VPATTERN_H

#include "vdomdocument.h"
#include "../widgets/vmaingraphicsscene.h"
#include "../tools/vdatatool.h"
#include "vtoolrecord.h"
#include <QComboBox>

namespace Document
{
    /**
     * @brief The Document enum parse types.
     */
    enum Document { LiteParse, FullParse};
    Q_DECLARE_FLAGS(Documents, Document)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(Document::Documents)

/*
   VAL_VERSION is (major << 16) + (minor << 8) + patch.
*/
// version without patch part
#define VAL_MIN_VERSION 0x000100
// max support version of format
#define VAL_VERSION 0x000100

#define VAL_STR_VERSION "0.1.0"

/**
 * @brief The VPattern class working with pattern file.
 */
class VPattern : public QObject, public VDomDocument
{
    Q_OBJECT
public:
    VPattern(VContainer *data, Valentina::Draws *mode, VMainGraphicsScene *sceneDraw, VMainGraphicsScene *sceneDetail,
             QObject *parent = nullptr);
    void           CreateEmptyFile(const QString &tablePath);
    void           ChangeActivDraw(const QString& name, const Document::Documents &parse = Document::FullParse);
    QString        GetNameActivDraw() const;
    bool           GetActivDrawElement(QDomElement &element) const;
    bool           appendDraw(const QString& name);
    bool           SetNameDraw(const QString& name);
    void           Parse(const Document::Documents &parse, VMainGraphicsScene *sceneDraw,
                         VMainGraphicsScene *sceneDetail);
    QHash<quint32, VDataTool*>* getTools();
    VDataTool*     getTool(const quint32 &id);
    QVector<VToolRecord> *getHistory();
    quint32        getCursor() const;
    void           setCursor(const quint32 &value);
    void           setCurrentData();
    void           AddTool(const quint32 &id, VDataTool *tool);
    void           UpdateToolData(const quint32 &id, VContainer *data);
    void           IncrementReferens(quint32 id) const;
    void           DecrementReferens(quint32 id) const;
    void           TestUniqueId() const;
    quint32        SPointActiveDraw();
    bool           isPatternModified() const;
    void           setPatternModified(bool value);
    bool           GetActivNodeElement(const QString& name, QDomElement& element) const;
    QString        MPath() const;
    void           SetPath(const QString &path);
    Valentina::Units MUnit() const;
    Pattern::Measurements MType() const;
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
    static const QString AttrName;
    static const QString AttrType;
    static const QString AttrPath;
    static const QString IncrementName;
    static const QString IncrementBase;
    static const QString IncrementKsize;
    static const QString IncrementKgrowth;
    static const QString IncrementDescription;
    virtual bool   SaveDocument(const QString &fileName);
    QStringList    getPatternPieces() const;
signals:
    /**
     * @brief ChangedActivDraw change active pattern peace.
     * @param newName new pattern peace name.
     */
    void           ChangedActivDraw(const QString &newName);
    /**
     * @brief ChangedNameDraw save new name active pattern peace.
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
    void           patternChanged();
    /**
     * @brief ShowTool highlight tool.
     * @param id tool id.
     * @param color highlight color.
     * @param enable enable or disable highlight.
     */
    void           ShowTool(quint32 id, Qt::GlobalColor color, bool enable);
    /**
     * @brief ChangedCursor change cursor position.
     * @param id tool id.
     */
    void           ChangedCursor(quint32 id);
    void           ClearMainWindow();
public slots:
    void           LiteParseTree();
    void           haveLiteChange();
    void           ShowHistoryTool(quint32 id, Qt::GlobalColor color, bool enable);
private:
    Q_DISABLE_COPY(VPattern)

    /** @brief nameActivDraw name current pattern peace. */
    QString        nameActivDraw;

    /** @brief tools list with pointer on tools. */
    QHash<quint32, VDataTool*> tools;

    /** @brief history history records. */
    QVector<VToolRecord> history;

    /** @brief cursor cursor keep id tool after which we will add new tool in file. */
    quint32        cursor;

    QStringList    patternPieces;

    /** @brief mode current draw mode. */
    Valentina::Draws    *mode;

    /** @brief fileModified true if exist change in file. */
    bool           patternModified;

    VMainGraphicsScene *sceneDraw;
    VMainGraphicsScene *sceneDetail;

    bool           CheckNameDraw(const QString& name) const;
    void           SetActivDraw(const QString& name);
    void           ParseDrawElement(VMainGraphicsScene  *sceneDraw, VMainGraphicsScene *sceneDetail,
                                    const QDomNode& node, const Document::Documents &parse);
    void           ParseDrawMode(VMainGraphicsScene  *sceneDraw, VMainGraphicsScene  *sceneDetail,
                                 const QDomNode& node, const Document::Documents &parse, const Valentina::Draws &mode);
    void           ParseDetailElement(VMainGraphicsScene  *sceneDetail, const QDomElement &domElement,
                                      const Document::Documents &parse);
    void           ParseDetails(VMainGraphicsScene  *sceneDetail, const QDomElement &domElement,
                                const Document::Documents &parse);
    void           ParsePointElement(VMainGraphicsScene *scene, QDomElement &domElement,
                                     const Document::Documents &parse, const QString &type);
    void           ParseLineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                    const Document::Documents &parse);
    void           ParseSplineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                      const Document::Documents &parse, const QString& type);
    void           ParseArcElement(VMainGraphicsScene *scene, QDomElement &domElement,
                                   const Document::Documents &parse, const QString& type);
    void           ParseToolsElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                     const Document::Documents &parse, const QString& type);
    void           ParseIncrementsElement(const QDomNode& node);
    quint32        GetParametrId(const QDomElement& domElement) const;
    void           CollectId(const QDomElement &node, QVector<quint32> &vector)const;
    void           PrepareForParse(const Document::Documents &parse, VMainGraphicsScene *sceneDraw,
                                   VMainGraphicsScene *sceneDetail);
    void           UpdateMeasurements();
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetNameActivDraw return current pattern peace name.
 * @return pattern peace name.
 */
inline QString VPattern::GetNameActivDraw() const
{
    return nameActivDraw;
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
inline bool VPattern::isPatternModified() const
{
    return patternModified;
}

//---------------------------------------------------------------------------------------------------------------------
inline void VPattern::setPatternModified(bool value)
{
    patternModified = value;
}

//---------------------------------------------------------------------------------------------------------------------
inline QStringList VPattern::getPatternPieces() const
{
    return patternPieces;
}

#endif // VPATTERN_H
