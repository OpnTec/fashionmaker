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


#include "dialogpatternxmledit.h"
#include "ui_dialogpatternxmledit.h"


const short int DialogPatternXmlEdit::ChangeTypeDelete=1;
const short int DialogPatternXmlEdit::ChangeTypeAdd=2;
const short int DialogPatternXmlEdit::ChangeTypeModify=3;

DialogPatternXmlEdit::DialogPatternXmlEdit(QWidget *parent,VPattern *xmldoc):QDialog(parent),ui(new Ui::DialogPatternXmlEdit)
{
    ui->setupUi(this);

    xmlmodel = new VXMLTreeView();
    currentNodeEdited=0;
    treeChange=false;
    this->changeStackRoot = this->changeStackLast =0;

    doc=xmldoc;
    root = doc->documentElement();
    // disable edit trigers :
    //ui->treeView_main->editTriggers(QTreeView::NoEditTriggers);
    ui->treeView_main->setEditTriggers(QTreeView::NoEditTriggers);

    ui->treeView_main->setWindowTitle("XML");


    rootNode = (QStandardItem*) xmlmodel->invisibleRootItem();

    qint16 drawnum=xmldoc->elementsByTagName("draw").size();
    rootBases = new VXMLTreeElement*[drawnum+1];
    rootBasesNum=1;
    rootBases[0]=new VXMLTreeElement("Valentina",VXMLTreeElement::TypeRoot,root,false);
    // TODO : OOM exception.


    VXMLTreeElement* standard_base = new VXMLTreeElement("Valentina",VXMLTreeElement::TypeRoot,root,false);
    ui->comboBox_Base_Selection->addItem(tr("All drawings"),QVariant(0));

    //rootBases[0]=(QDomNode) doc->DocumentNode;
    rootNode->appendRow(standard_base);

    ReadNodes(root,standard_base,xmlmodel,false);

    ui->treeView_main->setModel(xmlmodel);

    //ui->treeView_main->expandAll();

    // connectors
    connect(ui->comboBox_Base_Selection,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&DialogPatternXmlEdit::BaseSelectionChanged);
    connect(ui->treeView_main,&QTreeView::clicked,this,&DialogPatternXmlEdit::ElementClicked);
    connect(ui->lineEdit_Name,&QLineEdit::textEdited,this,&DialogPatternXmlEdit::NameTextEdited);
    connect(ui->lineEdit_Value,&QLineEdit::textEdited,this,&DialogPatternXmlEdit::ValueTextEdited);
    connect(ui->pushButton_Set_Values,&QPushButton::clicked,this,&DialogPatternXmlEdit::ButtonSetClicked);
    connect(ui->pushButton_Cancel_Values,&QPushButton::clicked,this,&DialogPatternXmlEdit::ButtonCancelClicked);
    connect(ui->pushButton_Remove_attribute,&QPushButton::clicked,this,&DialogPatternXmlEdit::ButtonDeleteAttributeClicked);
    connect(ui->pushButton_Add_son,&QPushButton::clicked,this,&DialogPatternXmlEdit::ButtonAddSonClicked);
    connect(ui->pushButton_Add_attribute,&QPushButton::clicked,this,&DialogPatternXmlEdit::ButtonAddAttributeClicked);
    connect(ui->pushButton_Apply_Changes,&QPushButton::clicked,this,&DialogPatternXmlEdit::ButtonApplyChangesClicked);
}

DialogPatternXmlEdit::ChangesStackElement* DialogPatternXmlEdit::CreateStackElement(short int typechange)
{
    ChangesStackElement* tmp;

    tmp= new ChangesStackElement;
    //if (tmp==0) TODO
    tmp->type=typechange;
    tmp->changedText=tmp->changedValue=false;

    if (changeStackLast==0)
    {
        changeStackLast=changeStackRoot=tmp;
    }
    else
    {
        changeStackLast->next=tmp;
        changeStackLast=changeStackLast->next;
    }
    changeStackLast->newValue=changeStackLast->newText=0;
    changeStackLast->next=0;
    return changeStackLast;
}

