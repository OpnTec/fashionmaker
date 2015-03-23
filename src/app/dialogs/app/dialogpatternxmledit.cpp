/************************************************************************
 **
 **  @file   dialogpatternxmledit.cpp
 **  @author Patrick Proy <patrick(at)proy.org>
 **  @date   14 5, 2014
 **
 **  @brief  Code file for the XML editor dialog
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

#include "dialogpatternxmledit.h"
#include "ui_dialogpatternxmledit.h"
#include "../core/vapplication.h"

#include <QInputDialog>
#include <QMessageBox>

const short int DialogPatternXmlEdit::ChangeTypeDelete=1;
const short int DialogPatternXmlEdit::ChangeTypeAdd=2;
const short int DialogPatternXmlEdit::ChangeTypeModify=3;

//---------------------------------------------------------------------------------------------------------------------
DialogPatternXmlEdit::DialogPatternXmlEdit(QWidget *parent, VPattern *xmldoc)
    :QDialog(parent), treeChange(false), currentNodeEdited(nullptr), currentNodeEditedStatus(0),
      currentNodeEditedStack(nullptr), ui(new Ui::DialogPatternXmlEdit), doc(nullptr), root(QDomElement()),
      rootNode(nullptr), rootBases(nullptr), rootBasesNum(0), xmlmodel(nullptr), changeStackRoot(nullptr),
      changeStackLast(nullptr)
{
    ui->setupUi(this);

    qApp->getSettings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale(QLocale::C));

    this->xmlmodel = new VXMLTreeView();
    this->doc=xmldoc;
    this->root = this->doc->documentElement();
    // disable edit trigers (no edition directly on tree)
    ui->treeView_main->setEditTriggers(QTreeView::NoEditTriggers);

    ui->treeView_main->setWindowTitle("XML");

    this->rootNode = this->xmlmodel->invisibleRootItem();

    int drawnum=xmldoc->elementsByTagName("draw").size();
    this->rootBases = new VXMLTreeElement*[drawnum+1];
    this->rootBasesNum=1;
    this->rootBases[0]=new VXMLTreeElement("Valentina", VXMLTreeElement::TypeRoot, root, false);
    // TODO : OOM exception.


    VXMLTreeElement* standard_base = new VXMLTreeElement("Valentina", VXMLTreeElement::TypeRoot, root, false);
    ui->comboBox_Base_Selection->addItem(tr("All pattern pieces"), QVariant(0));

    //rootBases[0]=(QDomNode) doc->DocumentNode;
    this->rootNode->appendRow(standard_base);

    this->ReadNodes(this->root, standard_base, this->xmlmodel, false);

    ui->treeView_main->setModel(xmlmodel);

    //ui->treeView_main->expandAll();

    // connectors
    connect(ui->comboBox_Base_Selection, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &DialogPatternXmlEdit::BaseSelectionChanged);
    connect(ui->treeView_main, &QTreeView::clicked, this, &DialogPatternXmlEdit::ElementClicked);
    connect(ui->lineEdit_Name, &QLineEdit::textEdited, this, &DialogPatternXmlEdit::NameTextEdited);
    connect(ui->lineEdit_Value, &QLineEdit::textEdited, this, &DialogPatternXmlEdit::ValueTextEdited);
    connect(ui->pushButton_Set_Values, &QPushButton::clicked, this, &DialogPatternXmlEdit::ButtonSetClicked);
    connect(ui->pushButton_Cancel_Values, &QPushButton::clicked, this, &DialogPatternXmlEdit::ButtonCancelClicked);
    connect(ui->pushButton_Remove_attribute, &QPushButton::clicked,
            this, &DialogPatternXmlEdit::ButtonDeleteAttributeClicked);
    connect(ui->pushButton_Add_son, &QPushButton::clicked, this, &DialogPatternXmlEdit::ButtonAddSonClicked);
    connect(ui->pushButton_Add_attribute, &QPushButton::clicked,
            this, &DialogPatternXmlEdit::ButtonAddAttributeClicked);
    connect(ui->pushButton_Apply_Changes, &QPushButton::clicked,
            this, &DialogPatternXmlEdit::ButtonApplyChangesClicked);
    connect(ui->pushButton_Undo_Last_Change, &QPushButton::clicked, this, &DialogPatternXmlEdit::ButtonUndoLastChange);
    connect(ui->pushButton_Remove_Node, &QPushButton::clicked, this, &DialogPatternXmlEdit::ButtonDeleteNode);
}

bool DialogPatternXmlEdit::CheckChanges(QString &message, QDomNode testRoot)
{
    Q_UNUSED(testRoot)
    message="OK";
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogPatternXmlEdit::ApplyAttributeChange(QDomNode domElement, QString name, QString value)
{
    if (domElement.isElement() == false)
    {
        return false;
    }
    domElement.toElement().setAttribute(name, value);
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogPatternXmlEdit::ApplyNodeChange(QDomNode domElement, QString name, QString value)
{
    if (domElement.isElement() == false)
    {
        return false;
    }
    domElement.toElement().setNodeValue(value);
    domElement.toElement().setTagName(name);
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogPatternXmlEdit::ApplyNodeAdd(QDomNode domElement, VXMLTreeElement* treeElement, QString name, QString value)
{
    QDomElement newnode = domElement.ownerDocument().createElement(name);
    //SCASSERT(newnode != nullptr);
    newnode.setNodeValue(value);
    domElement.appendChild(newnode);
    treeElement->SetDocNode(newnode);
    return true;
}

bool DialogPatternXmlEdit::ApplyAttributeDelete(QDomNode domElement, QString name)
{
    if (domElement.isElement() == false)
    {
        return false;
    }
    domElement.toElement().removeAttribute(name);
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogPatternXmlEdit::ApplyNodeDelete(QDomNode domElement)
{
    QDomNode mother=domElement.parentNode();
    mother.removeChild(domElement);
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::ButtonDeleteNode()
{
    ChangesStackElement* newstack;

    // TODO : maybe assert functions here
    if (this->currentNodeEdited == nullptr)
    {
        return;
    }
    if (this->currentNodeEdited->GetelementType() != VXMLTreeElement::TypeNode)
    {
        return;
    }

    if (this->currentNodeEditedStatus != 0)
    {
        if (this->currentNodeEditedStatus == DialogPatternXmlEdit::ChangeTypeDelete)
        { // Change already done ??? ignore...
            return;
        }
        if (this->currentNodeEditedStatus == DialogPatternXmlEdit::ChangeTypeAdd)
        { // Attribute was previously added, just undo the change
            if ( this->UndoChange(this->currentNodeEditedStack) == false )
            { // Error in undo... warn user
                QMessageBox::warning(this, tr("No changes"), tr("Cannot delete previously created node"));
                return;
            }
            this->ClearEditData();
            return;
        }
        if (this->currentNodeEditedStatus == DialogPatternXmlEdit::ChangeTypeModify)
        { // Node was edited : remove change then continue to delete node
            this->UndoChange(this->currentNodeEditedStack);
        }
    }

    // Modify display
    QFont textfont=currentNodeEdited->font();
    textfont.setStrikeOut(true);
    this->currentNodeEdited->setFont(textfont);
    //
    this->DeleteNodeAndSons(this->currentNodeEdited, true);
    // Create change
    newstack = this->CreateStackElement(DialogPatternXmlEdit::ChangeTypeDelete);
    newstack->element= this->currentNodeEdited;
    // Activate changes
    this->treeChange=true;
    ui->pushButton_Apply_Changes->setEnabled(true);
    ui->pushButton_Undo_Last_Change->setEnabled(true);
    // clear dialog
    this->ClearEditData();
    return;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogPatternXmlEdit::DeleteNodeAndSons(VXMLTreeElement * currentNode, bool onlydeactivate)
{

    QList<VXMLTreeElement*> stack;
    VXMLTreeElement * currentNodeStack;
    // First undo all changes on sons
    stack << currentNode;
    while (stack.isEmpty() == false)
    {
        //pop first element
        currentNodeStack=stack.takeFirst();
        // clear all changes made in son node and not on current or it will loop
        if (currentNodeStack != currentNode)
        {
            ChangesStackElement * local = this->changeStackRoot;
            while (local != nullptr)
            {
                if (local->element == currentNodeStack)
                {
                    this->UndoChange(local);
                    break;
                }
                local=local->next;
            }
        }
        // add sons to stack
        int index=currentNodeStack->rowCount();
        while (index > 0)
        {
            stack << static_cast<VXMLTreeElement *> (currentNodeStack->child(index-1));
            index--;
        }
        //stack.removeFirst();
    }
    // Next set strike font on all
    if (onlydeactivate==true)
    {
        stack << currentNode;
        while (stack.isEmpty() == false)
        {
            //pop first element
            currentNodeStack=stack.takeFirst();

            currentNodeStack->SetSetlectable(false);
            QFont textfont=currentNodeStack->font();
            textfont.setStrikeOut(true);
            currentNodeStack->setFont(textfont);
            // add sons to stack
            int index=currentNodeStack->rowCount();
            while (index > 0)
            {
                stack << static_cast<VXMLTreeElement *> (currentNodeStack->child(index-1));
                index--;
            }
        }
    }

    currentNode->SetSetlectable(true);

   if (onlydeactivate==false)
   { // Remove node from parent
       // Get parent node
       QStandardItem * parent= currentNode->parent();
       int index=parent->rowCount();
       while (index >= 0)
       {
           if (static_cast<VXMLTreeElement *> (parent->child(index)) ==  currentNode)
           {
               break;
           }
           index--;
       }
       if (index < 0)
       {
           SCASSERT(index==0);
           return false;
       }
       parent->removeRow(index);
   }
   return true;
}

//---------------------------------------------------------------------------------------------------------------------
DialogPatternXmlEdit::ChangesStackElement* DialogPatternXmlEdit::CreateStackElement(short int typechange)
{
    ChangesStackElement* tmp;

    tmp= new ChangesStackElement;
    //if (tmp==0) TODO
    tmp->type=typechange;
    tmp->changedText=tmp->changedValue=false;

    if (changeStackLast==nullptr )
    {
        changeStackLast=changeStackRoot=tmp;
    }
    else
    {
        changeStackLast->next=tmp;
        changeStackLast=changeStackLast->next;
    }
    changeStackLast->newValue=changeStackLast->newText=nullptr;
    changeStackLast->oldValue=changeStackLast->oldText=nullptr;
    changeStackLast->next=nullptr;
    return changeStackLast;
}

//---------------------------------------------------------------------------------------------------------------------
bool DialogPatternXmlEdit::UndoChange(DialogPatternXmlEdit::ChangesStackElement* current)
{
    SCASSERT(current != nullptr);

    VXMLTreeElement * currentNode = current->element;

    if (current->type == DialogPatternXmlEdit::ChangeTypeDelete)
    {
        if (currentNode->GetelementType()==VXMLTreeElement::TypeAttr)
        {
            // Attribute previously deleted -> restore
            QFont textfont=currentNode->font();
            textfont.setStrikeOut(false);
            currentNode->setFont(textfont);
            this->RemoveChangeStackElement(current);
        }
        else if (currentNode->GetelementType()==VXMLTreeElement::TypeNode)
        {
            QList<VXMLTreeElement*> stack;
            stack << currentNode;
            while (stack.isEmpty() == false)
            {
                currentNode=stack.takeFirst();
                currentNode->SetSetlectable(true);
                QFont textfont=currentNode->font();
                textfont.setStrikeOut(false);
                currentNode->setFont(textfont);

                int index=currentNode->rowCount();
                while (index > 0)
                {
                    stack << static_cast<VXMLTreeElement *> (currentNode->child(index-1));
                    index--;
                }
            }
            this->RemoveChangeStackElement(current);
        }
    }
    else if (current->type == DialogPatternXmlEdit::ChangeTypeAdd)
    {
        if (currentNode->GetelementType()==VXMLTreeElement::TypeAttr)
        {
            // Get parent node
            VXMLTreeElement * parent= static_cast<VXMLTreeElement *> (currentNode->parent());
            int index=parent->rowCount();
            while (index >= 0)
            {
                if (static_cast<VXMLTreeElement *> (parent->child(index)) ==  currentNode)
                {
                    break;
                }
                index--;
            }
            if (index < 0)
            {
                SCASSERT(index==0);
                return false;
            }
            parent->removeRow(index);
            this->RemoveChangeStackElement(current);
        }
        else if (currentNode->GetelementType()==VXMLTreeElement::TypeNode)
        {
            if (this->DeleteNodeAndSons(currentNode, false) == false)
            {
                // TODO : error message.
                return false;
            }
            this->RemoveChangeStackElement(current);
        }
    }
    else if (current->type == DialogPatternXmlEdit::ChangeTypeModify)
    {
        if (current->changedText == true)
        {
            currentNode->SetTreeNodeName(*current->oldText);
        }
        if (current->changedValue == true)
        {
            currentNode->SetTreeNodeValue(*current->oldValue);
        }

        QFont textfont=currentNode->font();
        textfont.setBold(false);
        currentNode->setFont(textfont);
        this->RemoveChangeStackElement(current);
    }
    // Check if last change was undone
    if (this->changeStackLast == nullptr)
    {
        this->treeChange=false;
        ui->pushButton_Apply_Changes->setEnabled(false);
        ui->pushButton_Undo_Last_Change->setEnabled(false);
    }
    return true;
}


//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::ButtonUndoLastChange()
{
    ChangesStackElement* current=this->changeStackLast;

    if (current == nullptr)
    {
        QMessageBox::warning(this, tr("No changes"), tr("No changes left"));
        this->ui->pushButton_Undo_Last_Change->setEnabled(false);
        return;
    }
    if (this->UndoChange(current) == false)
    {
        QMessageBox::warning(this, tr("No changes"), tr("Cannot undo change"));
    }
    this->ClearEditData();

}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::ButtonApplyChangesClicked()
{
    QString Changes="";
    QStringList stringtype;
    stringtype << "node" << "attribute" << "root";
    QString nodetype;
    ChangesStackElement* currentChange;
    if (this->changeStackRoot==nullptr)
    {
        Changes="No Changes";
    }
    else
    {
        currentChange=this->changeStackRoot;
        do
        {
            nodetype=stringtype.at(currentChange->element->GetelementType()-1);
            if (currentChange->type == DialogPatternXmlEdit::ChangeTypeAdd)
            {
                Changes += QString("Added type %1 : ").arg(nodetype);
                Changes += currentChange->element->GettreeNodeName();
                Changes += "/";
                Changes += (currentChange->element->GettreeNodeValueSet()) ?
                            currentChange->element->GettreeNodeValue(): tr("<no value>");
                Changes += "\n";
            }
            else
            {
                if (currentChange->type == DialogPatternXmlEdit::ChangeTypeModify)
                {
                    Changes += QString("Modified type %1 : ").arg(nodetype);
                    Changes += (currentChange->changedText) ? *currentChange->newText : tr("Unchanged");
                    Changes += "/";
                    Changes += (currentChange->changedValue) ? *currentChange->newValue: tr("Unchanged");
                    Changes += "\n";
                }
                else
                {
                    if (currentChange->type == DialogPatternXmlEdit::ChangeTypeDelete)
                    {
                        Changes += QString("Deleted type %1 : ").arg(nodetype);
                        Changes += currentChange->element->GettreeNodeName();
                        Changes += "/";
                        Changes += (currentChange->element->GettreeNodeValueSet()) ?
                                    currentChange->element->GettreeNodeValue(): tr("<no value>");
                        Changes += "\n";
                    }
                }
            }
            currentChange=currentChange->next;
        } while (currentChange != nullptr);
    }
    QMessageBox::information(this, "Changes : ", Changes);
    // TODO : clear stack and apply

    currentChange=this->changeStackRoot;
    // copy DOM
    QDomNode newroot = root.cloneNode(true);

    QString name, value;
    while (currentChange != nullptr)
    {
        if (currentChange->type == DialogPatternXmlEdit::ChangeTypeModify)
        {
            name=(currentChange->changedText == false) ? currentChange->element->GettreeNodeName()
                                                       : *currentChange->newText;
            value=(currentChange->changedValue == false) ? currentChange->element->GettreeNodeValue()
                                                         : *currentChange->newValue;
            if (currentChange->element->GetelementType() == VXMLTreeElement::TypeAttr)
            {

                this->ApplyAttributeChange(currentChange->element->GetDocNode(), name, value);
            }
            else if (currentChange->element->GetelementType() == VXMLTreeElement::TypeNode)
            {
                this->ApplyNodeChange(currentChange->element->GetDocNode(), name, value);
            }
            else
            {
                QMessageBox::warning(this, "Error in changes", "Invalid node type");
                return;
            }
        }
        else if (currentChange->type == DialogPatternXmlEdit::ChangeTypeAdd)
        {
            name=(currentChange->changedText == false) ? currentChange->element->GettreeNodeName()
                                                       : *currentChange->newText;
            value=(currentChange->changedValue == false) ? currentChange->element->GettreeNodeValue()
                                                         : *currentChange->newValue;
            if (currentChange->element->GetelementType() == VXMLTreeElement::TypeAttr)
            {
                this->ApplyAttributeChange(currentChange->element->GetFatherElement()->GetDocNode(), name, value);
            }
            else if (currentChange->element->GetelementType() == VXMLTreeElement::TypeNode)
            {
                this->ApplyNodeAdd(currentChange->element->GetFatherElement()->GetDocNode(), currentChange->element,
                                   name, value);
            }
            else
            {
                QMessageBox::warning(this, "Error in changes", "Invalid node type");
                return;
            }
        }
        else if (currentChange->type == DialogPatternXmlEdit::ChangeTypeDelete)
        {
            if (currentChange->element->GetelementType() == VXMLTreeElement::TypeAttr)
            {
                name=(currentChange->changedText == false) ? currentChange->element->GettreeNodeName()
                                                           : *currentChange->newText;
                this->ApplyAttributeDelete(currentChange->element->GetDocNode(), name);
            }
            else if (currentChange->element->GetelementType() == VXMLTreeElement::TypeNode)
            {
                this->ApplyNodeDelete(currentChange->element->GetDocNode().parentNode());
            }
            else
            {
                QMessageBox::warning(this, "Error in changes", "Invalid node type");
                return;
            }
        }
        currentChange=currentChange->next;
    }
    QString message;
    if (this->CheckChanges(message, newroot) == false)
    {
        QMessageBox::warning(this, "Error in changes", message);
        return;
    }
    this->doc->LiteParseTree(Document::LiteParse);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::ButtonCancelClicked()
{
   this->ClearEditData();
   if (this->currentNodeEditedStatus==DialogPatternXmlEdit::ChangeTypeDelete)
   {
       if (this->UndoChange(this->currentNodeEditedStack) == false)
       {
           QMessageBox::warning(this, "Error", "Cannot undo");
       }
   }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::RemoveChangeStackElement(ChangesStackElement* elmt)
{
    ChangesStackElement* index = this->changeStackRoot;

    if (changeStackRoot == elmt)
    {
        if (elmt->newText != nullptr)
        {
            delete elmt->newText;
        }
        if (elmt->newValue != nullptr)
        {
            delete elmt->newValue;
        }
        if (elmt->oldText != nullptr)
        {
            delete elmt->oldText;
        }
        if (elmt->oldValue != nullptr)
        {
            delete elmt->oldValue;
        }
        this->changeStackRoot = elmt->next;
        if (this->changeStackLast == elmt)
        {
            this->changeStackLast= elmt->next;
        }
        delete elmt;
        return;
    }
    while (index->next!=nullptr)
    {
        if (index->next == elmt)
        {
            break;
        }
        index=index->next;
    }
    if (index->next == nullptr)
    {
        SCASSERT(index->next != nullptr);
        return;
    }
    if (index->next->newText != nullptr)
    {
        delete index->next->newText;
    }
    if (index->next->newValue != nullptr)
    {
        delete index->next->newValue;
    }
    if (index->next->oldText != nullptr)
    {
        delete index->next->oldText;
    }
    if (index->next->oldValue != nullptr)
    {
        delete index->next->oldValue;
    }
    index->next=index->next->next;
    if (this->changeStackLast == elmt)
    {
        this->changeStackLast= index;
    }
    delete elmt;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::ButtonDeleteAttributeClicked()
{
    ChangesStackElement* newstack;

    // TODO : maybe assert functions here
    if (this->currentNodeEdited == nullptr)
    {
        return;
    }
    if (this->currentNodeEdited->GetelementType() != VXMLTreeElement::TypeAttr)
    {
        return;
    }

    if (this->currentNodeEditedStatus != 0)
    {
        if (this->currentNodeEditedStatus == DialogPatternXmlEdit::ChangeTypeDelete)
        { // Change already done ??? ignore...
            return;
        }
        if (this->currentNodeEditedStatus == DialogPatternXmlEdit::ChangeTypeAdd)
        { // Attribute was previously added, just undo the change
            if ( this->UndoChange(this->currentNodeEditedStack) == false )
            { // Error in undo... warn user
                QMessageBox::warning(this, tr("No changes"), tr("Cannot delete previously created attribute"));
                return;
            }
            this->ClearEditData();
            return;
        }
        if (this->currentNodeEditedStatus == DialogPatternXmlEdit::ChangeTypeModify)
        { // Node was edited : remove change then continue to delete node
            this->UndoChange(this->currentNodeEditedStack);
        }
    }
    newstack = this->CreateStackElement(DialogPatternXmlEdit::ChangeTypeDelete);
    newstack->element= this->currentNodeEdited;

    QFont textfont=currentNodeEdited->font();
    textfont.setStrikeOut(true);
    this->currentNodeEdited->setFont(textfont);
    // Activate changes
    this->treeChange=true;
    ui->pushButton_Apply_Changes->setEnabled(true);
    ui->pushButton_Undo_Last_Change->setEnabled(true);
    // clear dialog
    this->ClearEditData();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::ButtonAddSonClicked()
{
    ChangesStackElement* newstack;
    bool ok;
    //clear_edit_data();
    // TODO : maybe assert functions here
    if (this->currentNodeEdited == nullptr)
    {
        return;
    }
    if (this->currentNodeEdited->GetelementType() != VXMLTreeElement::TypeNode)
    {
        return;
    }


    QString name = QInputDialog::getText(this, tr("Node Name"), tr("Name:"), QLineEdit::Normal, "", &ok);
    if (ok==false)
    {
        return;
    }
    QString value = QInputDialog::getText(this, tr("Node Value (may be empty)"), tr("Value:"),
                                          QLineEdit::Normal, "", &ok);
    if (ok==false)
    {
        return;
    }

    // create element tree and put father tree element to catch up with DOM tree.
    QDomNode empty;
    VXMLTreeElement* tElement = new VXMLTreeElement(name, VXMLTreeElement::TypeNode, empty, true);
    tElement->SetFatherElement(this->currentNodeEdited);

    if (value.isEmpty() == false)
    {
        tElement->SetTreeNodeValue(value);
    }
    currentNodeEdited->appendRow(tElement);
    xmlmodel->appendchain(tElement);
    QFont textfont=currentNodeEdited->font();
    textfont.setBold(true);
    tElement->setFont(textfont);

    // Add to change stack
    newstack = this->CreateStackElement(DialogPatternXmlEdit::ChangeTypeAdd);
    newstack->element=tElement;
    newstack->newText=new QString(name); newstack->changedText=true;
    if (value.isEmpty())
    {
        newstack->changedValue=true;newstack->newValue=new QString(value);
    }
    // Activate changes
    treeChange=true;
    ui->pushButton_Apply_Changes->setEnabled(true);
    ui->pushButton_Undo_Last_Change->setEnabled(true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::ButtonAddAttributeClicked()
{
    ChangesStackElement* newstack;
    bool ok;
    QDomNode empty; // empty QNode to pas to create function : maybe a better way to do this ?

    // TODO : maybe assert functions here
    if (this->currentNodeEdited == nullptr)
    {
        return;
    }
    if (this->currentNodeEdited->GetelementType() != VXMLTreeElement::TypeNode)
    {
        return;
    }

    QString name = QInputDialog::getText(this, tr("Attribute Name"),
                                                tr("Name:"), QLineEdit::Normal,
                                                "", &ok);
    if (ok==false)
    {
        return;
    }
    QString value = QInputDialog::getText(this, tr("Attribute Value"),
                                                tr("Value:"), QLineEdit::Normal,
                                                "", &ok);
    if (ok==false)
    {
        return;
    }

    // create element tree but do not add attribute in DOM (will be done in apply).
    VXMLTreeElement* tElement = new VXMLTreeElement(name, VXMLTreeElement::TypeAttr, empty, true);
    // set parent node.
    tElement->SetFatherElement(this->currentNodeEdited);

    tElement->SetTreeNodeValue(value);
    currentNodeEdited->appendRow(tElement);
    xmlmodel->appendchain(tElement);
    QFont textfont=currentNodeEdited->font();
    textfont.setBold(true);
    tElement->setFont(textfont);
    // Add to change stack
    newstack = this->CreateStackElement(DialogPatternXmlEdit::ChangeTypeAdd);
    newstack->element= tElement;
    newstack->type=VXMLTreeElement::TypeAttr;
    newstack->newText=new QString(name);
    newstack->newValue=new QString(value);
    // Activate changes
    treeChange=true;
    ui->pushButton_Apply_Changes->setEnabled(true);
    ui->pushButton_Undo_Last_Change->setEnabled(true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::ButtonSetClicked()
{
    QString name, value;
    ChangesStackElement* newstack;

    if (this->currentNodeEdited == nullptr)
    {
        return;
    }
    name=ui->lineEdit_Name->text();
    value=ui->lineEdit_Value->text();
    bool nameedit, valueedit;

    nameedit = (name != currentNodeEdited->GettreeNodeName()) ? true : false;
    valueedit= (value != currentNodeEdited->GettreeNodeValue()) ? true : false;

    if ( nameedit == true  || valueedit == true )
    {
        if (this->currentNodeEditedStatus != 0)
        { // If node was created or edited previously, rewrite
            if (this->currentNodeEditedStatus == DialogPatternXmlEdit::ChangeTypeDelete)
            {   // You shouldn't be able to edit a deleted node...
                SCASSERT(this->currentNodeEditedStatus != DialogPatternXmlEdit::ChangeTypeDelete);
                ClearEditData();
                return;
            }
            newstack=this->currentNodeEditedStack;
            if (nameedit == true)
            {
                newstack->changedText=true;
                *newstack->newText=name;
                currentNodeEdited->SetTreeNodeName(name);
            }
            if (valueedit == true)
            {
                if (newstack->changedValue==true)
                {
                    *newstack->newValue = value;
                }
                else
                {
                    newstack->changedValue=true;
                    newstack->newValue= new QString(value);
                    newstack->oldValue= new QString(this->currentNodeEdited->GettreeNodeValue());
                }
                currentNodeEdited->SetTreeNodeValue(value);
            }
        }
        else
        {
            newstack = this->CreateStackElement(DialogPatternXmlEdit::ChangeTypeModify);
            newstack->element= this->currentNodeEdited;

            if (nameedit == true)
            {
                newstack->changedText=true;
                newstack->newText=new QString(name);
                newstack->oldText=new QString(this->currentNodeEdited->GettreeNodeName());
                currentNodeEdited->SetTreeNodeName(name);
            }
            if (valueedit == true)
            {
                newstack->changedValue=true;
                newstack->newValue= new QString(value);
                newstack->oldValue= new QString(this->currentNodeEdited->GettreeNodeValue());
                currentNodeEdited->SetTreeNodeValue(value);
            }
        }
        QFont textfont=currentNodeEdited->font();
        textfont.setBold(true);
        currentNodeEdited->setFont(textfont);
        treeChange=true;
        ui->pushButton_Apply_Changes->setEnabled(true);
        ui->pushButton_Undo_Last_Change->setEnabled(true);
        ClearEditData();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::NameTextEdited(QString newtext)
{
    Q_UNUSED(newtext)
    //QMessageBox::information(this, "nameTextEdited", QString("%1").arg(newtext));
    ui->pushButton_Set_Values->setEnabled(true);
    ui->pushButton_Cancel_Values->setEnabled(true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::ValueTextEdited(QString newtext)
{
    Q_UNUSED(newtext)
    //QMessageBox::information(this, "valueTextEdited", QString("%1").arg(newtext));
    ui->pushButton_Set_Values->setEnabled(true);
    ui->pushButton_Cancel_Values->setEnabled(true);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::ClearEditData()
{
    ui->lineEdit_Name->setText("");
    ui->lineEdit_Value->setText("");
    ui->lineEdit_Name->setEnabled(false);
    ui->lineEdit_Value->setEnabled(false);

    ui->label_type_value->setText(tr("No selection"));
    ui->pushButton_Set_Values->setEnabled(false);
    ui->pushButton_Add_attribute->setEnabled(false);
    ui->pushButton_Add_son->setEnabled(false);
    ui->pushButton_Remove_attribute->setEnabled(false);
    ui->pushButton_Cancel_Values->setEnabled(false);
    ui->pushButton_Remove_Node->setEnabled(false);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::ClearStack()
{
    ChangesStackElement * tmp;
    while (changeStackRoot != nullptr)
    {
        if (changeStackRoot->newText != nullptr)
        {
            delete changeStackRoot->newText;
        }
        if (changeStackRoot->newValue != nullptr)
        {
            delete changeStackRoot->newValue;
        }
        tmp=changeStackRoot;
        changeStackRoot=changeStackRoot->next;
        delete tmp;
    }
    changeStackLast=nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::BaseSelectionChanged(int value)
{
    QDomNode newbase;

    int index;

    if (this->treeChange == true)
    {//Changes have been made : warn ?
    }
    this->ClearStack();
    ui->pushButton_Apply_Changes->setEnabled(false);

    index = ui->comboBox_Base_Selection->itemData(value).toInt();  //.convert(QVariant::Int);
    SCASSERT(value < rootBasesNum);
    // QMessageBox::information(this, "test", QString("%1:%2").arg(value).arg(index));

    // Clear all tree info and nodes
    // TODO : Check
    while (rootNode->rowCount() > 0)
    {
        rootNode->removeRow(0);
    }
    this->xmlmodel->ClearTree();

    VXMLTreeElement* standard_base = new VXMLTreeElement(
                rootBases[index]->GettreeNodeName(),
                VXMLTreeElement::TypeRoot,
                rootBases[index]->GetDocNode(),
                false);
    rootNode->appendRow(standard_base);
    ReadNodes(rootBases[index]->GetDocNode(), standard_base, xmlmodel, true);

    ui->treeView_main->setModel(xmlmodel);
    this->ClearEditData();
    // TODO : warn if changes ?
    this->ClearStack();
    return;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::ElementClicked ( const QModelIndex & index )
{

    // Get item (function returns parent of clicked item)
    VXMLTreeElement *item = static_cast<VXMLTreeElement *> (index.internalPointer());

    // Get child specified by index row/column
    VXMLTreeElement * item2 = static_cast<VXMLTreeElement *> (item->child(index.row(), index.column()));

    // Clear all data and disable buttons
    this->ClearEditData();
    // Check if element is selectable at this time
    if (item2->IsSelectable() == false)
    {
        return;
    }
    // Check status of element in change stack
    this->currentNodeEdited=item2;
    this->currentNodeEditedStatus=0;
    ChangesStackElement * local = this->changeStackRoot;
    while (local != nullptr)
    {
        if (local->element == item2)
        {
            this->currentNodeEditedStatus=local->type;
            this->currentNodeEditedStack=local;
            if (local->type == DialogPatternXmlEdit::ChangeTypeDelete)
            {
                // Node is deleted, only enable cancel button
                ui->pushButton_Cancel_Values->setEnabled(true);
                return;
            }
        }
        local=local->next;
    }
    // Fill the dialog and  Activate the line edit
    if (item2->GetelementType() != VXMLTreeElement::TypeRoot)
    {
        ui->lineEdit_Name->setText(currentNodeEdited->GettreeNodeName());
        ui->lineEdit_Name->setEnabled(true);
        // Fill value
        ui->lineEdit_Value->setText(currentNodeEdited->GettreeNodeValue());
        ui->lineEdit_Value->setEnabled(true);
    }
    if (item2->GetelementType() == VXMLTreeElement::TypeRoot)
    {
        ui->label_type_value->setText(tr("Root node"));;
        ui->pushButton_Add_son->setEnabled(true);
    }
    else if (item2->GetelementType() == VXMLTreeElement::TypeNode)
    {
        ui->label_type_value->setText(tr("Node"));
        ui->pushButton_Add_attribute->setEnabled(true);
        ui->pushButton_Add_son->setEnabled(true);
        ui->pushButton_Remove_Node->setEnabled(true);
    }
    else if (item2->GetelementType() == VXMLTreeElement::TypeAttr)
    {
        ui->label_type_value->setText(tr("Attribute"));
        ui->pushButton_Remove_attribute->setEnabled(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
DialogPatternXmlEdit::~DialogPatternXmlEdit()
{
    this->ClearStack();
    this->xmlmodel->ClearTree();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternXmlEdit::ReadNodes(QDomNode dNode, VXMLTreeElement* root, VXMLTreeView* xmlmodel, bool refresh)
{

    QDomNode tNode, tNode2;
    VXMLTreeElement* tElement, *tElement2;
    int totalOfChilds = dNode.childNodes().size();
    if (totalOfChilds == 0)
    {
        return; // empty xml
    }
    for (int i=0;i<totalOfChilds;i++)
    {
        tNode = dNode.childNodes().at(i);
        if (tNode.nodeType() != QDomNode::CommentNode)
        {
            tElement = new VXMLTreeElement(tNode.nodeName(), VXMLTreeElement::TypeNode, tNode, false);
            xmlmodel->appendchain(tElement);
            root->appendRow(tElement);
            if (tNode.nodeName() == "draw")
            {
                for (int i = 0; i < tNode.attributes().size(); i++)
                {
                    if ((tNode.attributes().item(i).nodeName() == "name")
                            && (refresh == false))
                    {
                        ui->comboBox_Base_Selection->addItem(tNode.attributes().item(i).nodeValue(),
                                                             QVariant(rootBasesNum));
                        rootBases[rootBasesNum]=new VXMLTreeElement(
                                    QString("Drawing %1").arg(tNode.attributes().item(i).nodeValue()),
                                    VXMLTreeElement::TypeRoot, tNode, false);
                        rootBasesNum++;
                        //QStandardItem* rootNode = (QStandardItem*) rootBases[rootBasesNum]->invisibleRootItem();
                        //rootNode->appendRow(tElement);
                    }
                }
            }
        }
    }

    do
    {
        tElement=xmlmodel->getCurrent()->elmt;
        tNode=tElement->GetDocNode();
        if (tElement->GetelementType() == VXMLTreeElement::TypeNode)
        {
            totalOfChilds = tNode.childNodes().size();
            for (int i=0;i<totalOfChilds;i++)
            {
                tNode2 = tNode.childNodes().at(i);
                if (tNode2.nodeType() != QDomNode::CommentNode)
                {
                    tElement2 = new VXMLTreeElement(tNode2.nodeName(), VXMLTreeElement::TypeNode, tNode2, false);
                    xmlmodel->appendchain(tElement2);
                    tElement->appendRow(tElement2);
                }
            }

            if (tNode.nodeType() == QDomNode::TextNode)
            {
                tElement->SetTreeNodeValue(tNode.nodeValue());
            }

            for (int i = 0; i < tNode.attributes().size(); i++)
            {
                tElement2 = new VXMLTreeElement(tNode.attributes().item(i).nodeName(), VXMLTreeElement::TypeAttr,
                            tNode, false);
                tElement2->SetTreeNodeValue(tNode.attributes().item(i).nodeValue());
                tElement->appendRow(tElement2);
                xmlmodel->appendchain(tElement2);
            }
        }
        xmlmodel->SetCurrent(xmlmodel->getCurrent()->next);
    }
    while (xmlmodel->getCurrent() != 0);
    return;
}

//---------------------------------------------------------------------------------------------------------------------
VXMLTreeView::VXMLTreeView(QObject *parent)
    : QStandardItemModel(parent), items(nullptr), current(nullptr), last(nullptr)
{}

//---------------------------------------------------------------------------------------------------------------------
void VXMLTreeView::ClearTree()
{

    // TODO check if treeitems are deleted with the delete row command of QDom

    //clear the chain link
    TreeElementchain * tmp;
    while (items != nullptr)
    {
        tmp=items;
        items=items->next;
        delete tmp;
    }
    // reset chain counters
    current=last=items=nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
VXMLTreeView::~VXMLTreeView()
{}

//---------------------------------------------------------------------------------------------------------------------
void VXMLTreeView::appendchain(VXMLTreeElement* elmt)
{
    if (last == nullptr)
    { // first element
        current = new TreeElementchain;
        if (current == nullptr)
        {
            SCASSERT(current != nullptr);
            // TODO : throw exception
        }
        current->elmt=elmt;
        current->next=nullptr;
        last=items=current;
        return;
    }
    TreeElementchain* temp= new TreeElementchain;
    if (temp == nullptr)
    {
        SCASSERT(temp != nullptr);
        // TODO : throw exception
    }
    temp->elmt=elmt;
    temp->next=nullptr;
    last->next=temp;
    last=temp;
}

const short int VXMLTreeElement::TypeNode=1;
const short int VXMLTreeElement::TypeAttr=2;
const short int VXMLTreeElement::TypeRoot=3;

//---------------------------------------------------------------------------------------------------------------------
//TODO warning: conversion to 'short int' from 'int' may alter its value [-Wconversion]
//treeNodeValueSet(false), treeNodeName(name)
//                                         ^
VXMLTreeElement::VXMLTreeElement(QString name, short int nodetype, QDomNode source, bool editor)
    : QStandardItem(name), DocNode(source), parentnode(nullptr), addedNode(editor), elementType(nodetype),
      treeNodeValue("<empty>"), treeNodeValueSet(false), treeNodeName(name), selectable(true)
{
    if (editor == false)
    {
        this->DocNode=source;
    }
    this->setText(this->DisplayText());
    switch (this->elementType)
    {
        case VXMLTreeElement::TypeAttr:
            this->setBackground(BACKGROUND_COLOR_ATTRIBUTE);
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
VXMLTreeElement::~VXMLTreeElement()
{}

//---------------------------------------------------------------------------------------------------------------------
void VXMLTreeElement::SetTreeNodeValue(QString value)
{
    this->treeNodeValue=value;
    this->treeNodeValueSet=true;
    this->setText(this->DisplayText());
}

//---------------------------------------------------------------------------------------------------------------------
void VXMLTreeElement::SetTreeNodeName(QString value)
{
    this->treeNodeName=value;
    this->setText(this->DisplayText());
}

//---------------------------------------------------------------------------------------------------------------------
QString VXMLTreeElement::DisplayText()
{
    if (this->elementType == VXMLTreeElement::TypeRoot)
    {
        return this->treeNodeName;
    }
    return QString("%1 : %2").arg((this->treeNodeName)).arg(this->treeNodeValue);
}

//---------------------------------------------------------------------------------------------------------------------
QString VXMLTreeElement::GettreeNodeValue()
{
    // Only return value if it's really set.
    if (this->treeNodeValueSet)
    {
        return this->treeNodeValue;
    }
    else
    {
        return QString();
    }
}
