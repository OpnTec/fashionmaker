#include "vshortcutpropertyeditor.h"

#include <QHBoxLayout>
#include <QKeyEvent>

using namespace VPE;

VShortcutEditWidget::VShortcutEditWidget(QWidget *parent)
    : QWidget(parent)
{
    // Create the line edit widget
    LineEdit = new QLineEdit(this);
    LineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    LineEdit->clear();
    LineEdit->installEventFilter(this);
    setFocusProxy(LineEdit);
    connect(LineEdit, SIGNAL(textEdited(QString)), this, SLOT(onTextEdited(QString)));

    // The layout (a horizontal layout)
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(LineEdit);
}


VShortcutEditWidget::~VShortcutEditWidget()
{
    // nothing needs to be done here
}

bool VShortcutEditWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == LineEdit) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

            int keys = keyEvent->key();

            if(keys != Qt::Key_Shift &&
               keys != Qt::Key_Control &&
               keys != Qt::Key_Meta &&
               keys != Qt::Key_AltGr &&
               keys != Qt::Key_Alt)
            {
                keys += keyEvent->modifiers();
                setShortcut(QKeySequence(keys), true);
                return true;
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

QString VShortcutEditWidget::getShortcutAsString()
{
    return CurrentKeySequence.toString();
}

QKeySequence VShortcutEditWidget::getShortcut()
{
    return CurrentKeySequence;
}



void VShortcutEditWidget::setShortcut(const QString &shortcut, bool emit_signal)
{
    setShortcut(QKeySequence::fromString(shortcut), emit_signal);
}

void VShortcutEditWidget::setShortcut(const QKeySequence &shortcut, bool emit_signal)
{
    if(shortcut != CurrentKeySequence) {
        CurrentKeySequence = shortcut;
        LineEdit->setText(CurrentKeySequence.toString());
        if(emit_signal)
            emit dataChangedByUser(CurrentKeySequence, this);
    }
}

void VShortcutEditWidget::onTextEdited(const QString &text)
{
    setShortcut(text, true);
}