void DialogPatternXmlEdit::ButtonApplyChangesClicked()
{
    QString Changes="";
    ChangesStackElement* currentChange;
    if (this->changeStackRoot==0)
    {
        Changes="No Changes";
    }
    else
    {
        currentChange=this->changeStackRoot;
        do
        {
            if (currentChange->type == DialogPatternXmlEdit::ChangeTypeAdd)
            {
                Changes += QString("Added type %1 : ").arg(currentChange->element->getelementType());
                Changes += currentChange->element->gettreeNodeName();
                Changes += "/";
                Changes += (currentChange->element->gettreeNodeValueSet()) ?
                            currentChange->element->gettreeNodeValue(): QString(tr("<no value>"));
                Changes += "\n";
            }
            else
            {
                if (currentChange->type == DialogPatternXmlEdit::ChangeTypeModify)
                {
                    Changes += QString("Modified type %1 : ").arg(currentChange->element->getelementType());
                    Changes += (currentChange->changedText) ? *currentChange->newText : QString(tr("Unchanged"));
                    Changes += "/";
                    Changes += (currentChange->changedValue) ? *currentChange->newValue: QString(tr("Unchanged"));
                    Changes += "\n";
                }
                else
                {
                    if (currentChange->type == DialogPatternXmlEdit::ChangeTypeDelete)
                    {
                        Changes += QString("Deleted type %1 : ").arg(currentChange->element->getelementType());
                        Changes += currentChange->element->gettreeNodeName();
                        Changes += "/";
                        Changes += (currentChange->element->gettreeNodeValueSet()) ?
                                    currentChange->element->gettreeNodeValue(): QString(tr("<no value>"));
                        Changes += "\n";
                     }
                }
            }
            currentChange=currentChange->next;
        } while (currentChange != 0);
    }
    QMessageBox::information(this, "Changes (not REALLY applied for now)", Changes);
    // TODO : clear stack and apply
}

void DialogPatternXmlEdit::ButtonCancelClicked()
{
   this->ClearEditData();
   if (this->currentNodeEditedStatus==DialogPatternXmlEdit::ChangeTypeDelete)
   {
       if (this->currentNodeEdited->getelementType() == VXMLTreeElement::TypeAttr)
       {
           // Attribute previously deleted -> resore
           QFont textfont=currentNodeEdited->font();
           textfont.setStrikeOut(false);
           this->currentNodeEdited->setFont(textfont);
           this->RemoveChangeStackElement(this->currentNodeEditedStack);
       }
       else
       {
           QMessageBox::information(this, "Not Implemented", "Not Implemented");
       }
   }
}

void DialogPatternXmlEdit::RemoveChangeStackElement(ChangesStackElement* elmt)
{
    ChangesStackElement* index = this->changeStackRoot;

    if (changeStackRoot == elmt)
    {
        if (elmt->newText != 0)
        {
            delete elmt->newText;
        }
        if (elmt->newValue != 0)
        {
            delete elmt->newValue;
        }
        this->changeStackRoot = elmt->next;
        if (this->changeStackLast == elmt)
        {
            this->changeStackLast= elmt->next;
        }
        delete elmt;
        return;
    }
    while (index->next!=0) {
        if (index->next == elmt)
        {
            break;
        }
        index=index->next;
    }
    if (index->next == 0)
    {
        // TODO : debug error here
        return;
    }
    if (index->next->newText != 0)
    {
        delete index->next->newText;
    }
    if (index->next->newValue != 0)
    {
        delete index->next->newValue;
    }
    index->next=index->next->next;
    if (this->changeStackLast == elmt)
    {
        this->changeStackLast= index;
    }
    delete elmt;
}

void DialogPatternXmlEdit::ButtonDeleteAttributeClicked()
{
    ChangesStackElement* newstack;

    // TODO : maybe assert functions here
    if (this->currentNodeEdited == 0)
    {
        return;
    }
    if (this->currentNodeEdited->getelementType() != VXMLTreeElement::TypeAttr)
    {
        return;
    }

    newstack = this->CreateStackElement(DialogPatternXmlEdit::ChangeTypeDelete);
    newstack->element= this->currentNodeEdited;

    QFont textfont=currentNodeEdited->font();
    textfont.setStrikeOut(true);
    this->currentNodeEdited->setFont(textfont);
    // Activate changes
    this->treeChange=true;
    ui->pushButton_Apply_Changes->setEnabled(true);
    // clear dialog
    this->ClearEditData();
}

