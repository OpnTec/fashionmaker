/************************************************************************
 **
 **  @file   tst_vmeasurements.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 10, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "tst_vmeasurements.h"
#include "../vformat/vmeasurements.h"
#include "../ifc/xml/vvstconverter.h"
#include "../ifc/xml/vvitconverter.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
TST_VMeasurements::TST_VMeasurements(QObject *parent) :
    QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CreateEmptyStandardFile check if empty standard measurement file is valid.
 */
void TST_VMeasurements::CreateEmptyStandardFile()
{
    Unit mUnit = Unit::Cm;
    const int height = 176;
    const int size = 50;

    QSharedPointer<VContainer> data = QSharedPointer<VContainer>(new VContainer(nullptr, &mUnit));
    data->SetHeight(height);
    data->SetSize(size);

    QSharedPointer<VMeasurements> m =
            QSharedPointer<VMeasurements>(new VMeasurements(mUnit, size, height, data.data()));

    QTemporaryFile file;
    QString fileName;
    // In Windows we have problems when we try to open QSaveFile when QTemporaryFile with the same name is already open.
    if (file.open())
    {
        // So, before we try to open file in m->SaveDocument function we need to close it and remove.
        // Just closing - is not enough, if we just close QTemporaryFile we get "access denied" in Windows.
        fileName = file.fileName();
        file.close();
        file.remove();
        QString error;
        const bool result = m->SaveDocument(fileName, error);

        QVERIFY2(result, error.toUtf8().constData());
    }
    else
    {
        QFAIL("Can't open temporary file.");
    }

    try
    {
        VDomDocument::ValidateXML(VVSTConverter::CurrentSchema, fileName);
    }
    catch (VException &e)
    {
        QFAIL(e.ErrorMessage().toUtf8().constData());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CreateEmptyIndividualFile check if empty individual measurement file is valid.
 */
void TST_VMeasurements::CreateEmptyIndividualFile()
{
    Unit mUnit = Unit::Cm;

    QSharedPointer<VContainer> data = QSharedPointer<VContainer>(new VContainer(nullptr, &mUnit));

    QSharedPointer<VMeasurements> m =
            QSharedPointer<VMeasurements>(new VMeasurements(mUnit, data.data()));

    QTemporaryFile file;
    QString fileName;
    if (file.open())
    {
        fileName = file.fileName();
        file.close();
        file.remove();
        QString error;
        const bool result = m->SaveDocument(fileName, error);

        QVERIFY2(result, error.toUtf8().constData());
    }
    else
    {
        QFAIL("Can't open temporary file.");
    }

    try
    {
        VDomDocument::ValidateXML(VVITConverter::CurrentSchema, fileName);
    }
    catch (VException &e)
    {
        QFAIL(e.ErrorMessage().toUtf8().constData());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ValidPMCodesStandardFile helps to check that all current pattern making systems match pattern inside XSD
 * scheme.
 */
void TST_VMeasurements::ValidPMCodesStandardFile()
{
    Unit mUnit = Unit::Cm;
    const int height = 176;
    const int size = 50;

    QSharedPointer<VContainer> data = QSharedPointer<VContainer>(new VContainer(nullptr, &mUnit));
    data->SetHeight(height);
    data->SetSize(size);

    QSharedPointer<VMeasurements> m =
            QSharedPointer<VMeasurements>(new VMeasurements(mUnit, size, height, data.data()));

    const QStringList listSystems = ListPMSystems();
    for (int i = 0; i < listSystems.size(); ++i)
    {
        QString code = listSystems.at(i);
        code.remove(0, 1); // remove 'p'
        m->SetPMSystem(code);

        QTemporaryFile file;
        QString fileName;
        if (file.open())
        {
            fileName = file.fileName();
            file.close();
            file.remove();
            QString error;
            const bool result = m->SaveDocument(fileName, error);

            const QString message = QString("Error: %1 for code=%2").arg(error).arg(listSystems.at(i));
            QVERIFY2(result, qUtf8Printable(message));
        }
        else
        {
            QFAIL("Can't open temporary file.");
        }

        try
        {
            VDomDocument::ValidateXML(VVSTConverter::CurrentSchema, fileName);
        }
        catch (VException &e)
        {
            const QString message = QString("Error: %1 for code=%2").arg(e.ErrorMessage()).arg(listSystems.at(i));
            QFAIL(qUtf8Printable(message));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ValidPMCodesIndividualFile helps to check that all current pattern making systems match pattern inside XSD
 * scheme.
 */
void TST_VMeasurements::ValidPMCodesIndividualFile()
{
    Unit mUnit = Unit::Cm;

    QSharedPointer<VContainer> data = QSharedPointer<VContainer>(new VContainer(nullptr, &mUnit));

    QSharedPointer<VMeasurements> m =
            QSharedPointer<VMeasurements>(new VMeasurements(mUnit, data.data()));

    const QStringList listSystems = ListPMSystems();
    for (int i = 0; i < listSystems.size(); ++i)
    {
        QString code = listSystems.at(i);
        code.remove(0, 1); // remove 'p'
        m->SetPMSystem(code);

        QTemporaryFile file;
        QString fileName;
        if (file.open())
        {
            fileName = file.fileName();
            file.close();
            file.remove();
            QString error;
            const bool result = m->SaveDocument(fileName, error);

            const QString message = QString("Error: %1 for code=%2").arg(error).arg(listSystems.at(i));
            QVERIFY2(result, qUtf8Printable(message));
        }
        else
        {
            QFAIL("Can't open temporary file.");
        }

        try
        {
            VDomDocument::ValidateXML(VVITConverter::CurrentSchema, fileName);
        }
        catch (VException &e)
        {
            const QString message = QString("Error: %1 for code=%2").arg(e.ErrorMessage()).arg(listSystems.at(i));
            QFAIL(qUtf8Printable(message));
        }
    }
}
