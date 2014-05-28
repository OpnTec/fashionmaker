/************************************************************************
 **
 **  @file   dialogpatternxmledit.h
 **  @author Patrick Proy <patrick(at)proy.org>
 **  @date   14 5, 2014
 **
 **  @brief  Include file for the XML editor dialog
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

#ifndef DIALOGPATTERNXMLEDIT_H
#define DIALOGPATTERNXMLEDIT_H

#include <QDialog>
#include <QStandardItemModel>
#include "../../xml/vpattern.h"

//********************************************************************************************
/**
 * @brief The vXMLTreeElement class : node/attribute of xml pattern. Used by vXMLTreeView
 *
 */

#define BACKGROUND_COLOR_ATTRIBUTE QBrush(Qt::GlobalColor::cyan)
#define BACKGROUND_COLOR_INACTIVE_NODE QBrush(Qt::GlobalColor::gray)
class VXMLTreeElement : public QStandardItem
{

public:
    /**
     * @brief vXMLTreeElement
     * @param name : display name
     * @param nodetype : node type (node, attribute, root)
     * @param source : the source dom node
     */
    explicit VXMLTreeElement (QString name, int nodetype, QDomNode source, bool editor);

    explicit VXMLTreeElement (QString name, int nodetype);

    ~VXMLTreeElement ();
    static const short int TypeNode;
    static const short int TypeAttr;
    static const short int TypeRoot;
    /**
     * @brief setNodeValue : set value of node (content or attribute)
     * @param value : the value
     */
    void SetTreeNodeValue(QString value);
    /**
     * @brief displayText : text to display
     * @return text to display
     */
    QString DisplayText();
    /**
     * @brief getDocNode
     * @return current document node
     */
    QDomNode GetDocNode();
    /**
     * @brief SetDocNode set tree element DomNode
     * @param node
     */
    inline void SetDocNode(QDomNode node)
    {
        this->DocNode=node;
    }
    inline void SetAddedNode(bool state) {this->addedNode=state;}
    inline bool GetAddedNode() {return this->addedNode;}

    QString GettreeNodeName();
    QString GettreeNodeValue();
    /**
     * @brief GettreeNodeValueSet check if value has been set
     * @return true if value is set
     */
    bool GettreeNodeValueSet();
    void SetTreeNodeName(QString value);
    /**
     * @brief getelementType
     * @return elementType value
     */
    short int GetelementType();
    bool IsSelectable();
    void SetSetlectable(bool value);

    inline void SetFatherElement(VXMLTreeElement* parent)
    {
        this->parentnode=parent;
    }
    inline VXMLTreeElement* GetFatherElement()
    {
        return this->parentnode;
    }

private:
    /**
     * @brief DocNode : link to current document node
     */
    QDomNode DocNode;

    /**
     * @brief parentnode parent node of this element. If null then the DocNode must be valid
     */
    VXMLTreeElement* parentnode;

    /**
     * @brief addedNode : true if node is added by editor (implies empty DocNode)
     */
    bool addedNode;
    /**
     * @brief type : element type (node, attribute, text value of node)
     */
    short int elementType;
    /**
     * @brief nodeValue : Attribute or node value
     */
    QString treeNodeValue;
    /**
     * @brief treeNodeValueSet : true if value has been set
     */
    bool treeNodeValueSet;
    /**
     * @brief nodeValue : Attribute or node value
     */
    QString treeNodeName;
    /**
     * @brief selectable : can item be selected to be changed (ex : false if father deleted).
     */
    bool selectable;
    Q_DISABLE_COPY(VXMLTreeElement)
};

inline bool VXMLTreeElement::IsSelectable()
{
    return this->selectable;
}

inline void VXMLTreeElement::SetSetlectable(bool value)
{
    this->selectable=value;
}

inline short int VXMLTreeElement::GetelementType()
{
    return this->elementType;
}

inline QDomNode VXMLTreeElement::GetDocNode()
{
    return this->DocNode;
}

inline QString VXMLTreeElement::GettreeNodeName()
{
    return this->treeNodeName;
}

inline bool VXMLTreeElement::GettreeNodeValueSet()
{
    return this->treeNodeValueSet;
}

//********************************************************************************************
/**
 * @brief The vXMLTreeView class : container to display/edit xml pattern
 */
class VXMLTreeView : public QStandardItemModel
{
public:
    explicit VXMLTreeView (QObject *parent = 0);
    void appendchain(VXMLTreeElement* elmt);

    /**
     * @brief The TreeElement struct : chained list of vXMLTreeElement
     */
    typedef struct TreeElementchain
    {
        VXMLTreeElement* elmt;
        TreeElementchain* next;
    } TreeElementchain;

    TreeElementchain * getCurrent();
    void SetCurrent(TreeElementchain * value);
    TreeElementchain * GetLast();
    TreeElementchain * GetItems();
    /**
     * @brief clearTree : clear tree elements and listed items.
     */
    void ClearTree();
    ~VXMLTreeView ();
private:
    Q_DISABLE_COPY(VXMLTreeView)

