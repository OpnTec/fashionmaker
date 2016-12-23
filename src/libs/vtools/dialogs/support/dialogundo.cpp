/************************************************************************
 **
 **  @file   dialogundo.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 6, 2014
 **
 **  @brief
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

#include "dialogundo.h"

#include <QCloseEvent>
#include <QLocale>
#include <QPushButton>
#include <Qt>

#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "ui_dialogundo.h"

class QCloseEvent;
class QWidget;

//---------------------------------------------------------------------------------------------------------------------
DialogUndo::DialogUndo(QWidget *parent)
    :QDialog(parent), ui(new Ui::DialogUndo), result(UndoButton::Cancel)
{
    ui->setupUi(this);

    qApp->Settings()->GetOsSeparator() ? setLocale(QLocale::system()) : setLocale(QLocale::c());

    bool opening = qApp->getOpeningPattern();
    if (opening)
    {
        ui->pushButtonUndo->setDisabled(opening);
    }
    else
    {
        connect(ui->pushButtonUndo, &QPushButton::clicked, RECEIVER(this)[this]()
        {
            result = UndoButton::Undo;
            accept();
        });
    }
    connect(ui->pushButtonFix, &QPushButton::clicked, RECEIVER(this)[this]()
    {
        result = UndoButton::Fix;
        accept();
    });
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &DialogUndo::Cancel);

    setCursor(Qt::ArrowCursor);
}

//---------------------------------------------------------------------------------------------------------------------
DialogUndo::~DialogUndo()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogUndo::Cancel()
{
    result = UndoButton::Cancel;
    reject();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogUndo::closeEvent(QCloseEvent *event)
{
    Cancel();
    event->accept();
}
