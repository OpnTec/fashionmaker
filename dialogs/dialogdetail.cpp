#include "dialogdetail.h"

DialogDetail::DialogDetail(const VContainer *data, Draw::Mode mode, QWidget *parent) :
    DialogTool(data, parent), ui(), details(VDetail()), mode(mode){
    ui.setupUi(this);
    bOk = ui.buttonBox->button(QDialogButtonBox::Ok);
    connect(bOk, &QPushButton::clicked, this, &DialogDetail::DialogAccepted);

    QPushButton *bCansel = ui.buttonBox->button(QDialogButtonBox::Cancel);
    connect(bCansel, &QPushButton::clicked, this, &DialogDetail::DialogRejected);
}

void DialogDetail::ChoosedObject(qint64 id, Scene::Type type){
    if(type != Scene::Line){
        NewItem(id, type, mode);
        this->show();
    }
}

void DialogDetail::DialogAccepted(){
    details.Clear();
    for(qint32 i = 0; i < ui.listWidget->count(); ++i){
        QListWidgetItem *item = ui.listWidget->item(i);
        details.append( qvariant_cast<VNodeDetail>(item->data(Qt::UserRole)));
    }
    details.setName(ui.lineEditNameDetail->text());
    emit DialogClosed(QDialog::Accepted);
}

void DialogDetail::NewItem(qint64 id, Scene::Type typeTool, Draw::Mode mode){
    QString name;
    switch(typeTool){
        case(Scene::Line):
        break;
        case(Scene::Point):{
            VPointF point = data->GetPoint(id);
            name = point.name();
        }
            break;
        case(Scene::Arc):{
            VArc arc = data->GetArc(id);
            name = data->GetNameArc(arc.GetCenter(), id);
        }
            break;
        case(Scene::Spline):{
            VSpline spl = data->GetSpline(id);
            name = spl.GetName();
        }
            break;
        case(Scene::SplinePath):{
            VSplinePath splPath = data->GetSplinePath(id);
            name = data->GetNameSplinePath(splPath);
        }
            break;
    }

    QListWidgetItem *item = new QListWidgetItem(name);
    item->setFont(QFont("Times", 12, QFont::Bold));
    VNodeDetail node(id, typeTool, mode);
    item->setData(Qt::UserRole, QVariant::fromValue(node));
    ui.listWidget->addItem(item);
}

VDetail DialogDetail::getDetails() const{
    return details;
}

void DialogDetail::setDetails(const VDetail &value){
    details = value;
    ui.listWidget->clear();
    for(qint32 i = 0; i < details.CountNode(); ++i){
        NewItem(details[i].getId(), details[i].getTypeTool(),details[i].getMode());
    }
    details.setName(ui.lineEditNameDetail->text());
    ui.listWidget->setFocus(Qt::OtherFocusReason);
}

