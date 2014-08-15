/************************************************************************
 **
 **  @file   vlinelength.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 7, 2014
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

#ifndef VLINELENGTH_H
#define VLINELENGTH_H

#include "vinternalvariable.h"

class VPointF;

class VLengthLine :public VInternalVariable
{
public:
    VLengthLine();
    VLengthLine(const VPointF *p1, const quint32 &p1Id, const VPointF *p2, const quint32 &p2Id);
    VLengthLine(const VLengthLine &var);
    VLengthLine &operator=(const VLengthLine &var);
    virtual ~VLengthLine();

    virtual bool Filter(quint32 id);
    void         SetValue(const VPointF *p1, const VPointF *p2);
    quint32      GetP1Id() const;
    quint32      GetP2Id() const;
private:
    quint32 p1Id;
    quint32 p2Id;
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 VLengthLine::GetP1Id() const
{
    return p1Id;
}

//---------------------------------------------------------------------------------------------------------------------
inline quint32 VLengthLine::GetP2Id() const
{
    return p2Id;
}

#endif // VLINELENGTH_H
