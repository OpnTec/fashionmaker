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

/**
 * @brief The VPattern class working with pattern file.
 */
class VPattern : public QObject, public VDomDocument
{
    Q_OBJECT
public:
    VPattern(VContainer *data, QComboBox *comboBoxDraws, Valentina::Draws *mode, QObject *parent = nullptr);
    /**
     * @brief CreateEmptyFile create minimal empty file.
     * @param tablePath
     */
    void           CreateEmptyFile(const QString &tablePath);
    /**
     * @brief ChangeActivDraw set new pattern peace name.
     * @param name new name.
     * @param parse parser file mode.
     */
    void           ChangeActivDraw(const QString& name, const Document::Documents &parse = Document::FullParse);
    /**
     * @brief GetNameActivDraw return current pattern peace name.
     * @return pattern peace name.
     */
    QString        GetNameActivDraw() const;
    /**
     * @brief GetActivDrawElement return draw tag for current pattern peace.
     * @param element draw tag.
     * @return true if found.
     */
    bool           GetActivDrawElement(QDomElement &element);
    /**
     * @brief appendDraw add new pattern peace structure to the file.
     * @param name pattern peace name.
     * @return true if success.
     */
    bool           appendDraw(const QString& name);
    /**
     * @brief SetNameDraw change current pattern peace.
     * @param name pattern peace name.
     * @return true if success.
     */
    bool           SetNameDraw(const QString& name);
    /**
     * @brief Parse parse file.
     * @param parse parser file mode.
     * @param sceneDraw pointer to draw scene.
     * @param sceneDetail pointer to details scene.
     */
    void           Parse(const Document::Documents &parse, VMainGraphicsScene *sceneDraw,
                         VMainGraphicsScene *sceneDetail);
    /**
     * @brief getTools return list of tools pointers.
     * @return list.
     */
    QHash<quint32, VDataTool*>* getTools();
    /**
     * @brief getTool return tool from tool list.
     * @param id tool id.
     * @return tool.
     */
    VDataTool*     getTool(const quint32 &id);
    /**
     * @brief getHistory return list with list of history records.
     * @return list of history records.
     */
    QVector<VToolRecord> *getHistory();
    /**
     * @brief getCursor return cursor.
     * @return cursor.
     */
    quint32        getCursor() const;
    /**
     * @brief setCursor set cursor.
     * @param value cursor.
     */
    void           setCursor(const quint32 &value);
    /**
     * @brief setCurrentData set current data set.
     */
    void           setCurrentData();
    /**
     * @brief AddTool add tool to list tools.
     * @param id tool id.
     * @param tool tool.
     */
    void           AddTool(const quint32 &id, VDataTool *tool);
    /**
     * @brief UpdateToolData update tool in list tools.
     * @param id tool id.
     * @param data container with variables.
     */
    void           UpdateToolData(const quint32 &id, VContainer *data);
    /**
     * @brief IncrementReferens increment reference parent objects.
     * @param id parent object id.
     */
    void           IncrementReferens(quint32 id) const;
    /**
     * @brief DecrementReferens decrement reference parent objects.
     * @param id parent object id.
     */
    void           DecrementReferens(quint32 id) const;
    /**
     * @brief TestUniqueId test exist unique id in pattern file. Each id must be unique.
     */
    void           TestUniqueId() const;
    /**
     * @brief SPointActiveDraw return id base point current pattern peace.
     * @return id base point.
     */
    quint32        SPointActiveDraw();
    bool           isPatternModified() const;
    void           setPatternModified(bool value);
    /**
     * @brief GetActivNodeElement find element in current pattern piece by name.
     * @param name name tag.
     * @param element element.
     * @return true if found.
     */
    bool           GetActivNodeElement(const QString& name, QDomElement& element);
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
public slots:
    /**
     * @brief FullUpdateTree lite parse file.
     */
    void           FullUpdateTree();
    /**
     * @brief haveLiteChange we have unsaved change.
     */
    void           haveLiteChange();
    /**
     * @brief ShowHistoryTool hightlight tool.
     * @param id tool id.
     * @param color hightlight color.
     * @param enable enable or diasable hightlight.
     */
    void           ShowHistoryTool(quint32 id, Qt::GlobalColor color, bool enable);
private:
    Q_DISABLE_COPY(VPattern)
    /**
     * @brief nameActivDraw name current pattern peace.
     */
    QString        nameActivDraw;
    /**
     * @brief tools list with pointer on tools.
     */
    QHash<quint32, VDataTool*> tools;
    /**
     * @brief history history records.
     */
    QVector<VToolRecord> history;
    /**
     * @brief cursor cursor keep id tool after which we will add new tool in file.
     */
    quint32         cursor;
    QComboBox      *comboBoxDraws;
    /**
     * @brief mode current draw mode.
     */
    Valentina::Draws    *mode;
    /**
     * @brief fileModified true if exist change in file.
     */
    bool            patternModified;
    /**
     * @brief CheckNameDraw check if exist pattern peace with this name.
     * @param name pattern peace name.
     * @return true if exist.
     */
    bool           CheckNameDraw(const QString& name) const;
    /**
     * @brief SetActivDraw set current pattern peace.
     * @param name pattern peace name.
     */
    void           SetActivDraw(const QString& name);

