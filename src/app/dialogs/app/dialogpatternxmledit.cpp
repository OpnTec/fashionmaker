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

DialogPatternXmlEdit::DialogPatternXmlEdit(QWidget *parent,VPattern *xmldoc) :
    QDialog(parent),
    ui(new Ui::DialogPatternXmlEdit)
{
    ui->setupUi(this);

    xmlmodel = new vXMLTreeView();
    current_node_edited=0;
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
    rootBases = (vXMLTreeElement**)malloc(sizeof(vXMLTreeElement*)*(drawnum+1));
    rootBasesNum=1;
    rootBases[0]=new vXMLTreeElement("Valentina",vXMLTreeElement::TypeRoot,root,false);
    // TODO : OOM exception.


    vXMLTreeElement* standard_base = new vXMLTreeElement("Valentina",vXMLTreeElement::TypeRoot,root,false);
    ui->comboBox_Base_Selection->addItem(tr("All drawings"),QVariant(0));

    //rootBases[0]=(QDomNode) doc->DocumentNode;
    rootNode->appendRow(standard_base);

    readNodes(root,standard_base,xmlmodel,false);

    ui->treeView_main->setModel(xmlmodel);

    //ui->treeView_main->expandAll();

    // connectors
    connect(ui->comboBox_Base_Selection,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &DialogPatternXmlEdit::baseSelectionChanged);
    connect(ui->treeView_main,
            &QTreeView::clicked,
            this,
            &DialogPatternXmlEdit::Element_clicked);
    connect(ui->lineEdit_Name,
            &QLineEdit::textEdited,
            this,
            &DialogPatternXmlEdit::nameTextEdited
            );
    connect(ui->lineEdit_Value,
            &QLineEdit::textEdited,
            this,
            &DialogPatternXmlEdit::valueTextEdited
            );
    connect(ui->pushButton_Set_Values,
            &QPushButton::clicked,
            this,
            &DialogPatternXmlEdit::ButtonSetClicked);
    connect(ui->pushButton_Cancel_Values,
            &QPushButton::clicked,
            this,
            &DialogPatternXmlEdit::ButtonCancelClicked);
    connect(ui->pushButton_Remove_attribute,
            &QPushButton::clicked,
            this,
            &DialogPatternXmlEdit::ButtonDeleteAttributeClicked);
    connect(ui->pushButton_Add_son,
            &QPushButton::clicked,
            this,
            &DialogPatternXmlEdit::ButtonAddSonClicked);
    connect(ui->pushButton_Add_attribute,
            &QPushButton::clicked,
            this,
            &DialogPatternXmlEdit::ButtonAddAttributeClicked);
    connect(ui->pushButton_Apply_Changes,
            &QPushButton::clicked,
            this,
            &DialogPatternXmlEdit::ButtonApplyChangesClicked);
}

DialogPatternXmlEdit::ChangesStackElement* DialogPatternXmlEdit::CreateStackElement(short int typechange) {
    ChangesStackElement* tmp;

    tmp= (ChangesStackElement* ) malloc(sizeof(ChangesStackElement));
    //if (tmp==0) TODO
    tmp->type=typechange;
    tmp->changedText=tmp->changedValue=false;

    if (changeStackLast==0) {
        changeStackLast=changeStackRoot=tmp;
    } else {
        changeStackLast->next=tmp;
        changeStackLast=changeStackLast->next;
    }
    changeStackLast->newValue=changeStackLast->newText=0;
    changeStackLast->next=0;
    return changeStackLast;
}

