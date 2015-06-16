/************************************************************************
 **
 **  @file   vabstractpattern.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef VABSTRACTPATTERN_H
#define VABSTRACTPATTERN_H

#include "vdomdocument.h"
#include "vtoolrecord.h"

#include <QObject>

enum class Document : char { LiteParse, LitePPParse, FullParse };

class VDataTool;

class VAbstractPattern : public QObject, public VDomDocument
{
    Q_OBJECT
public:
    VAbstractPattern(QObject *parent = nullptr);
    virtual ~VAbstractPattern();

    virtual void   CreateEmptyFile(const QString &tablePath)=0;

    void           ChangeActivPP(const QString& name, const Document &parse = Document::FullParse);
    QString        GetNameActivPP() const;
    bool           CheckExistNamePP(const QString& name) const;
    int            CountPP() const;
    QDomElement    GetPPElement(const QString &name);
    bool           ChangeNamePP(const QString& oldName, const QString &newName);
    bool           appendPP(const QString& name);

    bool           GetActivDrawElement(QDomElement &element) const;
    bool           GetActivNodeElement(const QString& name, QDomElement& element) const;

    quint32        getCursor() const;
    void           setCursor(const quint32 &value);

    virtual void   IncrementReferens(quint32 id) const=0;
    virtual void   DecrementReferens(quint32 id) const=0;

    QHash<quint32, VDataTool *> *getTools();
    VDataTool     *getTool(const quint32 &id);
    void           AddTool(const quint32 &id, VDataTool *tool);

    QVector<VToolRecord> *getHistory();
    QVector<VToolRecord> getLocalHistory() const;

    QString        MPath() const;
    void           SetPath(const QString &path);
    Unit           MUnit() const;
    MeasurementsType MType() const;

    quint32        SiblingNodeId(const quint32 &nodeId) const;

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

signals:
    /**
     * @brief ChangedActivDraw change active pattern peace.
     * @param newName new pattern peace name.
     */
    void           ChangedActivPP(const QString &newName);

    /**
     * @brief ChangedCursor change cursor position.
     * @param id tool id.
     */
    void           ChangedCursor(quint32 id);

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
    void           ClearMainWindow();
    void           UndoCommand();
    void           SetEnabledGUI(bool enabled);
    void           CheckLayout();
    void           SetCurrentPP(const QString &patterPiece);

protected:
    /** @brief nameActivDraw name current pattern peace. */
    QString        nameActivPP;

    /** @brief cursor cursor keep id tool after which we will add new tool in file. */
    quint32        cursor;

    /** @brief tools list with pointer on tools. */
    QHash<quint32, VDataTool*> tools;

    /** @brief history history records. */
    QVector<VToolRecord> history;

    void           ToolExists(const quint32 &id) const;

    void           SetActivPP(const QString& name);

private:
    Q_DISABLE_COPY(VAbstractPattern)
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getTools return list of tools pointers.
 * @return list.
 */
inline QHash<quint32, VDataTool *> *VAbstractPattern::getTools()
{
    return &tools;
}

#endif // VABSTRACTPATTERN_H
