/************************************************************************
 **
 **  @file   vabstractconverter.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 12, 2014
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

#ifndef VABSTRACTCONVERTER_H
#define VABSTRACTCONVERTER_H

#include <qcompilerdetection.h>

#if !defined(Q_OS_OSX) && !defined(Q_OS_WIN) && defined(Q_CC_GNU)
#include <sys/sysmacros.h>
#endif

#include <QCoreApplication>
#include <QString>
#include <QtGlobal>

#include "vdomdocument.h"

template <class Key, class T> class QMap;

#define CONVERTER_VERSION_CHECK(major, minor, patch) ((major<<16)|(minor<<8)|(patch))

class VAbstractConverter :public VDomDocument
{
    Q_DECLARE_TR_FUNCTIONS(VAbstractConverter)
public:
    explicit VAbstractConverter(const QString &fileName);
    virtual ~VAbstractConverter() Q_DECL_OVERRIDE;

    void         Convert();
    virtual bool SaveDocument(const QString &fileName, QString &error) const Q_DECL_OVERRIDE;

    static int GetVersion(const QString &version);

protected:
    int     ver;
    QString fileName;

    void ValidateInputFile(const QString &currentSchema) const;
    Q_NORETURN void InvalidVersion(int ver) const;
    void Save() const;
    void SetVersion(const QString &version);

    virtual int     MinVer() const =0;
    virtual int     MaxVer() const =0;

    virtual QString MinVerStr() const =0;
    virtual QString MaxVerStr() const =0;

    virtual QString XSDSchema(int ver) const =0;
    virtual void    ApplyPatches() =0;
    virtual void    DowngradeToCurrentMaxVersion() =0;

    void Replace(QString &formula, const QString &newName, int position, const QString &token, int &bias) const;
    void CorrectionsPositions(int position, int bias, QMap<int, QString> &tokens) const;
    static void BiasTokens(int position, int bias, QMap<int, QString> &tokens);

private:
    Q_DISABLE_COPY(VAbstractConverter)

    QString GetVersionStr() const;

    static void ValidateVersion(const QString &version);

    void ReserveFile() const;
    void ReplaceSymLink() const;
};

#endif // VABSTRACTCONVERTER_H