void DialogPatternXmlEdit::ButtonAddSonClicked()
{
    ChangesStackElement* newstack;
    bool ok;
    //clear_edit_data();
    // TODO : maybe assert functions here
    if (this->currentNodeEdited == 0)
    {
        return;
    }
    if (this->currentNodeEdited->getelementType() != VXMLTreeElement::TypeNode)
    {
        return;
    }


    QString name = QInputDialog::getText(this, tr("Node Name"),
                                                tr("Name:"), QLineEdit::Normal,
                                                "", &ok);
    if (ok==false)
    {
        return;
    }
    QString value = QInputDialog::getText(this, tr("Node Value (may be empty)"),
                                                tr("Value:"), QLineEdit::Normal,
                                                "", &ok);
    if (ok==false)
    {
        return;
    }

    // create element tree but do not add attribute in DOM (will be done in apply).
    QDomNode empty;
    VXMLTreeElement* tElement = new VXMLTreeElement(
                name,
                VXMLTreeElement::TypeNode,
                empty,true);

    if (!value.isEmpty())
    {
        tElement->setTreeNodeValue(value);
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
}

void DialogPatternXmlEdit::ButtonAddAttributeClicked()
{
    ChangesStackElement* newstack;
    bool ok;
    QDomNode empty; // empty QNode to pas to create function : maybe a better way to do this ?

    // TODO : maybe assert functions here
    if (this->currentNodeEdited == 0)
    {
        return;
    }
    if (this->currentNodeEdited->getelementType() != VXMLTreeElement::TypeNode) return;

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
    VXMLTreeElement* tElement = new VXMLTreeElement(
                name,
                VXMLTreeElement::TypeAttr,
                empty,true);

    tElement->setTreeNodeValue(value);
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
}

void DialogPatternXmlEdit::ButtonSetClicked()
{
    QString name,value;
    ChangesStackElement* newstack;

    // TODO : check if a change has already been done
    if (this->currentNodeEdited == 0) return;
    name=ui->lineEdit_Name->text();
    value=ui->lineEdit_Value->text();
    bool nameedit,valueedit;

    nameedit = (name != currentNodeEdited->gettreeNodeName()) ? true : false;
    valueedit= (value != currentNodeEdited->gettreeNodeValue()) ? true : false;

    if ( nameedit == true  || valueedit == true )
    {
        newstack = this->CreateStackElement(DialogPatternXmlEdit::ChangeTypeModify);
        newstack->element= this->currentNodeEdited;

        if (nameedit == true)
        {
            newstack->changedText=true;
            newstack->newText=new QString(name);
            currentNodeEdited->setTreeNodeName(name);
        }
        if (valueedit == true)
        {
            newstack->changedValue=true;
            newstack->newValue= new QString(value);
            currentNodeEdited->setTreeNodeValue(value);
        }

        QFont textfont=currentNodeEdited->font();
        textfont.setBold(true);
        currentNodeEdited->setFont(textfont);
        treeChange=true;
        ui->pushButton_Apply_Changes->setEnabled(true);
        ClearEditData();
    }
}

void DialogPatternXmlEdit::NameTextEdited(QString newtext)
{
    //QMessageBox::information(this, "nameTextEdited", QString("%1").arg(newtext));
    ui->pushButton_Set_Values->setEnabled(true);
    ui->pushButton_Cancel_Values->setEnabled(true);
}

void DialogPatternXmlEdit::ValueTextEdited(QString newtext)
{
    //QMessageBox::information(this, "valueTextEdited", QString("%1").arg(newtext));
    ui->pushButton_Set_Values->setEnabled(true);
    ui->pushButton_Cancel_Values->setEnabled(true);
}

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
}

void DialogPatternXmlEdit::ClearStack()
{
    ChangesStackElement * tmp;
    while (changeStackRoot != 0)
    {
        if (changeStackRoot->newText != 0)
        {
            delete changeStackRoot->newText;
        }
        if (changeStackRoot->newValue != 0)
        {
            delete changeStackRoot->newValue;
        }
        tmp=changeStackRoot;
        changeStackRoot=changeStackRoot->next;
        delete tmp;
    }
    changeStackLast=0;
}