    /**
     * @brief ParseDrawElement parse draw tag.
     * @param sceneDraw draw scene.
     * @param sceneDetail details scene.
     * @param node node.
     * @param parse parser file mode.
     */
    void           ParseDrawElement(VMainGraphicsScene  *sceneDraw, VMainGraphicsScene *sceneDetail,
                                    const QDomNode& node, const Document::Documents &parse);
    /**
     * @brief ParseDrawMode parse draw tag with draw mode.
     * @param sceneDraw draw scene.
     * @param sceneDetail details scene.
     * @param node node.
     * @param parse parser file mode.
     * @param mode draw mode.
     */
    void           ParseDrawMode(VMainGraphicsScene  *sceneDraw, VMainGraphicsScene  *sceneDetail,
                                 const QDomNode& node, const Document::Documents &parse, const Valentina::Draws &mode);
    /**
     * @brief ParseDetailElement parse detail tag.
     * @param sceneDetail detail scene.
     * @param domElement tag in xml tree.
     * @param parse parser file mode.
     */
    void           ParseDetailElement(VMainGraphicsScene  *sceneDetail, const QDomElement &domElement,
                                      const Document::Documents &parse);
    /**
     * @brief ParseDetails parse details tag.
     * @param sceneDetail detail scene.
     * @param domElement tag in xml tree.
     * @param parse parser file mode.
     */
    void           ParseDetails(VMainGraphicsScene  *sceneDetail, const QDomElement &domElement,
                                const Document::Documents &parse);
    /**
     * @brief ParsePointElement parse point tag.
     * @param scene scene.
     * @param domElement tag in xml tree.
     * @param parse parser file mode.
     * @param type type of point.
     * @param mode draw mode.
     */
    void           ParsePointElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                     const Document::Documents &parse, const QString &type);
    /**
     * @brief ParseLineElement parse line tag.
     * @param scene scene.
     * @param domElement tag in xml tree.
     * @param parse parser file mode.
     * @param mode draw mode.
     */
    void           ParseLineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                    const Document::Documents &parse);
    /**
     * @brief ParseSplineElement parse spline tag.
     * @param scene scene.
     * @param domElement tag in xml tree.
     * @param parse parser file mode.
     * @param type type of spline.
     * @param mode draw mode.
     */
    void           ParseSplineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                      const Document::Documents &parse, const QString& type);
    /**
     * @brief ParseArcElement parse arc tag.
     * @param scene scene.
     * @param domElement tag in xml tree.
     * @param parse parser file mode.
     * @param type type of spline.
     * @param mode draw mode.
     */
    void           ParseArcElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                   const Document::Documents &parse, const QString& type);
    /**
     * @brief ParseToolsElement parse tools tag.
     * @param scene scene.
     * @param domElement tag in xml tree.
     * @param parse parser file mode.
     * @param type type of spline.
     */
    void           ParseToolsElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                     const Document::Documents &parse, const QString& type);
    /**
     * @brief ParseIncrementsElement parse increments tag.
     * @param node tag in xml tree.
     */
    void           ParseIncrementsElement(const QDomNode& node);
    /**
     * @brief GetParametrId return value id attribute.
     * @param domElement tag in xml tree.
     * @return id value.
     */
    quint32         GetParametrId(const QDomElement& domElement) const;
    /**
     * @brief CollectId recursive function, try find id attribute in file. Throw exclusion if find not unique.
     * @param node tag in xml tree.
     * @param vector list with ids.
     */
    void           CollectId(const QDomElement &node, QVector<quint32> &vector)const;
    void           PrepareForParse(const Document::Documents &parse, VMainGraphicsScene *sceneDraw,
                                   VMainGraphicsScene *sceneDetail);
    void           UpdateMeasurements();
};

inline QString VPattern::GetNameActivDraw() const
{
    return nameActivDraw;
}

inline QHash<quint32, VDataTool *> *VPattern::getTools()
{
    return &tools;
}

inline QVector<VToolRecord> *VPattern::getHistory()
{
    return &history;
}

inline quint32 VPattern::getCursor() const
{
    return cursor;
}

inline bool VPattern::isPatternModified() const
{
    return patternModified;
}

inline void VPattern::setPatternModified(bool value)
{
    patternModified = value;
}

#endif // VPATTERN_H