void DialogPatternXmlEdit::ButtonApplyChangesClicked() {
    QString Changes="";
    ChangesStackElement* currentChange;
    if (this->changeStackRoot==0) Changes="No Changes";
    else {
        currentChange=this->changeStackRoot;
        do {
            if (currentChange->type == DialogPatternXmlEdit::ChangeTypeAdd) {
                Changes += QString("Added type %1 : ").arg(currentChange->element->getelementType());
                Changes += currentChange->element->gettreeNodeName();
                Changes += "/";
                Changes += (currentChange->element->gettreeNodeValueSet()) ?
                            currentChange->element->gettreeNodeValue(): QString(tr("<no value>"));
                Changes += "\n";
            } else {
                if (currentChange->type == DialogPatternXmlEdit::ChangeTypeModify) {
                    Changes += QString("Modified type %1 : ").arg(currentChange->element->getelementType());
                    Changes += (currentChange->changedText) ? *currentChange->newText : QString(tr("Unchanged"));
                    Changes += "/";
                    Changes += (currentChange->changedValue) ? *currentChange->newValue: QString(tr("Unchanged"));
                    Changes += "\n";
                } else {
                    if (currentChange->type == DialogPatternXmlEdit::ChangeTypeDelete) {
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

void DialogPatternXmlEdit::ButtonCancelClicked(){
   this->clear_edit_data();
   if (this->current_node_edited_status=DialogPatternXmlEdit::ChangeTypeDelete) {
       if (this->current_node_edited->getelementType() == vXMLTreeElement::TypeAttr) {
           // Attribute previously deleted -> resore
           QFont textfont=current_node_edited->font();
           textfont.setStrikeOut(false);
           this->current_node_edited->setFont(textfont);
           this->removeChangeStackElement(this->current_node_edited_stack);
       } else {
           QMessageBox::information(this, "Not Implemented", "Not Implemented");
       }
   }
}

void DialogPatternXmlEdit::removeChangeStackElement(ChangesStackElement* elmt) {
    ChangesStackElement* index = this->changeStackRoot;

    if (changeStackRoot == elmt) {
        if (elmt->newText != 0) free (elmt->newText);
        if (elmt->newValue != 0) free (elmt->newValue);
        this->changeStackRoot = elmt->next;
        if (this->changeStackLast == elmt) { this->changeStackLast= elmt->next; }
        free(elmt);
        return;
    }
    while (index->next!=0) {
        if (index->next == elmt) {
            break;
        }
        index=index->next;
    }
    if (index->next == 0) {
        // TODO : debug error here
        return;
    }
    if (index->next->newText != 0) free (index->next->newText);
    if (index->next->newValue != 0) free (index->next->newValue);
    index->next=index->next->next;
    if (this->changeStackLast == elmt) { this->changeStackLast= index;}
    free(elmt);
}

void DialogPatternXmlEdit::ButtonDeleteAttributeClicked(){
    ChangesStackElement* newstack;

    // TODO : maybe assert functions here
    if (this->current_node_edited == 0) return;
    if (this->current_node_edited->getelementType() != vXMLTreeElement::TypeAttr) return;

    newstack = this->CreateStackElement(DialogPatternXmlEdit::ChangeTypeDelete);
    newstack->element= this->current_node_edited;

    QFont textfont=current_node_edited->font();
    textfont.setStrikeOut(true);
    this->current_node_edited->setFont(textfont);
    // Activate changes
    this->treeChange=true;
    ui->pushButton_Apply_Changes->setEnabled(true);
    // clear dialog
    this->clear_edit_data();
}

void DialogPatternXmlEdit::ButtonAddSonClicked(){
    ChangesStackElement* newstack;
    //clear_edit_data();
    // TODO : maybe assert functions here
    if (this->current_node_edited == 0) return;
    if (this->current_node_edited->getelementType() != vXMLTreeElement::TypeNode) return;

    bool ok;
    QString name = QInputDialog::getText(this, tr("Node Name"),
                                                tr("Name:"), QLineEdit::Normal,
                                                "", &ok);
    if (!ok) return;
    QString value = QInputDialog::getText(this, tr("Node Value (may be empty)"),
                                                tr("Value:"), QLineEdit::Normal,
                                                "", &ok);
    if (!ok) return;

    // create element tree but do not add attribute in DOM (will be done in apply).
    QDomNode empty;
    vXMLTreeElement* tElement = new vXMLTreeElement(
                name,
                vXMLTreeElement::TypeNode,
                empty,true);

    if (value !="") tElement->setTreeNodeValue(value);
    current_node_edited->appendRow(tElement);
    xmlmodel->appendchain(tElement);
    QFont textfont=current_node_edited->font();
    textfont.setBold(true);
    tElement->setFont(textfont);

    // Add to change stack
    newstack = this->CreateStackElement(DialogPatternXmlEdit::ChangeTypeAdd);
    newstack->element=tElement;
    newstack->newText=new QString(name); newstack->changedText=true;
    if (value !="") { newstack->changedValue=true;newstack->newValue=new QString(value);}
    // Activate changes
    treeChange=true;
    ui->pushButton_Apply_Changes->setEnabled(true);
}

void DialogPatternXmlEdit::ButtonAddAttributeClicked(){
    ChangesStackElement* newstack;
    bool ok;
    QDomNode empty; // empty QNode to pas to create function : maybe a better way to do this ?

    // TODO : maybe assert functions here
    if (this->current_node_edited == 0) return;
    if (this->current_node_edited->getelementType() != vXMLTreeElement::TypeNode) return;

    QString name = QInputDialog::getText(this, tr("Attribute Name"),
                                                tr("Name:"), QLineEdit::Normal,
                                                "", &ok);
    if (!ok) return;
    QString value = QInputDialog::getText(this, tr("Attribute Value"),
                                                tr("Value:"), QLineEdit::Normal,
                                                "", &ok);
    if (!ok) return;

    // create element tree but do not add attribute in DOM (will be done in apply).
    vXMLTreeElement* tElement = new vXMLTreeElement(
                name,
                vXMLTreeElement::TypeAttr,
                empty,true);

    tElement->setTreeNodeValue(value);
    current_node_edited->appendRow(tElement);
    xmlmodel->appendchain(tElement);
    QFont textfont=current_node_edited->font();
    textfont.setBold(true);
    tElement->setFont(textfont);
    // Add to change stack
    newstack = this->CreateStackElement(DialogPatternXmlEdit::ChangeTypeAdd);
    newstack->element= tElement;
    newstack->type=vXMLTreeElement::TypeAttr;
    newstack->newText=new QString(name);
    newstack->newValue=new QString(value);
    // Activate changes
    treeChange=true;
    ui->pushButton_Apply_Changes->setEnabled(true);
}

void DialogPatternXmlEdit::ButtonSetClicked(){
    QString name,value;
    ChangesStackElement* newstack;

    // TODO : check if a change has already been done
    if (this->current_node_edited == 0) return;
    name=ui->lineEdit_Name->text();
    value=ui->lineEdit_Value->text();
    bool nameedit,valueedit;

    nameedit = (name != current_node_edited->gettreeNodeName()) ? true : false;
    valueedit= (value != current_node_edited->gettreeNodeValue()) ? true : false;

    if ( nameedit == true  || valueedit == true ) {
        newstack = this->CreateStackElement(DialogPatternXmlEdit::ChangeTypeModify);
        newstack->element= this->current_node_edited;

        if (nameedit == true) {
            newstack->changedText=true;
            newstack->newText=new QString(name);
            current_node_edited->setTreeNodeName(name);
        }
        if (valueedit == true) {
            newstack->changedValue=true;
            newstack->newValue= new QString(value);
            current_node_edited->setTreeNodeValue(value);
        }

        QFont textfont=current_node_edited->font();
        textfont.setBold(true);
        current_node_edited->setFont(textfont);
        treeChange=true;
        ui->pushButton_Apply_Changes->setEnabled(true);
        clear_edit_data();
    }
}

void DialogPatternXmlEdit::nameTextEdited(QString newtext) {
    //QMessageBox::information(this, "nameTextEdited", QString("%1").arg(newtext));
    ui->pushButton_Set_Values->setEnabled(true);
    ui->pushButton_Cancel_Values->setEnabled(true);
}

void DialogPatternXmlEdit::valueTextEdited(QString newtext) {
    //QMessageBox::information(this, "valueTextEdited", QString("%1").arg(newtext));
    ui->pushButton_Set_Values->setEnabled(true);
    ui->pushButton_Cancel_Values->setEnabled(true);
}

void DialogPatternXmlEdit::clear_edit_data() {
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

void DialogPatternXmlEdit::clearStack() {
    ChangesStackElement * tmp;
    while (changeStackRoot != 0) {
        if (changeStackRoot->newText != 0) free (changeStackRoot->newText);
        if (changeStackRoot->newValue != 0) free (changeStackRoot->newValue);
        tmp=changeStackRoot;
        changeStackRoot=changeStackRoot->next;
        free(tmp);
    }
    changeStackLast=0;
}

void DialogPatternXmlEdit::baseSelectionChanged(int value)
{
    QDomNode newbase;

    int index;

    if (this->treeChange == true) {//Changes have been made : warn ?
    }
    clearStack();
    ui->pushButton_Apply_Changes->setEnabled(false);

    index = ui->comboBox_Base_Selection->itemData(value).toInt();  //.convert(QVariant::Int);
    Q_ASSERT(value < rootBasesNum);
    // QMessageBox::information(this, "test", QString("%1:%2").arg(value).arg(index));

    // Clear all tree info and nodes
    // TODO : Check
    while (rootNode->rowCount() > 0) {
        rootNode->removeRow(0);
    }
    xmlmodel->clearTree();

    vXMLTreeElement* standard_base = new vXMLTreeElement(
                rootBases[index]->gettreeNodeName(),
                vXMLTreeElement::TypeRoot,
                rootBases[index]->getDocNode(),
                false);
    rootNode->appendRow(standard_base);
    readNodes(rootBases[index]->getDocNode(),standard_base,xmlmodel,true);

    ui->treeView_main->setModel(xmlmodel);
    this->clear_edit_data();
    // TODO : warn if changes ?
    this->clearStack();
    return;
}

void DialogPatternXmlEdit::Element_clicked ( const QModelIndex & index ) {

    // Get item (function returns parent of clicked item)
    vXMLTreeElement *item = (vXMLTreeElement *)index.internalPointer();

    // Get child specified by index row/column
    vXMLTreeElement * item2 = (vXMLTreeElement *) item->child(index.row(),index.column());

    // Clear all data and disable buttons
    this->clear_edit_data();
    // Check status of element in change stack
    this->current_node_edited=item2;
    this->current_node_edited_status=0;
    DialogPatternXmlEdit::ChangesStackElement * local = this->changeStackRoot;
    while (local != 0) {
        if (local->element == item2) {
            this->current_node_edited_status=local->type;
            this->current_node_edited_stack=local;
            if (local->type == DialogPatternXmlEdit::ChangeTypeDelete) {
                // Node is deleted, only enable cancel button
                ui->pushButton_Cancel_Values->setEnabled(true);
                return;
            }
        }
        local=local->next;
    }
    // Fill the dialog and  Activate the line edit
    if (item2->getelementType() != vXMLTreeElement::TypeRoot) {
        ui->lineEdit_Name->setText(current_node_edited->gettreeNodeName());
        ui->lineEdit_Name->setEnabled(true);
        // Fill value
        ui->lineEdit_Value->setText(current_node_edited->gettreeNodeValue());
        ui->lineEdit_Value->setEnabled(true);
    }
    if (item2->getelementType() == vXMLTreeElement::TypeRoot) {
        ui->label_type_value->setText(tr("Root node"));;
        ui->pushButton_Add_son->setEnabled(true);
    } else if  (item2->getelementType() == vXMLTreeElement::TypeNode) {
        ui->label_type_value->setText(tr("Node"));
        ui->pushButton_Add_attribute->setEnabled(true);
        ui->pushButton_Add_son->setEnabled(true);
    } else if (item2->getelementType() == vXMLTreeElement::TypeAttr) {
        ui->label_type_value->setText(tr("Attribute"));
        ui->pushButton_Remove_attribute->setEnabled(true);
    }
}

DialogPatternXmlEdit::~DialogPatternXmlEdit()
{
    //xmlmodel->clearTree();
    delete ui;
    ChangesStackElement * tmp; while (changeStackRoot != 0) {
        tmp=changeStackRoot->next;
        free(changeStackRoot);
        changeStackRoot=tmp;
    }
    //delete rootBases[0];
    //free(rootBases);
    //delete xmlmodel;
}

void DialogPatternXmlEdit::readNodes(QDomNode dNode, vXMLTreeElement* root,vXMLTreeView* xmlmodel, bool refresh)
{

    QDomNode tNode,tNode2;
    vXMLTreeElement* tElement,*tElement2;
    int totalOfChilds = dNode.childNodes().size();
    if (totalOfChilds == 0) return; // empty xml
    for (int i=0;i<totalOfChilds;i++) {
        tNode = dNode.childNodes().at(i);
        if (tNode.nodeType() != QDomNode::CommentNode) {
            tElement = new vXMLTreeElement(tNode.nodeName(),vXMLTreeElement::TypeNode,tNode,false);
            xmlmodel->appendchain(tElement);
            root->appendRow(tElement);
            if (tNode.nodeName() == "draw") {
                for (int i = 0; i < tNode.attributes().size(); i++)
                {
                    if ((tNode.attributes().item(i).nodeName() == "name")
                            && (refresh == false)){
                        ui->comboBox_Base_Selection->addItem(tNode.attributes().item(i).nodeValue(),
                                                             QVariant(rootBasesNum));
                        rootBases[rootBasesNum]=new vXMLTreeElement(
                                    QString("Drawing %1").arg(tNode.attributes().item(i).nodeValue()),
                                    vXMLTreeElement::TypeRoot,tNode,false);;
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
        if (tElement->getelementType() == vXMLTreeElement::TypeNode) {
            totalOfChilds = tNode.childNodes().size();
            for (int i=0;i<totalOfChilds;i++) {
                tNode2 = tNode.childNodes().at(i);
                if (tNode2.nodeType() != QDomNode::CommentNode) {
                    tElement2 = new vXMLTreeElement(tNode2.nodeName(),vXMLTreeElement::TypeNode,tNode2,false);
                    xmlmodel->appendchain(tElement2);
                    tElement->appendRow(tElement2);
                }
            }

            if (tNode.nodeType() == QDomNode::TextNode) {
                tElement->setTreeNodeValue(tNode.nodeValue());
            }

            for (int i = 0; i < tNode.attributes().size(); i++)
            {
                tElement2 = new vXMLTreeElement(
                            tNode.attributes().item(i).nodeName(),
                            vXMLTreeElement::TypeAttr,
                            tNode,false);
                tElement2->setTreeNodeValue(tNode.attributes().item(i).nodeValue());
                tElement->appendRow(tElement2);
                xmlmodel->appendchain(tElement2);
            }
        }
        xmlmodel->setCurrent(xmlmodel->getCurrent()->next);
    }
    while (xmlmodel->getCurrent() != 0);
    return;
}

// vXMLTreeView Code  -----------------------------------------------------------------
vXMLTreeView::vXMLTreeView(QObject *parent) : QStandardItemModel(parent)
{
    current=last=items=0;
}

void vXMLTreeView::clearTree() {

    // TODO check if treeitems are deleted with the delete row command of QDom

    //clear the chain link
    TreeElementchain * tmp;
    while (items != 0) {
        tmp=items;
        items=items->next;
        free (tmp);
    }
    // reset chain counters
    current=last=items=0;

}

vXMLTreeView::~vXMLTreeView()
{

}

void vXMLTreeView::appendchain(vXMLTreeElement* elmt)
{
    if (last == 0) { // first element
        current=(TreeElementchain*)malloc(sizeof(TreeElementchain));
        if (current == 0) {
            Q_ASSERT(current != 0);
            // TODO : throw exception
        }
        current->elmt=elmt;
        current->next=0;
        last=items=current;
        return;
    }
    TreeElementchain* temp=(TreeElementchain*)malloc(sizeof(TreeElementchain));
    if (temp == 0) {
        Q_ASSERT(temp != 0);
        // TODO : throw exception
    }
    temp->elmt=elmt;
    temp->next=0;
    last->next=temp;
    last=temp;
}

vXMLTreeView::TreeElementchain * vXMLTreeView::getCurrent() { return this->current;}
void vXMLTreeView::setCurrent(vXMLTreeView::TreeElementchain * value) { this->current=value;}
vXMLTreeView::TreeElementchain * vXMLTreeView::getLast() { return this->last;}
vXMLTreeView::TreeElementchain * vXMLTreeView::getItems() { return this->items;}


// vXMLTreeElement code -----------------------------------------------------------------

const short int vXMLTreeElement::TypeNode=1;
const short int vXMLTreeElement::TypeAttr=2;
const short int vXMLTreeElement::TypeRoot=3;


vXMLTreeElement::vXMLTreeElement(QString name, int nodetype, QDomNode source,bool editor)
    : QStandardItem(name)
{
    this->elementType=nodetype;
    this->DocNode=source;
    this->addedNode=editor;
    if (!editor) this->DocNode=source;
    this->treeNodeName=name;
    this->treeNodeValue="<empty>"; // TODO : translation ?
    this->treeNodeValueSet=false,
            this->setText(this->displayText());
    switch (this->elementType) {
    case vXMLTreeElement::TypeAttr:
        this->setBackground(BACKGROUND_COLOR_ATTRIBUTE);
        break;
    default:
        break;
    }
}


vXMLTreeElement::~vXMLTreeElement()
{

}
void vXMLTreeElement::setTreeNodeValue(QString value)
{
    this->treeNodeValue=value;
    this->treeNodeValueSet=true;
    this->setText(this->displayText());
}

void vXMLTreeElement::setTreeNodeName(QString value)
{
    this->treeNodeName=value;
    this->setText(this->displayText());
}

QString vXMLTreeElement::displayText()
{
    if (this->elementType == vXMLTreeElement::TypeRoot)
        return this->treeNodeName;
    return QString("%1 : %2").arg((this->treeNodeName)).arg(this->treeNodeValue);
}

short int vXMLTreeElement::getelementType() { return this->elementType;}

QDomNode vXMLTreeElement::getDocNode()
{
    return this->DocNode;
}

QString vXMLTreeElement::gettreeNodeName() { return this->treeNodeName;}
QString vXMLTreeElement::gettreeNodeValue() {
    // Only return value if it's really set.
    if (this->treeNodeValueSet)
        return this->treeNodeValue;
    else
        return "";
}
bool vXMLTreeElement::gettreeNodeValueSet() {return this->treeNodeValueSet;}