    /**
     * @brief items : root of chained list of vXMLTreeElement
     */
    TreeElementchain * items;
    /**
     * @brief current : used to parse in iteration.
     */
    TreeElementchain * current;
    /**
     * @brief last : used to parse in iteration.
     */
    TreeElementchain * last;
};

inline VXMLTreeView::TreeElementchain * VXMLTreeView::getCurrent()
{
    return this->current;
}
inline void VXMLTreeView::SetCurrent(VXMLTreeView::TreeElementchain * value)
{
    this->current=value;
}
inline VXMLTreeView::TreeElementchain * VXMLTreeView::GetLast()
{
    return this->last;
}
inline VXMLTreeView::TreeElementchain * VXMLTreeView::GetItems()
{
    return this->items;
}

//********************************************************************************************

namespace Ui
{
    class DialogPatternXmlEdit;
}
/**
 * @brief The DialogPatternXmlEdit class : ui dialog for XML editing of pattern
 */
class DialogPatternXmlEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPatternXmlEdit(QWidget *parent = 0, VPattern *xmldoc = 0);
    ~DialogPatternXmlEdit();

    /**
     * @brief clear_edit_data :  clear input boxes and disables buttons.
     */
    void ClearEditData();

    void NameTextEdited(QString newtext);
    void ValueTextEdited(QString newtext);
    void ButtonSetClicked();
    void ButtonCancelClicked();
    void ButtonDeleteAttributeClicked();
    void ButtonAddSonClicked();
    void ButtonAddAttributeClicked();
    void ButtonApplyChangesClicked();
    void ButtonUndoLastChange();
    void ButtonDeleteNode();

    // Stack of changes definition
    typedef struct ChangesStackElement
    {
        short int type;
        VXMLTreeElement *element;
        QString *newText;
        QString *oldText;
        bool changedText;
        QString *newValue;
        QString *oldValue;
        bool changedValue;
        ChangesStackElement* next;
    } ChangesStackElement;

    // Change stack functions
    ChangesStackElement* CreateStackElement(short int typechange);
    void RemoveChangeStackElement(ChangesStackElement* elmt);
    void ClearStack();
    bool treeChange;
    VXMLTreeElement* currentNodeEdited;
    short int currentNodeEditedStatus;
    ChangesStackElement* currentNodeEditedStack;
    /**
     * @brief UndoChange : undo change in change stack element
     * @param current : change to undo
     * @return true if undo has been done, false on error
     */
    bool UndoChange(ChangesStackElement* current);
    /**
     * @brief DeleteNodeAndSons : delete node and all it's attribute and sons below
     * @param currentNode : node to delete
     * @param onlydeactivate : if true, dont delete just deactivate sons and attributes
     * @return false if changes couldn't be done
     */
    bool DeleteNodeAndSons(VXMLTreeElement *currentNode, bool onlydeactivate);

    /**
     * @brief ApplyAttributeChange change or add attribute for node
     * @param domElement : node containing attribute
     * @param name
     * @param value
     * @return true on success
     */
    bool ApplyAttributeChange(QDomNode domElement, QString name, QString value);
    /**
     * @brief ApplyNodeChange Change name or text content of node
     * @param domElement node
     * @param name
     * @param value text content of node
     * @return true on success
     */
    bool ApplyNodeChange(QDomNode domElement, QString name, QString value);
    /**
     * @brief ApplyNodeAdd add node as child of domElement and set DocNode of treeElement
     * @param domElement
     * @param treeElement the XMLTreeElement of added node
     * @param name
     * @param value
     * @return true on success
     */
    bool ApplyNodeAdd(QDomNode domElement, VXMLTreeElement* treeElement, QString name, QString value);
    /**
     * @brief ApplyAttributeDelete delete attribute for node
     * @param domElement
     * @param name attribute name
     * @return true on success
     */
    bool ApplyAttributeDelete(QDomNode domElement, QString name);
    /**
     * @brief ApplyNodeDelete delete node domElement
     * @param domElement
     * @return true on success
     */
    bool ApplyNodeDelete(QDomNode domElement);
    /**
     * @brief CheckChanges Check if changes made are OK
     * @param message error message returned if false
     * @param testRoot root of DOM to test
     * @return true if validated, false otherwise
     */
    bool CheckChanges(QString &message, QDomNode testRoot);
private slots:
    void BaseSelectionChanged(int value);
    void ElementClicked ( const QModelIndex & index );

private:
    Q_DISABLE_COPY(DialogPatternXmlEdit)
    Ui::DialogPatternXmlEdit *ui;

    VPattern          *doc;

    QDomElement root;
    QStandardItem *rootNode;
    //typedef struct rootbases { vXMLTreeElement * root; QString name; } rootbases;
    VXMLTreeElement ** rootBases;
    qint16 rootBasesNum;

    void ReadNodes(QDomNode dNode, VXMLTreeElement* root, VXMLTreeView *xmlmodel, bool refresh);
    VXMLTreeView* xmlmodel;


    // Stack of changes
    static const short int ChangeTypeDelete;
    static const short int ChangeTypeAdd;
    static const short int ChangeTypeModify;

    // Stack of changes

    ChangesStackElement* changeStackRoot;
    ChangesStackElement* changeStackLast;
};

#endif // DIALOGPATTERNXMLEDIT_H
