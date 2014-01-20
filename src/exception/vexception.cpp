/************************************************************************
 **
 **  @file   vexception.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
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

#include "vexception.h"
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>

VException::VException(const QString &what):QException(), what(what)
{
    Q_ASSERT_X(what.isEmpty() == false, Q_FUNC_INFO, "Error message is empty");
}

QString VException::ErrorMessage() const
{
    QString error = QString("Exception: %1").arg(what);
    return error;
}

void VException::CriticalMessageBox(const QString &situation) const
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Critical error!");
    msgBox.setText(situation);
    msgBox.setInformativeText(ErrorMessage());
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    if (DetailedInformation().isEmpty() == false)
    {
        msgBox.setDetailedText(DetailedInformation());
    }
    msgBox.setIcon(QMessageBox::Critical);
    QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    Q_ASSERT(horizontalSpacer != 0);
    QGridLayout* layout = static_cast<QGridLayout*>(msgBox.layout());
    Q_ASSERT(layout != 0);
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    msgBox.exec();
}
