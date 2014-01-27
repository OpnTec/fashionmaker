/************************************************************************
 **
 **  @file   vdomdocument.h
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

#ifndef VDOMDOCUMENT_H
#define VDOMDOCUMENT_H

#include "../container/vcontainer.h"
#include "../widgets/vmaingraphicsscene.h"
#include "../tools/vdatatool.h"
#include "vtoolrecord.h"

#include <QComboBox>
#include <QDomDocument>
#include <QDebug>

namespace Document
{
    /**
     * @brief The Document enum types parse.
     */
    enum Document { LiteParse, FullParse};
    Q_DECLARE_FLAGS(Documents, Document)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(Document::Documents)

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

/**
 * @brief The VDomDocument class working with pattern file.
 */
class VDomDocument : public QObject, public QDomDocument
{
    Q_OBJECT
public:
                   /**
                    * @brief VDomDocument constructor.
                    * @param data container with variables.
                    * @param comboBoxDraws combobox for pattern peaces names.
                    * @param mode draw mode.
                    */
                   VDomDocument(VContainer *data, QComboBox *comboBoxDraws, Draw::Draws *mode, QObject *parent = 0);
                   /**
                    * @brief VDomDocument constructor.
                    * @param name pattern peace name.
                    * @param data container with variables.
                    * @param comboBoxDraws combobox for pattern peaces names.
                    * @param mode draw mode.
                    */
                   VDomDocument(const QString& name, VContainer *data, QComboBox *comboBoxDraws, Draw::Draws *mode,
                                QObject *parent = 0);
                   /**
                    * @brief VDomDocument constructor.
                    * @param doc dom document container type.
                    * @param data container with variables.
                    * @param comboBoxDraws combobox for pattern peaces names.
                    * @param mode draw mode.
                    */
                   VDomDocument(const QDomDocumentType& doctype, VContainer *data, QComboBox *comboBoxDraws,
                                Draw::Draws *mode, QObject *parent = 0);
                   ~VDomDocument(){}
    /**
     * @brief elementById find element by id.
     * @param id value id attribute.
     * @return dom element.
     */
    QDomElement    elementById(const QString& id);
    /**
     * @brief removeAllChilds remove all tag childs.
     * @param element tag.
     */
    void           removeAllChilds(QDomElement &element);
    /**
     * @brief CreateEmptyFile create minimal empty file.
     */
    void           CreateEmptyFile();
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
    inline QString GetNameActivDraw() const {return nameActivDraw;}
    /**
     * @brief GetActivDrawElement return draw tag for current pattern peace.
     * @param element draw tag.
     * @return true if found.
     */
    bool           GetActivDrawElement(QDomElement &element);
    /**
     * @brief GetActivCalculationElement return calculation tag for current pattern peace.
     * @param element calculation tag.
     * @return true if found.
     */
    bool           GetActivCalculationElement(QDomElement &element);
    /**
     * @brief GetActivModelingElement return modeling tag for current pattern peace.
     * @param element modeling tag
     * @return true if found.
     */
    bool           GetActivModelingElement(QDomElement &element);
    /**
     * @brief GetActivDetailsElement return details tag for current pattern peace.
     * @param element details tag.
     * @return true if found.
     */
    bool           GetActivDetailsElement(QDomElement &element);
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
    inline QHash<qint64, VDataTool*>* getTools() {return &tools;}
    /**
     * @brief getTool return tool from tool list.
     * @param id tool id.
     * @return tool.
     */
    VDataTool* getTool(const qint64 &id);
    /**
     * @brief getHistory return list with list of history records.
     * @return list of history records.
     */
    inline QVector<VToolRecord> *getHistory() {return &history;}
    /**
     * @brief getCursor return cursor.
     * @return cursor.
     */
    inline qint64  getCursor() const {return cursor;}
    /**
     * @brief setCursor set cursor.
     * @param value cursor.
     */
    void           setCursor(const qint64 &value);
    /**
     * @brief setCurrentData set current data set.
     */
    void           setCurrentData();
    /**
     * @brief AddTool add tool to list tools.
     * @param id tool id.
     * @param tool tool.
     */
    void           AddTool(const qint64 &id, VDataTool *tool);
    /**
     * @brief UpdateToolData update tool in list tools.
     * @param id tool id.
     * @param data container with variables.
     */
    void           UpdateToolData(const qint64 &id, VContainer *data);
    /**
     * @brief IncrementReferens increment reference parent objects.
     * @param id parent object id.
     */
    void           IncrementReferens(qint64 id) const;
    /**
     * @brief DecrementReferens decrement reference parent objects.
     * @param id parent object id.
     */
    void           DecrementReferens(qint64 id) const;
    /**
     * @brief TestUniqueId test exist unique id in pattern file. Each id must be unique.
     */
    void           TestUniqueId() const;
    /**
     * @brief GetParametrLongLong return long long value of attribute.
     * @param domElement tag in xml tree.
     * @param name attribute name.
     * @return long long value.
     */
    qint64         GetParametrLongLong(const QDomElement& domElement, const QString &name,
                                       const QString &defValue) const;
    /**
     * @brief GetParametrString return string value of attribute.
     * @param domElement tag in xml tree.
     * @param name attribute name.
     * @return attribute value.
     */
    QString        GetParametrString(const QDomElement& domElement, const QString &name, const QString &defValue) const;
    /**
     * @brief GetParametrDouble return double value of attribute.
     * @param domElement tag in xml tree.
     * @param name attribute name.
     * @return double value.
     */
    qreal          GetParametrDouble(const QDomElement& domElement, const QString &name, const QString &defValue) const;
    /**
     * @brief SPointActiveDraw return id base point current pattern peace.
     * @return id base point.
     */
    qint64         SPointActiveDraw();
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
     * @brief haveChange emit if we have unsaved change.
     */
    void           haveChange();
    /**
     * @brief ShowTool highlight tool.
     * @param id tool id.
     * @param color highlight color.
     * @param enable enable or disable highlight.
     */
    void           ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    /**
     * @brief ChangedCursor change cursor position.
     * @param id tool id.
     */
    void           ChangedCursor(qint64 id);
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
    void           ShowHistoryTool(qint64 id, Qt::GlobalColor color, bool enable);
private:
    Q_DISABLE_COPY(VDomDocument)
    /**
     * @brief map use for finding element by id.
     */
    QHash<QString, QDomElement> map;
    /**
     * @brief nameActivDraw name current pattern peace.
     */
    QString        nameActivDraw;
    /**
     * @brief data container with data.
     */
    VContainer     *data;
    /**
     * @brief tools list with pointer on tools.
     */
    QHash<qint64, VDataTool*> tools;
    /**
     * @brief history history records.
     */
    QVector<VToolRecord> history;
    /**
     * @brief cursor cursor keep id tool after which we will add new tool in file.
     */
    qint64         cursor;
    /**
     * @brief comboBoxDraws combobox with pattern peace names.
     */
    QComboBox      *comboBoxDraws;
    /**
     * @brief mode current draw mode.
     */
    Draw::Draws    *mode;
    /**
     * @brief find find element by id.
     * @param node node.
     * @param id id value.
     * @return true if found.
     */
    bool           find(const QDomElement &node, const QString& id);
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
     * @brief GetActivNodeElement find element in current pattern peace by name.
     * @param name name tag.
     * @param element element.
     * @return true if found.
     */
    bool           GetActivNodeElement(const QString& name, QDomElement& element);
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
                                 const QDomNode& node, const Document::Documents &parse, const Draw::Draws &mode);
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
    qint64         GetParametrId(const QDomElement& domElement) const;
    /**
     * @brief CollectId recursive function, try find id attribute in file. Throw exclusion if find not unique.
     * @param node tag in xml tree.
     * @param vector list with ids.
     */
    void           CollectId(const QDomElement &node, QVector<qint64> &vector)const;
};

#ifdef Q_CC_GNU
    #pragma GCC diagnostic pop
#endif

#endif // VDOMDOCUMENT_H
