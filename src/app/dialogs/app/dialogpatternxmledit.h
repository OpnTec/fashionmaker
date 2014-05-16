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
    void setTreeNodeValue(QString value);
    /**
     * @brief displayText : text to display
     * @return text to display
     */
    QString displayText();
    /**
     * @brief getDocNode
     * @return current document node
     */
    QDomNode getDocNode();
    QString gettreeNodeName();
    QString gettreeNodeValue();
    bool gettreeNodeValueSet();
    void setTreeNodeName(QString value);
    /**
     * @brief getelementType
     * @return elementType value
     */
    short int getelementType();

private:
    /**
     * @brief DocNode : link to current document node
     */
    QDomNode DocNode;

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
    Q_DISABLE_COPY(VXMLTreeElement)
};

inline short int VXMLTreeElement::getelementType()
{
    return this->elementType;
}

inline QDomNode VXMLTreeElement::getDocNode()
{
    return this->DocNode;
}

inline QString VXMLTreeElement::gettreeNodeName()
{
    return this->treeNodeName;
}

inline bool VXMLTreeElement::gettreeNodeValueSet()
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


    // Stack of changes definition
    typedef struct ChangesStackElement
    {
        short int type;
        VXMLTreeElement *element;
        QString *newText;
        bool changedText;
        QString *newValue;
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
