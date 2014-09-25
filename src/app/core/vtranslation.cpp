/************************************************************************
 **
 **  @file   vtranslation.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 5, 2014
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

#include "vtranslation.h"
#include <QCoreApplication>

//---------------------------------------------------------------------------------------------------------------------
VTranslation VTranslation::translate(const QString &context, const QString &sourceText, const QString &disambiguation,
                                     int n)
{
    if (n < 0)
    {
        n = -1;
    }
    VTranslation t(context, sourceText, disambiguation, n);
    return t;
}

//---------------------------------------------------------------------------------------------------------------------
VTranslation::VTranslation()
    :mcontext(QString()), msourceText(QString()), mdisambiguation(QString()), mn(-1)
{}

//---------------------------------------------------------------------------------------------------------------------
VTranslation::VTranslation(const QString &context, const QString &sourceText, const QString &disambiguation, int n)
    :mcontext(context), msourceText(sourceText), mdisambiguation(disambiguation), mn(n)
{}

//---------------------------------------------------------------------------------------------------------------------
VTranslation &VTranslation::operator=(const VTranslation &tr)
{
    if ( &tr == this )
    {
        return *this;
    }
    this->mcontext = tr.getMcontext();
    this->msourceText = tr.getMsourceText();
    this->mdisambiguation = tr.getMdisambiguation();
    this->mn = tr.getN();
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VTranslation::VTranslation(const VTranslation &tr)
    :mcontext(tr.getMcontext()), msourceText(tr.getMsourceText()), mdisambiguation(tr.getMdisambiguation()),
      mn(tr.getN())
{}

//---------------------------------------------------------------------------------------------------------------------
QString VTranslation::VTranslation::translate() const
{
    return QCoreApplication::translate(mcontext.toUtf8().constData(), msourceText.toUtf8().constData(),
                                       mdisambiguation.toUtf8().constData(), mn);
}
