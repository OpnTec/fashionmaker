#include "vpropertyformwidget.h"

#include <QFormLayout>
#include "vpropertyformwidget_p.h"

#include "plugins/vwidgetproperty.h"
#include <QEvent>
#include <QKeyEvent>
#include "vproperty.h"
#include <QDebug>

using namespace VPE;

VPropertyFormWidget::VPropertyFormWidget(const QString &title, const QString &description, const QList<VProperty*>& properties, QWidget *parent)
    : QGroupBox(title, parent), d_ptr(new VPropertyFormWidgetPrivate(properties))
{
    build();
    setToolTip(description);
    setWhatsThis(description);
}

VPropertyFormWidget::VPropertyFormWidget(VProperty *parent_property, QWidget *parent)
    : QGroupBox(parent), d_ptr(new VPropertyFormWidgetPrivate())
{
    if(parent_property) {
        d_ptr->Properties = parent_property->getChildren();
        build();
        setTitle(parent_property->getName());
        setToolTip(parent_property->getDescription());
        setWhatsThis(parent_property->getDescription());
    }
}

VPropertyFormWidget::VPropertyFormWidget(VPropertyFormWidgetPrivate *d_pointer, QWidget *parent, const QString &title, const QString &description)
    : QGroupBox(title, parent), d_ptr(d_pointer)
{
    build();
    setToolTip(description);
    setWhatsThis(description);
}

VPropertyFormWidget::~VPropertyFormWidget()
{
    delete d_ptr;
}


void VPropertyFormWidget::build()
{
    // Clear the old content, delete old widgets
    d_ptr->EditorWidgets.clear();
    if(layout()) {
        QLayoutItem *child;
        while (layout()->count() > 0 && (child = layout()->takeAt(0)) != 0) {
            if(child->widget())
                delete child->widget();
            delete child;
        }
        delete layout();
    }

    // Create new content
    if(d_ptr->Properties.isEmpty()) return;	//... only if there are properties

    QFormLayout* tmpFormLayout = new QFormLayout(this);
    setLayout(tmpFormLayout);

    for(int i = 0; i < d_ptr->Properties.count(); ++i) {
        // Get the current property
        VProperty* tmpProperty = d_ptr->Properties.value(i, nullptr);
        if(!tmpProperty) continue;

        if(tmpProperty->getRowCount() > 0) {
            if (tmpProperty->propertyType() == Property::Complex)
            {
                buildEditor(tmpProperty, tmpFormLayout, Property::Complex);
                QWidget *group = new QWidget(this);
                tmpFormLayout->addRow(group);

                QFormLayout* subFormLayout = new QFormLayout(group);
                QMargins margins = subFormLayout->contentsMargins();
                margins.setTop(0);
                margins.setLeft(14);
                subFormLayout->setContentsMargins(margins);

                group->setLayout(subFormLayout);
                QList<VProperty*> children = tmpProperty->getChildren();
                for (int j = 0; j < children.size(); ++j)
                {
                    buildEditor(children[j], subFormLayout);
                    connect(children[j], &VProperty::childChanged, tmpProperty, &VProperty::ValueChildChanged,
                            Qt::UniqueConnection);
                    ++i;
                    d_ptr->Properties.insert(i, children[j]);
                }
            }
            else
            {
                // Child properties, the property itself is not being added
                VPropertyFormWidget* tmpNewFormWidget = new VPropertyFormWidget(tmpProperty, this);
                tmpFormLayout->addRow(tmpNewFormWidget);
                d_ptr->EditorWidgets.append(VPropertyFormWidgetPrivate::SEditorWidget(tmpNewFormWidget));
                tmpNewFormWidget->setCommitBehaviour(d_ptr->UpdateEditors);
            }
        } else if(tmpProperty->type() == "widget") {
            VWidgetProperty* tmpWidgetProperty = static_cast<VWidgetProperty*>(tmpProperty);
            tmpFormLayout->addRow(tmpWidgetProperty->getWidget());
            d_ptr->EditorWidgets.append(VPropertyFormWidgetPrivate::SEditorWidget(tmpWidgetProperty->getWidget()));
        } else {
            buildEditor(tmpProperty, tmpFormLayout);
        }
    }
}

void VPropertyFormWidget::buildEditor(VProperty* property, QFormLayout* formLayout, Property type)
{
    // Add property (no child properties)
    // Create the editor (if it doesn't work, create empty widget)
    QWidget* tmpEditor = property->createEditor(this, QStyleOptionViewItem(), nullptr);
    if(!tmpEditor)
        tmpEditor = new QWidget(this);

    // set tooltip and whats this
    tmpEditor->setToolTip(property->getDescription());
    tmpEditor->setWhatsThis(property->getDescription());

    // Install event filter
    tmpEditor->installEventFilter(this);

    // Set the editor data
    property->setEditorData(tmpEditor);

    // add new row
    if (type == Property::Complex)
    {
        QString name = "<b>"+property->getName()+"</b>";
        formLayout->addRow(name, tmpEditor);
    }
    else
    {
        formLayout->addRow(property->getName(), tmpEditor);
    }

    d_ptr->EditorWidgets.append(VPropertyFormWidgetPrivate::SEditorWidget(tmpEditor));
}