void DialogPatternXmlEdit::BaseSelectionChanged(int value)
{
    QDomNode newbase;

    int index;

    if (this->treeChange == true)
    {//Changes have been made : warn ?
    }
    ClearStack();
    ui->pushButton_Apply_Changes->setEnabled(false);

    index = ui->comboBox_Base_Selection->itemData(value).toInt();  //.convert(QVariant::Int);
    Q_ASSERT(value < rootBasesNum);
    // QMessageBox::information(this, "test", QString("%1:%2").arg(value).arg(index));

    // Clear all tree info and nodes
    // TODO : Check
    while (rootNode->rowCount() > 0)
    {
        rootNode->removeRow(0);
    }
    xmlmodel->ClearTree();

    VXMLTreeElement* standard_base = new VXMLTreeElement(
                rootBases[index]->gettreeNodeName(),
                VXMLTreeElement::TypeRoot,
                rootBases[index]->getDocNode(),
                false);
    rootNode->appendRow(standard_base);
    ReadNodes(rootBases[index]->getDocNode(),standard_base,xmlmodel,true);

    ui->treeView_main->setModel(xmlmodel);
    this->ClearEditData();
    // TODO : warn if changes ?
    this->ClearStack();
    return;
}

void DialogPatternXmlEdit::ElementClicked ( const QModelIndex & index )
{

    // Get item (function returns parent of clicked item)
    VXMLTreeElement *item = (VXMLTreeElement *)index.internalPointer();

    // Get child specified by index row/column
    VXMLTreeElement * item2 = (VXMLTreeElement *) item->child(index.row(),index.column());

    // Clear all data and disable buttons
    this->ClearEditData();
    // Check status of element in change stack
    this->currentNodeEdited=item2;
    this->currentNodeEditedStatus=0;
    DialogPatternXmlEdit::ChangesStackElement * local = this->changeStackRoot;
    while (local != 0)
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
    if (item2->getelementType() != VXMLTreeElement::TypeRoot)
    {
        ui->lineEdit_Name->setText(currentNodeEdited->gettreeNodeName());
        ui->lineEdit_Name->setEnabled(true);
        // Fill value
        ui->lineEdit_Value->setText(currentNodeEdited->gettreeNodeValue());
        ui->lineEdit_Value->setEnabled(true);
    }
    if (item2->getelementType() == VXMLTreeElement::TypeRoot)
    {
        ui->label_type_value->setText(tr("Root node"));;
        ui->pushButton_Add_son->setEnabled(true);
    }
    else if  (item2->getelementType() == VXMLTreeElement::TypeNode)
    {
        ui->label_type_value->setText(tr("Node"));
        ui->pushButton_Add_attribute->setEnabled(true);
        ui->pushButton_Add_son->setEnabled(true);
    }
    else if (item2->getelementType() == VXMLTreeElement::TypeAttr)
    {
        ui->label_type_value->setText(tr("Attribute"));
        ui->pushButton_Remove_attribute->setEnabled(true);
    }
}

DialogPatternXmlEdit::~DialogPatternXmlEdit()
{
    //xmlmodel->clearTree();
    delete ui;
    ChangesStackElement * tmp;
    while (changeStackRoot != 0)
    {
        tmp=changeStackRoot->next;
        delete changeStackRoot;
        changeStackRoot=tmp;
    }
}

