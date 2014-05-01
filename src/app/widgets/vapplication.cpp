/************************************************************************
 **
 **  @file   vapplication.cpp
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

#include "vapplication.h"
#include "../exception/vexceptionobjecterror.h"
#include "../exception/vexceptionbadid.h"
#include "../exception/vexceptionconversionerror.h"
#include "../exception/vexceptionemptyparameter.h"
#include "../exception/vexceptionwrongid.h"

#include <QMessageBox>
#include <QDebug>

const qreal VApplication::PrintDPI = 96.0;

#define DefWidth 1.2//mm

VApplication::VApplication(int &argc, char **argv)
    : QApplication(argc, argv), _patternUnit(Valentina::Cm), _patternType(Pattern::Individual),
      _widthMainLine(DefWidth), _widthHairLine(DefWidth/3.0)
{
    InitLineWidth();
}

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
    catch (const VExceptionWrongId &e)
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

double VApplication::toPixel(double unit) const
{
    double result = 0;
    switch (_patternUnit)
    {
    case Valentina::Mm:
        result = (unit / 25.4) * PrintDPI;
        break;
    case Valentina::Cm:
        result = ((unit * 10.0) / 25.4) * PrintDPI;
        break;
    case Valentina::Inch:
        result = unit * PrintDPI;
        break;
    default:
        break;
    }
    return result;
}

double VApplication::fromPixel(double pix) const
{
    double result = 0;
    switch (_patternUnit)
    {
    case Valentina::Mm:
        result = (pix / PrintDPI) * 25.4;
        break;
    case Valentina::Cm:
        result = ((pix / PrintDPI) * 25.4) / 10.0;
        break;
    case Valentina::Inch:
        result = pix / PrintDPI;
        break;
    default:
        break;
    }
    return result;
}
QString VApplication::pathToTables() const
{
    if (_patternType == Pattern::Individual)
    {
        #ifdef Q_OS_WIN
            return QApplication::applicationDirPath() + QStringLiteral("/tables/individual");
        #else
            #ifdef QT_DEBUG
                return QApplication::applicationDirPath() + QStringLiteral("/tables/individual");
            #else
                return QStringLiteral("/usr/share/valentina/tables/individual");
            #endif
        #endif
    }
    else
    {
        #ifdef Q_OS_WIN
            return QApplication::applicationDirPath() + QStringLiteral("/tables/standard");
        #else
            #ifdef QT_DEBUG
                return QApplication::applicationDirPath() + QStringLiteral("/tables/standard");
            #else
                return QStringLiteral("/usr/share/valentina/tables/standard");
            #endif
        #endif
    }
}

QString VApplication::translationsPath() const
{
#ifdef Q_OS_WIN
    return QApplication::applicationDirPath() + QStringLiteral("/translations");
#else
#ifdef QT_DEBUG
    return QApplication::applicationDirPath() + QStringLiteral("/translations");
        #else
            return QStringLiteral("/usr/share/valentina/translations");
        #endif
    #endif
}


void VApplication::InitLineWidth()
{
    switch (_patternUnit)
    {
        case Valentina::Mm:
            _widthMainLine = DefWidth;
            break;
        case Valentina::Cm:
            _widthMainLine = DefWidth/10.0;
            break;
        case Valentina::Inch:
            _widthMainLine = DefWidth/25.4;
            break;
        default:
            _widthMainLine = DefWidth;
            break;
    }
    _widthHairLine = _widthMainLine/3.0;
}


void VApplication::setPatternUnit(const Valentina::Units &patternUnit)
{
    _patternUnit = patternUnit;
    InitLineWidth();
}