void VPropertyFormWidget::commitData()
{
    for(int i = 0; i < d_ptr->Properties.count(); ++i)
        commitData(i);
}

void VPropertyFormWidget::loadData()
{
    for(int i = 0; i < d_ptr->Properties.count(); ++i)
        loadData(i);
}

void VPropertyFormWidget::commitData(int row)
{
    if(row < 0 || row >= d_ptr->EditorWidgets.count() || row >= d_ptr->Properties.count()) return;

    VPropertyFormWidgetPrivate::SEditorWidget& tmpEditorWidget = d_ptr->EditorWidgets[row];
    VProperty* tmpProperty = d_ptr->Properties[row];
    if(tmpEditorWidget.FormWidget)
        tmpEditorWidget.FormWidget->commitData();
    else if(tmpEditorWidget.Editor && tmpProperty)
    {
        QVariant newValue = tmpProperty->getEditorData(tmpEditorWidget.Editor);
        QVariant oldValue = tmpProperty->data(VProperty::DPC_Data, Qt::EditRole);
        if (oldValue != newValue)
        {
            VProperty *parent = tmpProperty->getParent();
            if (parent == nullptr)
            {
                tmpProperty->setValue(newValue);
                emit propertyDataSubmitted(tmpProperty);
            }
            else if (parent->propertyType() == Property::Complex)
            {
                tmpProperty->UpdateParent(newValue);
                emit propertyDataSubmitted(parent);
            }
            else
            {
                tmpProperty->setValue(newValue);
                emit propertyDataSubmitted(tmpProperty);
            }
        }
    }
}

void VPropertyFormWidget::loadData(int row)
{
    if(row < 0 || row >= d_ptr->EditorWidgets.count() || row >= d_ptr->Properties.count()) return;

    VPropertyFormWidgetPrivate::SEditorWidget& tmpEditorWidget = d_ptr->EditorWidgets[row];
    VProperty* tmpProperty = d_ptr->Properties[row];
    if(tmpEditorWidget.FormWidget)
        tmpEditorWidget.FormWidget->loadData();
    else if(tmpEditorWidget.Editor && tmpProperty) {
        tmpProperty->setEditorData(tmpEditorWidget.Editor);
    }
}

void VPropertyFormWidget::setCommitBehaviour(bool auto_commit)
{
    d_ptr->UpdateEditors = auto_commit;

    QList<VPropertyFormWidget*> tmpChildFormWidgets = getChildPropertyFormWidgets();
    foreach(VPropertyFormWidget* tmpChild, tmpChildFormWidgets) {
        if(tmpChild)
            tmpChild->setCommitBehaviour(auto_commit);
    }
}

QList<VPropertyFormWidget *> VPropertyFormWidget::getChildPropertyFormWidgets() const
{
    QList<VPropertyFormWidget *> tmpResult;
    foreach(const VPropertyFormWidgetPrivate::SEditorWidget& tmpEditorWidget, d_ptr->EditorWidgets) {
        if(tmpEditorWidget.FormWidget)
            tmpResult.append(tmpEditorWidget.FormWidget);
    }

    return tmpResult;
}

bool VPropertyFormWidget::eventFilter(QObject *object, QEvent *event)
{
    if(!d_ptr->UpdateEditors)
        return false;

    QWidget* editor = qobject_cast<QWidget*>(object);
    if (!editor)
        return false;

    if (event->type() == QEvent::KeyPress) {
        switch (static_cast<QKeyEvent *>(event)->key()) {
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
                commitData(editor);
                event->accept();
                return true;
            default:
                return false;
        }
    } else if (event->type() == QEvent::FocusOut || (event->type() == QEvent::Hide && editor->isWindow())) {
        commitData(editor);
        return false;
    } else if (event->type() == QEvent::ShortcutOverride) {
        if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_Escape) {
            commitData(editor);
            event->accept();
            return true;
        }
    } else if (event->type() == MyCustomEventType) {
        commitData(editor);
        event->accept();
        return true;
       }
    else
    {
        return QGroupBox::eventFilter(object, event);
    }

    // Default:
    return false;
}

void VPropertyFormWidget::commitData(QWidget *editor)
{
    if(!editor)
        return;

    for(int i = 0; i < d_ptr->EditorWidgets.count(); ++i) {
        VPropertyFormWidgetPrivate::SEditorWidget& tmpEditorWidget = d_ptr->EditorWidgets[i];
        if(tmpEditorWidget.Editor == editor)
            commitData(i);
    }
}