void DialogPatternXmlEdit::ReadNodes(QDomNode dNode, VXMLTreeElement* root,VXMLTreeView* xmlmodel, bool refresh)
{

    QDomNode tNode,tNode2;
    VXMLTreeElement* tElement,*tElement2;
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
            tElement = new VXMLTreeElement(tNode.nodeName(),VXMLTreeElement::TypeNode,tNode,false);
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
                                    VXMLTreeElement::TypeRoot,tNode,false);;
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
        tNode=tElement->getDocNode();
        if (tElement->getelementType() == VXMLTreeElement::TypeNode)
        {
            totalOfChilds = tNode.childNodes().size();
            for (int i=0;i<totalOfChilds;i++)
            {
                tNode2 = tNode.childNodes().at(i);
                if (tNode2.nodeType() != QDomNode::CommentNode)
                {
                    tElement2 = new VXMLTreeElement(tNode2.nodeName(),VXMLTreeElement::TypeNode,tNode2,false);
                    xmlmodel->appendchain(tElement2);
                    tElement->appendRow(tElement2);
                }
            }

            if (tNode.nodeType() == QDomNode::TextNode)
            {
                tElement->setTreeNodeValue(tNode.nodeValue());
            }

            for (int i = 0; i < tNode.attributes().size(); i++)
            {
                tElement2 = new VXMLTreeElement(
                            tNode.attributes().item(i).nodeName(),
                            VXMLTreeElement::TypeAttr,
                            tNode,false);
                tElement2->setTreeNodeValue(tNode.attributes().item(i).nodeValue());
                tElement->appendRow(tElement2);
                xmlmodel->appendchain(tElement2);
            }
        }
        xmlmodel->SetCurrent(xmlmodel->getCurrent()->next);
    }
    while (xmlmodel->getCurrent() != 0);
    return;
}

// vXMLTreeView Code  -----------------------------------------------------------------
VXMLTreeView::VXMLTreeView(QObject *parent) : QStandardItemModel(parent)
{
    current=last=items=0;
}

void VXMLTreeView::ClearTree() {

    // TODO check if treeitems are deleted with the delete row command of QDom

    //clear the chain link
    TreeElementchain * tmp;
    while (items != 0)
    {
        tmp=items;
        items=items->next;
        delete tmp;
    }
    // reset chain counters
    current=last=items=0;

}

VXMLTreeView::~VXMLTreeView()
{

}

void VXMLTreeView::appendchain(VXMLTreeElement* elmt)
{
    if (last == 0)
    { // first element
        current = new TreeElementchain;
        if (current == 0)
        {
            Q_ASSERT(current != 0);
            // TODO : throw exception
        }
        current->elmt=elmt;
        current->next=0;
        last=items=current;
        return;
    }
    TreeElementchain* temp= new TreeElementchain;
    if (temp == 0)
    {
        Q_ASSERT(temp != 0);
        // TODO : throw exception
    }
    temp->elmt=elmt;
    temp->next=0;
    last->next=temp;
    last=temp;
}


// vXMLTreeElement code -----------------------------------------------------------------

const short int VXMLTreeElement::TypeNode=1;
const short int VXMLTreeElement::TypeAttr=2;
const short int VXMLTreeElement::TypeRoot=3;


VXMLTreeElement::VXMLTreeElement(QString name, int nodetype, QDomNode source,bool editor): QStandardItem(name)
{
    this->elementType=nodetype;
    this->DocNode=source;
    this->addedNode=editor;
    if (!editor)
    {
        this->DocNode=source;
    }
    this->treeNodeName=name;
    this->treeNodeValue="<empty>"; // TODO : translation ?
    this->treeNodeValueSet=false,
            this->setText(this->displayText());
    switch (this->elementType)
    {
    case VXMLTreeElement::TypeAttr:
        this->setBackground(BACKGROUND_COLOR_ATTRIBUTE);
        break;
    default:
        break;
    }
}


VXMLTreeElement::~VXMLTreeElement()
{

}

void VXMLTreeElement::setTreeNodeValue(QString value)
{
    this->treeNodeValue=value;
    this->treeNodeValueSet=true;
    this->setText(this->displayText());
}

void VXMLTreeElement::setTreeNodeName(QString value)
{
    this->treeNodeName=value;
    this->setText(this->displayText());
}

QString VXMLTreeElement::displayText()
{
    if (this->elementType == VXMLTreeElement::TypeRoot)
        return this->treeNodeName;
    return QString("%1 : %2").arg((this->treeNodeName)).arg(this->treeNodeValue);
}

QString VXMLTreeElement::gettreeNodeValue() {
    // Only return value if it's really set.
    if (this->treeNodeValueSet)
        return this->treeNodeValue;
    else
        return "";
}

