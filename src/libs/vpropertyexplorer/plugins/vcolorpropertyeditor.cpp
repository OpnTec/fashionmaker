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

#include <QColorDialog>
#include <QCoreApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QRgb>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QToolButton>

#include "../vproperty.h"

VPE::VColorPropertyEditor::VColorPropertyEditor(QWidget *parent)
    : QWidget(parent), Color(), ToolButton(nullptr), TextLabel(nullptr), ColorLabel(nullptr), Spacer(nullptr)
{
    setAutoFillBackground(true);

    // Create the tool button
    ToolButton = new QToolButton(this);
    ToolButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    ToolButton->setText("...");
    ToolButton->setFixedWidth(20);
    ToolButton->installEventFilter(this);
    setFocusProxy(ToolButton);  // Make the ToolButton the focus proxy
    setFocusPolicy(ToolButton->focusPolicy());
    connect(ToolButton, &QToolButton::clicked, this, &VColorPropertyEditor::onToolButtonClicked);

    // Create the text label
    TextLabel = new QLabel(this);
    TextLabel->setText(GetColorString(Color));

    // Create the label for the pixmap
    ColorLabel = new QLabel(this);
    ColorLabel->setPixmap(GetColorPixmap(Color));

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
    //ColorLabel->hide();   // for now, we just use the standard display and only add the button
}

void VPE::VColorPropertyEditor::SetColor(const QColor& color_)
{
    if (Color != color_)
    {
        Color = color_;
        ColorLabel->setPixmap(GetColorPixmap(Color));
        TextLabel->setText(GetColorString(Color));
    }
}

QPixmap VPE::VColorPropertyEditor::GetColorPixmap(const QColor& color, quint32 size)
{
    QImage tmpImgage(static_cast<int>(size), static_cast<int>(size), QImage::Format_ARGB32_Premultiplied);
    tmpImgage.fill(static_cast<quint32>(color.rgb()));
    return QPixmap::fromImage(tmpImgage);
    // todo: support alpha channel
}

QString VPE::VColorPropertyEditor::GetColorString(const QColor& color)
{
    return QString("[%1, %2, %3] (%4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
}

void VPE::VColorPropertyEditor::onToolButtonClicked()
{
    const QColor newColor = QColorDialog::getColor(Color, this, QString(), QColorDialog::ShowAlphaChannel);
    if (newColor.isValid() && newColor != Color)
    {
        SetColor(newColor);
        emit dataChangedByUser(Color, this);
        UserChangeEvent *event = new UserChangeEvent();
        QCoreApplication::postEvent ( this, event );
    }
}

bool VPE::VColorPropertyEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == ToolButton && ev->type() == QEvent::KeyPress)
    {
        // Ignore the event, so that eventually the delegate gets the event.
        ev->ignore();
        return true;
    }

    return QWidget::eventFilter(obj, ev);
}


VPE::VColorPropertyEditor::~VColorPropertyEditor()
{
    //
}

QColor VPE::VColorPropertyEditor::GetColor() const
{
    return Color;
}
