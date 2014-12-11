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
 **  Copyright (C) 2014 Valentina project
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

#include "vdomdocument.h"

class VAbstractConverter :public VDomDocument
{

public:
    VAbstractConverter(const QString &fileName);
    virtual ~VAbstractConverter();

protected:
    int     ver;

    int  GetVersion(const QString &version) const;
    void CheckVersion(int ver) const;

    virtual int     MinVer() const =0;
    virtual int     MaxVer() const =0;

    virtual QString MinVerStr() const =0;
    virtual QString MaxVerStr() const =0;

    virtual QString XSDSchema(int ver) const =0;

private:
    Q_DISABLE_COPY(VAbstractConverter)
    QString fileName;

    QString GetVersionStr() const;

    void    ValidateVersion(const QString &version) const;
};

#endif // VABSTRACTCONVERTER_H
