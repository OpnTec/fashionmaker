/************************************************************************
 **
 **  @file   vapplication.cpp
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

#include "vapplication.h"
#include "../exception/vexceptionobjecterror.h"
#include "../exception/vexceptionbadid.h"
#include "../exception/vexceptionconversionerror.h"
#include "../exception/vexceptionemptyparameter.h"
#include "../exception/vexceptionwrongparameterid.h"

#include <QMessageBox>
#include <QDebug>

// reimplemented from QApplication so we can throw exceptions in slots
bool VApplication::notify(QObject *receiver, QEvent *event)
{
    try
    {
      return QApplication::notify(receiver, event);
    }
    catch (const VExceptionObjectError &e)
    {
        e.CriticalMessageBox(tr("Error parsing file. Program will be terminated."));
        abort();
    }
    catch (const VExceptionBadId &e)
    {
        e.CriticalMessageBox(tr("Error bad id. Program will be terminated."));
        abort();
    }
    catch (const VExceptionConversionError &e)
    {
        e.CriticalMessageBox(tr("Error can't convert value. Program will be terminated."));
        abort();
    }
    catch (const VExceptionEmptyParameter &e)
    {
        e.CriticalMessageBox(tr("Error empty parameter. Program will be terminated."));
        abort();
    }
    catch (const VExceptionWrongParameterId &e)
    {
        e.CriticalMessageBox(tr("Error wrong id. Program will be terminated."));
        abort();
    }
    catch (const VException &e)
    {
        e.CriticalMessageBox(tr("Something's wrong!!"));
        return true;
    }
    catch (std::exception& e)
    {
      qCritical() << "Exception thrown:" << e.what();
    }
    return false;
}
