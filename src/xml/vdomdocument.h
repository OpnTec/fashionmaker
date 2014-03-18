/************************************************************************
 **
 **  @file   vdomdocument.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program that allows creating and modelling patterns of clothing.
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
     * @brief Flags to determine the parsing mode in VDomDocument::Parse() and 
     * related functions.
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
 * @brief The VDomDocument class represents a Valentina document (.val file).
 * 
 * A Valentina document describes the construction of a sewing pattern. The 
 * information is stored in XML format. By parsing a VDomDocument, the contained
 * pattern is rendered to a Valentina graphics scene (VMainGraphicsScene).
 *
 * A sewing pattern consists of zero or more increments and one 
 * or more pattern pieces.
 *
 * An increment is an auxiliary variable that is calculated from regular measurement
 * variables (that belong to the standard measurements table). Increments are used to
 * create a graduation schema for the sewing pattern.
 *
 * A pattern piece contains 
 * 1) auxiliary pattern construction elements (calculation),
 * 2) pattern construction elements (modeling), and 
 * 3) special markers, e.g. seam allowances (details).
 * Of these, 2) and 3) are visible in the final pattern (draw mode 'Modeling'),
 * 1) is only displayed when editing (draw mode 'Calculation') the pattern.
 */
class VDomDocument : public QObject, public QDomDocument
{
    Q_OBJECT
public:
    /**
    * @param data container with variables
    * @param comboBoxDraws pointer to the ComboBox that will hold the pattern piece names
    * @param mode draw mode
    * @param parent
    */
    VDomDocument(VContainer *data, QComboBox *comboBoxDraws, Draw::Draws *mode, QObject *parent = 0);
    /**
    * @param name pattern piece name
    * @param data container with variables
    * @param comboBoxDraws pointer to the ComboBox that will hold the pattern piece names
    * @param mode draw mode
    * @param parent
    */
    VDomDocument(const QString& name, VContainer *data, QComboBox *comboBoxDraws, Draw::Draws *mode,
                QObject *parent = 0);
    /**
    * @param doctype dom document container type
    * @param data container with variables
    * @param comboBoxDraws pointer to the ComboBox that will hold the pattern piece names
    * @param mode draw mode
    * @param parent
    */
    VDomDocument(const QDomDocumentType& doctype, VContainer *data, QComboBox *comboBoxDraws,
                    Draw::Draws *mode, QObject *parent = 0);
    ~VDomDocument(){}
    /**
     * @brief Finds an element by id.
     * @param id value id attribute.
     * @return dom element.
     */
    QDomElement    elementById(const QString& id);
    /**
     * @brief Removes all children of a given element tag. RENAME: removeAllChildren!
     * @param element tag
     */
    void           removeAllChilds(QDomElement &element);
    /**
     * @brief Create a minimal empty file.
     */
    void           CreateEmptyFile();
    /**
     * @brief Select pattern piece based on name. RENAME: ChangeActiveDraw? ChangeActivePatternPiece?
     * @param name pattern piece name
     * @param parse parsing mode
     */
    void           ChangeActivDraw(const QString& name, const Document::Documents &parse = Document::FullParse);
    /**
     * @brief Get selected pattern piece name. RENAME: GetNameOfActiveDraw? GetNameOfActivePatternPiece?
     * @return name of the active pattern piece
     */
    inline QString GetNameActivDraw() const {return nameActivDraw;}
    /**
     * @brief Finds the draw element of the selected pattern piece and returns it by reference. RENAME: GetActiveDrawElement?
     * @param element the returned draw element -- not used as input
     * @return true if found
     */
    bool           GetActivDrawElement(QDomElement &element);
    /**
     * @brief Finds the calculation element for the selected pattern piece and returns it by reference. RENAME: GetActiveCalculationElement?
     * @param element the returned calculation element -- not used as input
     * @return true if found
     */
    bool           GetActivCalculationElement(QDomElement &element);
    /**
     * @brief Finds the modeling element for the selected pattern piece and returns it by reference. RENAME: GetActiveModelingElement?
     * @param element the returned modeling element -- not used as input
     * @return true if found
     */
    bool           GetActivModelingElement(QDomElement &element);
    /**
     * @brief Finds the details element for the selected pattern piece and returns it by reference. RENAME: GetActiveDetailsElement?
     * @param element the returned details element -- not used as input
     * @return true if found
     */
    bool           GetActivDetailsElement(QDomElement &element);
    /**
     * @brief Adds a new pattern piece to the document. NOTE: inconsistent capitalization
     * @param name pattern piece name.
     * @return true if successful
     */
    bool           appendDraw(const QString& name);
    /**
     * @brief Sets the name of the selected pattern piece. RENAME: SetNameOfActiveDraw!
     * @param name pattern piece name.
     * @return true if successful
     */
    bool           SetNameDraw(const QString& name);
    /**
     * @brief Parse the document, resulting in a rendering of the pattern in the graphics scenes.
     * @param parse parsing mode
     * @param sceneDraw pointer to draw scene
     * @param sceneDetail pointer to details scene
     */
    void           Parse(const Document::Documents &parse, VMainGraphicsScene *sceneDraw,
                         VMainGraphicsScene *sceneDetail);
    /**
     * @brief Return the list of tool pointers. When we create lines, points, curves on scene we 
     * use tools. This structure keeps the  pointers to those tools. 
     * @return list of tool pointers
     */
    inline QHash<qint64, VDataTool*>* getTools() {return &tools;} // NOTE: inconsistent capitalization
    /**
     * @brief Get a tool from the tool list by id.
     * @param id tool id
     * @return tool
     */
    VDataTool* getTool(const qint64 &id); // NOTE: inconsistent capitalization
    /**
     * @brief Return a list of history record lists. The history keeps track of the tools the user used 
     * to create the pattern piece.
     * @return list of history record lists
     */
    inline QVector<VToolRecord> *getHistory() {return &history;} // NOTE: inconsistent capitalization
    /**
     * @brief Returns the cursor. Here, the cursor is the id of the tool after which a
     * new object will be added.
     * @return cursor
     */
    inline qint64  getCursor() const {return cursor;} // NOTE: inconsistent capitalization
    /**
     * @brief Set the cursor.
     * @param value cursor
     */
    void           setCursor(const qint64 &value); // NOTE: inconsistent capitalization
    /**
     * @brief Set current data set.
     */
    void           setCurrentData(); // NOTE: inconsistent capitalization
    /**
     * @brief Add the given tool to the tools list.
     * @param id tool id.
     * @param tool tool.
     */
    void           AddTool(const qint64 &id, VDataTool *tool);
    /**
     * @brief Updates a given tool in the tools list.
     * @param id id of the tool to update
     * @param data container with variables
     */
    void           UpdateToolData(const qint64 &id, VContainer *data);
    /**
     * @brief Increment reference parent objects. RENAME: IncrementReference?
     * @param id parent object id
     */
    void           IncrementReferens(qint64 id) const;
    /**
     * @brief Decrement reference parent objects. RENAME: like above
     * @param id parent object id
     */
    void           DecrementReferens(qint64 id) const;
    /**
     * @brief Throws an exception if there exists a duplicate id in the pattern file.
     */
    void           TestUniqueId() const;
    /**
     * @brief Returns the long long value of the given attribute. RENAME: GetParameterLongLong?
     * @param domElement tag in xml tree
     * @param name attribute name
     * @return long long value
     */
    qint64         GetParametrLongLong(const QDomElement& domElement, const QString &name,
                                       const QString &defValue) const;
    /**
     * @brief Returns the string value of the given attribute. RENAME: see above
     * @param domElement tag in xml tree
     * @param name attribute name
     * @return attribute value
     */
    QString        GetParametrString(const QDomElement& domElement, const QString &name, const QString &defValue) const;
    /**
     * @brief Returns the double value of the given attribute.
     * @param domElement tag in xml tree
     * @param name attribute name
     * @return double value
     */
    qreal          GetParametrDouble(const QDomElement& domElement, const QString &name, const QString &defValue) const;
    /**
     * @brief Returns the id of the base point of the current pattern piece.
     * @return id of base point
     */
    qint64         SPointActiveDraw();
    bool           isPatternModified() const; // NOTE: inconsistent capitalization
    void           setPatternModified(bool value); // NOTE: inconsistent capitalization
    QString        UniqueTagText(const QString &tagName, const QString &defVal = QString());
signals:
    /**
     * @brief The active pattern piece was changed. RENAME: ChangedActiveDraw
     * @param newName new pattern piece name
     */
    void           ChangedActivDraw(const QString &newName);
    /**
     * @brief The name of the selected pattern piece was changed.
     * @param oldName old name
     * @param newName new name
     */
    void           ChangedNameDraw(const QString &oldName, const QString &newName);
    /**
     * @brief Update tool data from file.
     */
    void           FullUpdateFromFile();
    /**
     * @brief Emit if there exists an unsaved change to the pattern document.
     */
    void           patternChanged(); //NOTE: inconsistent capitalization
    /**
     * @brief Highlight tool.
     * @param id tool id
     * @param color highlight color
     * @param enable enable or disable highlight
     */
    void           ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    /**
     * @brief The cursor position has changed.
     * @param id tool id
     */
    void           ChangedCursor(qint64 id);
public slots:
    /**
     * @brief Perform a lite parse run on the pattern document.
     */
    void           FullUpdateTree();
    /**
     * @brief Sets that we have an unsaved change on the pattern file. Emits patternChanged().
     */
    void           haveLiteChange(); //NOTE: inconsistent capitalization
    /**
     * @brief Highlights a tool by id.
     * @param id tool id
     * @param color highlight color
     * @param enable enable or disable highlight
     */
    void           ShowHistoryTool(qint64 id, Qt::GlobalColor color, bool enable);
private:
    Q_DISABLE_COPY(VDomDocument)
    /**
     * @brief Map used for finding element by id.
     */
    QHash<QString, QDomElement> map;
    /**
     * @brief Name of the selected pattern piece.
     */
    QString        nameActivDraw;
    /**
     * @brief Container with pattern document data.
     */
    VContainer     *data;
    /**
     * @brief Tools map with pointers to tools.
     */
    QHash<qint64, VDataTool*> tools;
    /**
     * @brief Tool history record.
     */
    QVector<VToolRecord> history;
    /**
     * @brief Cursor keep id tool after which we will add new tool in file. TODO: very unclear what does that mean?
     */
    qint64         cursor;
    /**
     * @brief A pointer to the QComboBox used to select pattern piece names.
     */
    QComboBox      *comboBoxDraws;
    /**
     * @brief Current draw mode.
     */
    Draw::Draws    *mode;
    /**
     * @brief True if there is an unsaved change in the document.
     */
    bool            patternModified;
    /**
     * @brief Find element by id.
     * @param node node
     * @param id id value
     * @return true if found
     */
    bool           find(const QDomElement &node, const QString& id);
    /**
     * @brief Checks whether there exists a pattern piece with the given name.
     * @param name pattern piece name
     * @return true if it exists
     */
    bool           CheckNameDraw(const QString& name) const;
    /**
     * @brief Set selected pattern piece. RENAME: SetActiveDraw? SetActivePatternPiece?
     * @param name pattern piece name
     */
    void           SetActivDraw(const QString& name);
    /**
     * @brief Finds an element in the current pattern piece by name. RENAME: GetActiveNodeElement?
     * @param name name tag
     * @param element element returned by reference
     * @return true if found
     */
    bool           GetActivNodeElement(const QString& name, QDomElement& element);
    /**
     * @brief Parses a draw tag.
     * @param sceneDraw draw scene
     * @param sceneDetail details scene
     * @param node node
     * @param parse parse mode
     */
    void           ParseDrawElement(VMainGraphicsScene  *sceneDraw, VMainGraphicsScene *sceneDetail,
                                    const QDomNode& node, const Document::Documents &parse);
    /**
     * @brief Parses a draw tag in draw mode.
     * @param sceneDraw draw scene
     * @param sceneDetail details scene
     * @param node node
     * @param parse parse mode
     * @param mode draw mode
     */
    void           ParseDrawMode(VMainGraphicsScene  *sceneDraw, VMainGraphicsScene  *sceneDetail,
                                 const QDomNode& node, const Document::Documents &parse, const Draw::Draws &mode);
    /**
     * @brief Parses a detail element tag.
     * @param sceneDetail details scene
     * @param domElement tag in XML tree
     * @param parse parse mode
     */
    void           ParseDetailElement(VMainGraphicsScene  *sceneDetail, const QDomElement &domElement,
                                      const Document::Documents &parse);
    /**
     * @brief Parses a details tag.
     * @param sceneDetail details scene
     * @param domElement tag in XML tree
     * @param parse parse mode
     */
    void           ParseDetails(VMainGraphicsScene  *sceneDetail, const QDomElement &domElement,
                                const Document::Documents &parse);
    /**
     * @brief Parses a point element tag.
     * @param scene scene
     * @param domElement tag in XML tree
     * @param parse parse mode
     * @param type type of point
     * @param mode draw mode
     */
    void           ParsePointElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                     const Document::Documents &parse, const QString &type);
    /**
     * @brief Parses a line element tag.
     * @param scene scene
     * @param domElement tag in XML tree
     * @param parse parse mode
     * @param mode draw mode
     */
    void           ParseLineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                    const Document::Documents &parse);
    /**
     * @brief Parses a spline element tag.
     * @param scene scene
     * @param domElement tag in XML tree
     * @param parse parse mode.
     * @param type type of spline
     * @param mode draw mode
     */
    void           ParseSplineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                      const Document::Documents &parse, const QString& type);
    /**
     * @brief Parses an arc element tag.
     * @param scene scene
     * @param domElement tag in XML tree
     * @param parse parse mode
     * @param type type of spline
     * @param mode draw mode
     */
    void           ParseArcElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                   const Document::Documents &parse, const QString& type);
    /**
     * @brief Parses a tools element tag.
     * @param scene scene
     * @param domElement tag in XML tree
     * @param parse parse mode
     * @param type type of spline
     */
    void           ParseToolsElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                     const Document::Documents &parse, const QString& type);
    /**
     * @brief Parses an increments element tag.
     * @param node tag in XML tree
     */
    void           ParseIncrementsElement(const QDomNode& node);
    /**
     * @brief Returns the id attribute of the given element.  RENAME: GetParameterId?
     * @param domElement tag in XML tree
     * @return id value
     */
    qint64         GetParametrId(const QDomElement& domElement) const;
    /**
     * @brief Recursively collects all id attribute in file. Throws an exception if a duplicate is found.
     * @param node tag in XML tree
     * @param vector list with ids
     */
    void           CollectId(const QDomElement &node, QVector<qint64> &vector)const;
};

#ifdef Q_CC_GNU
    #pragma GCC diagnostic pop
#endif

#endif // VDOMDOCUMENT_H
