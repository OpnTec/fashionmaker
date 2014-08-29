/************************************************************************
 **
 **  @file   vformulapropertyeditor.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#include "vformulapropertyeditor.h"

#include <QHBoxLayout>
#include <QFileDialog>
#include <QKeyEvent>
#include <QApplication>
#include <QColorDialog>

#include "../libs/vpropertyexplorer/vproperty.h"
#include "../dialogs/tools/dialogeditwrongformula.h"

using namespace VPE;

// VFormulaPropertyEditor
//---------------------------------------------------------------------------------------------------------------------
VFormulaPropertyEditor::VFormulaPropertyEditor(QWidget *parent) :
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
    TextLabel->setText(formula.getValue());

    // Spacer (this is needed for proper display of the label and button)
    Spacer = new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Ignored);

    // The layout (a horizontal layout)
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(3);
    layout->setMargin(0);
    layout->addWidget(TextLabel);
    layout->addItem(Spacer);
    layout->addWidget(ToolButton);
}

//---------------------------------------------------------------------------------------------------------------------
void VFormulaPropertyEditor::setFormula(const VFormula& formula)
{
    if (this->formula != formula)
    {
        this->formula = formula;
        TextLabel->setText(this->formula.getValue());
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VFormulaPropertyEditor::onToolButtonClicked()
{
    DialogEditWrongFormula* tmpWidget = new DialogEditWrongFormula(formula.getData(), formula.getToolId());
    tmpWidget->setCheckZero(formula.getCheckZero());
    tmpWidget->setPostfix(formula.getPostfix());
    tmpWidget->setFormula(formula.getFormula(FormulaType::FromUser));

    if (tmpWidget->exec() == QDialog::Accepted)
    {
        formula.setFormula(tmpWidget->getFormula(), FormulaType::ToUser);
        TextLabel->setText(formula.getValue());
        delete tmpWidget;
        emit dataChangedByUser(formula, this);
        UserChangeEvent *event = new UserChangeEvent();
        QCoreApplication::postEvent ( this, event );
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VFormulaPropertyEditor::eventFilter(QObject *obj, QEvent *ev)
{
    if(obj == ToolButton && (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyPress))
    {
        // Ignore the event, so that eventually the delegate gets the event.
        ev->ignore();
        return true;
    }

    return QWidget::eventFilter(obj, ev);
}

//---------------------------------------------------------------------------------------------------------------------
VFormulaPropertyEditor::~VFormulaPropertyEditor()
{
    //
}

//---------------------------------------------------------------------------------------------------------------------
VFormula VFormulaPropertyEditor::getFormula()
{
    return formula;
}
