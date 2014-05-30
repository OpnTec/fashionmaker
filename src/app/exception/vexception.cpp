/************************************************************************
 **
 **  @file   vexception.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

//---------------------------------------------------------------------------------------------------------------------
VException::VException(const QString &what):QException(), what(what), moreInfo(QString())
{
    Q_ASSERT_X(what.isEmpty() == false, Q_FUNC_INFO, "Error message is empty");
}

//---------------------------------------------------------------------------------------------------------------------
VException::VException(const VException &e):what(e.What()), moreInfo(e.MoreInformation())
{}

//---------------------------------------------------------------------------------------------------------------------
QString VException::ErrorMessage() const
{
    QString error = QString("Exception: %1").arg(what);
    return error;
}

//---------------------------------------------------------------------------------------------------------------------
void VException::CriticalMessageBox(const QString &situation, QWidget * parent) const
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(tr("Critical error!"));
    msgBox.setText(situation);
    msgBox.setInformativeText(ErrorMessage());
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    if (moreInfo.isEmpty() == false)
    {
        msgBox.setDetailedText(DetailedInformation());
    }
    msgBox.setIcon(QMessageBox::Critical);
    QSpacerItem* horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = static_cast<QGridLayout*>(msgBox.layout());
    Q_CHECK_PTR(layout);
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    //Disable Qt::WaitCursor for error message.
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    msgBox.exec();
}

//---------------------------------------------------------------------------------------------------------------------
void VException::AddMoreInformation(const QString &info)
{
    if (info.isEmpty())
    {
        return;
    }
    moreInfo = QString("%1\n%2").arg(moreInfo, info);
}

//---------------------------------------------------------------------------------------------------------------------
QString VException::MoreInfo(const QString &detInfo) const
{
    if (moreInfo.isEmpty() == false)
    {
        return QString("%1\n%2").arg(moreInfo, detInfo);
    }
    else
    {
        return detInfo;
    }
}
