/************************************************************************
 **
 **  @file   tst_measurementregexp.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 9, 2015
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

#include "tst_measurementregexp.h"
#include "../qmuparser/qmudef.h"
#include "../vmisc/def.h"
#include "../vpatterndb/vtranslatemeasurements.h"

#include <QtTest>
#include <QTranslator>

enum ErrorState {ErrorLoad = 0, ErrorInstall, ErrorSize, NoError};

//---------------------------------------------------------------------------------------------------------------------
TST_MeasurementRegExp::TST_MeasurementRegExp(QObject *parent)
    :QObject(parent),
      pmsTranslator(nullptr),
      trMs(nullptr)
{
}

//---------------------------------------------------------------------------------------------------------------------
TST_MeasurementRegExp::~TST_MeasurementRegExp()
{
    delete pmsTranslator;
    delete trMs;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_MeasurementRegExp::TestOriginalMeasurementNamesRegExp()
{
    const QStringList originalNames = OriginalNames();
    const QRegularExpression re(NameRegExp());

    foreach(const QString &str, originalNames)
    {
        QCOMPARE(re.match(str).hasMatch(), true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void TST_MeasurementRegExp::TestMeasurementRegExp()
{
    const int systemCounts = 55;
    const QStringList locales = SupportedLocales();

    {
        const int combinations = systemCounts * locales.size(); // 55*12=660

        QDir dir(TranslationsPath());
        const QStringList fileNames = dir.entryList(QStringList("measurements_p*_*.qm"));

        QVERIFY2(combinations == fileNames.size(), "Unexpected count of files.");
    }

    for(int s = 0; s < systemCounts; ++s)
    {
        for(int l = 0, sz = locales.size(); l < sz; ++l)
        {
            const int res = LoadTranslation(QString("p%1").arg(s), locales.at(l));

            switch(res)
            {
                case ErrorInstall:
                case ErrorSize:
                case ErrorLoad:
                {
                    const QString message = QString("Failed to check translation for system = p%1 and locale = %2")
                            .arg(s)
                            .arg(locales.at(l));
                    QFAIL(message.toUtf8().constData());
                    break;
                }
                case NoError:
                {
                    CheckNames();

                    if (not pmsTranslator.isNull())
                    {
                        const bool result = QCoreApplication::removeTranslator(pmsTranslator);

                        if (result == false)
                        {
                            const QString message = QString("Can't remove translation for system = p%1 and locale = %2")
                                    .arg(s)
                                    .arg(locales.at(l));
                            QWARN(message.toUtf8().constData());
                        }
                        delete pmsTranslator;
                    }
                }
                default:
                    QWARN("Unexpected state");
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString TST_MeasurementRegExp::TranslationsPath() const
{
    return QApplication::applicationDirPath() + QStringLiteral("/../../../app/valentina/bin/translations");
}

//---------------------------------------------------------------------------------------------------------------------
int TST_MeasurementRegExp::LoadTranslation(const QString &checkedSystem, const QString &checkedLocale)
{
    const QString path = TranslationsPath();
    const QString file = QString("measurements_%1_%2.qm").arg(checkedSystem).arg(checkedLocale);

    if (QFileInfo(path+"/"+file).size() <= 34)
    {
        const QString message = QString("Translation for system = %1 and locale = %2 is empty. \nFull path: %3/%4")
                .arg(checkedSystem)
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(message.toUtf8().constData());

        return ErrorSize;
    }

    pmsTranslator = new QTranslator(this);

    if (not pmsTranslator->load(file, path))
    {
        const QString message = QString("Can't load translation for system = %1 and locale = %2. \nFull path: %3/%4")
                .arg(checkedSystem)
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(message.toUtf8().constData());

        delete pmsTranslator;

        return ErrorLoad;
    }

    if (not QCoreApplication::installTranslator(pmsTranslator))
    {
        const QString message = QString("Can't install translation for system = %1 and locale = %2. \nFull path: %3/%4")
                .arg(checkedSystem)
                .arg(checkedLocale)
                .arg(path)
                .arg(file);
        QWARN(message.toUtf8().constData());

        delete pmsTranslator;

        return ErrorInstall;
    }

    InitTrMs();//Very important do it after load QM file.

    return NoError;
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::InitTrMs()
{
    if (trMs != nullptr)
    {
        trMs->Retranslate();
    }
    else
    {
        trMs = new VTranslateMeasurements();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TST_MeasurementRegExp::CheckNames() const
{
    const QStringList originalNames = OriginalNames();
    const QRegularExpression re(NameRegExp());

    foreach(const QString &str, originalNames)
    {
        const QString translated = trMs->MToUser(str);
        QCOMPARE(re.match(translated).hasMatch(), true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QStringList TST_MeasurementRegExp::OriginalNames() const
{
    const QStringList originalNames = QStringList() << ListGroupA()
                                                    << ListGroupB()
                                                    << ListGroupC()
                                                    << ListGroupD()
                                                    << ListGroupE()
                                                    << ListGroupF()
                                                    << ListGroupG()
                                                    << ListGroupH()
                                                    << ListGroupI()
                                                    << ListGroupJ()
                                                    << ListGroupK()
                                                    << ListGroupL()
                                                    << ListGroupM()
                                                    << ListGroupN()
                                                    << ListGroupO()
                                                    << ListGroupP()
                                                    << ListGroupQ();

    return originalNames;
}
