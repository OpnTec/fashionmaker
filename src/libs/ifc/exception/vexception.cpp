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
#include <QGridLayout>
#include <QMessageBox>
#include <QSpacerItem>
#include <QApplication>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(vExcep, "v.excep")

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VException constructor exception
 * @param what string with error
 */
VException::VException(const QString &what):QException(), what(what), moreInfo(QString())
{
    Q_ASSERT_X(what.isEmpty() == false, Q_FUNC_INFO, "Error message is empty");
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VException copy constructor
 * @param e exception
 */
VException::VException(const VException &e):what(e.What()), moreInfo(e.MoreInformation())
{}

//---------------------------------------------------------------------------------------------------------------------
VException &VException::operator=(const VException &e)
{
    if ( &e == this )
    {
        return *this;
    }
    this->what = e.What();
    this->moreInfo = e.MoreInformation();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ErrorMessage return main error message
 * @return error message
 */
QString VException::ErrorMessage() const
{
    QString error = QString("Exception: %1").arg(what);
    return error;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CriticalMessageBox show Critical Message Box.
 * @param situation main text message box.
 */
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
    SCASSERT(layout != nullptr);
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    //Disable Qt::WaitCursor for error message.
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    qCDebug(vExcep)<<"Critical error!"<<situation<<ErrorMessage()<<DetailedInformation();
    msgBox.exec();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddMoreInformation add more information for error
 * @param info information
 */
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

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief DetailedInformation return detailed information about error
 * @return detailed information
 */
QString VException::DetailedInformation() const
{
    return moreInfo;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief clone clone exception
 * @return new exception
 */
// cppcheck-suppress unusedFunction
VException *VException::clone() const
{
    return new VException(*this);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief raise method raise for exception
 */
Q_NORETURN void VException::raise() const
{
    throw *this;
}
