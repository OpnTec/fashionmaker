/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#include "vapplication.h"
#include <QtGui>
#include <QMessageBox>
#include "exception/vexceptionobjecterror.h"
#include "exception/vexceptionbadid.h"
#include "exception/vexceptionconversionerror.h"
#include "exception/vexceptionemptyparameter.h"
#include "exception/vexceptionwrongparameterid.h"

VApplication::VApplication(int & argc, char ** argv) :
    QApplication(argc, argv){
}

// reimplemented from QApplication so we can throw exceptions in slots
bool VApplication::notify(QObject *receiver, QEvent *event){
    try {
      return QApplication::notify(receiver, event);
    }
    catch(const VExceptionObjectError &e){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error!"));
        msgBox.setText(tr("Error parsing file. Program will be terminated."));
        msgBox.setInformativeText(e.ErrorMessage());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setDetailedText(e.DetailedInformation());
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        abort();
    }
    catch(const VExceptionBadId &e){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error!"));
        msgBox.setText(tr("Error bad id. Program will be terminated."));
        msgBox.setInformativeText(e.ErrorMessage());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        abort();
    }
    catch(const VExceptionConversionError &e){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error!"));
        msgBox.setText(tr("Error can't convert value. Program will be terminated."));
        msgBox.setInformativeText(e.ErrorMessage());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        abort();
    }
    catch(const VExceptionEmptyParameter &e){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error!"));
        msgBox.setText(tr("Error empty parameter. Program will be terminated."));
        msgBox.setInformativeText(e.ErrorMessage());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setDetailedText(e.DetailedInformation());
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        abort();
    }
    catch(const VExceptionWrongParameterId &e){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error!"));
        msgBox.setText(tr("Error wrong id. Program will be terminated."));
        msgBox.setInformativeText(e.ErrorMessage());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setDetailedText(e.DetailedInformation());
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        abort();
    }
    catch(std::exception& e) {
      qCritical() << "Exception thrown:" << e.what();
    }
    return false;
}
