/************************************************************************
 **
 **  @file   vcolorpropertyeditor.cpp
 **  @author hedgeware <internal(at)hedgeware.net>
 **  @date
 **
 **  @brief
 **  @copyright
 **  All rights reserved. This program and the accompanying materials
 **  are made available under the terms of the GNU Lesser General Public License
 **  (LGPL) version 2.1 which accompanies this distribution, and is available at
 **  http://www.gnu.org/licenses/lgpl-2.1.html
 **
 **  This library is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 **  Lesser General Public License for more details.
 **
 *************************************************************************/

#include "vcolorpropertyeditor.h"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>
#include <QColorDialog>

#include "vproperty.h"

using namespace VPE;

VColorPropertyEditor::VColorPropertyEditor(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);

    // Create the tool button
    ToolButton = new QToolButton(this);
    ToolButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    ToolButton->setText(tr("..."));
    ToolButton->setFixedWidth(20);
    ToolButton->installEventFilter(this);
    setFocusProxy(ToolButton);	// Make the ToolButton the focus proxy
    setFocusPolicy(ToolButton->focusPolicy());
    connect(ToolButton, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));

    // Create the text label
    TextLabel = new QLabel(this);
    TextLabel->setText(getColorString(Color));

    // Create the label for the pixmap
    ColorLabel = new QLabel(this);
    ColorLabel->setPixmap(getColorPixmap(Color));

    // Spacer (this is needed for proper display of the label and button)
    Spacer = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Ignored);

    // The layout (a horizontal layout)
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(3);
    layout->setMargin(0);
    layout->addWidget(ColorLabel);
    layout->addWidget(TextLabel);
    layout->addItem(Spacer);
    layout->addWidget(ToolButton);
    //TextLabel->hide();
    //ColorLabel->hide();	// for now, we just use the standard display and only add the button
}

void VColorPropertyEditor::setColor(const QColor& color_)
{
    if (Color != color_)
    {
        Color = color_;
        ColorLabel->setPixmap(getColorPixmap(Color));
        TextLabel->setText(getColorString(Color));
    }
}

QPixmap VColorPropertyEditor::getColorPixmap(const QColor& color, unsigned int size)
{
    QImage tmpImgage(size, size, QImage::Format_ARGB32_Premultiplied);
    tmpImgage.fill(static_cast<unsigned int>(color.rgb()));
    return QPixmap::fromImage(tmpImgage);
    // todo: support alpha channel
}

QString VColorPropertyEditor::getColorString(const QColor& color)
{
    return QApplication::translate("QtPropertyExplorer", "[%1, %2, %3] (%4)", "Colors as string")
                                 .arg(QString::number(color.red()))
                                 .arg(QString::number(color.green()))
                                 .arg(QString::number(color.blue()))
                                 .arg(QString::number(color.alpha()));

}

void VColorPropertyEditor::onToolButtonClicked()
{
    bool ok = false;
    QRgb oldRgba = Color.rgba();
    QRgb newRgba = QColorDialog::getRgba(oldRgba, &ok, this);
    if (ok && newRgba != oldRgba) {
        setColor(QColor::fromRgba(newRgba));
        emit dataChangedByUser(Color, this);
        UserChangeEvent *event = new UserChangeEvent();
        QCoreApplication::postEvent ( this, event );
    }
}

bool VColorPropertyEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == ToolButton && (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyPress))
    {
        // Ignore the event, so that eventually the delegate gets the event.
        ev->ignore();
        return true;
    }

    return QWidget::eventFilter(obj, ev);
}


VColorPropertyEditor::~VColorPropertyEditor()
{
    //
}



QColor VColorPropertyEditor::getColor()
{
    return Color;
}
