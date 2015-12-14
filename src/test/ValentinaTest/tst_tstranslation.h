/************************************************************************
 **
 **  @file   tst_tstranslation.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 12, 2015
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

#ifndef TST_TSTRANSLATION_H
#define TST_TSTRANSLATION_H

#include <QDomDocument>
#include <QFile>
#include <QObject>
#include <QSharedPointer>

class TST_TSTranslation : public QObject
{
    Q_OBJECT
public:
    explicit TST_TSTranslation(QObject *parent = 0);

private slots:
    void CheckEnglishLocalization();
    void CheckEmptyToolButton();

private:
    Q_DISABLE_COPY(TST_TSTranslation)
    QSharedPointer<QFile> tsFile;
    QSharedPointer<QDomDocument> tsXML;

    static const QString TagName;
    static const QString TagMessage;
    static const QString TagSource;
    static const QString TagTranslation;

    static const QString AttrType;
    static const QString AttrValVanished;
    static const QString AttrValUnfinished;

    QDomNodeList LoadTSFile(const QString &filename);
};

#endif // TST_TSTRANSLATION_H
