/************************************************************************
 **
 **  @file   vlabeltemplate.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 8, 2015
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

#ifndef VLABELTEMPLATE_H
#define VLABELTEMPLATE_H

#include "../ifc/xml/vdomdocument.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

struct VLabelTemplateLine
{
    QString line;
    bool    bold;
    bool    italic;
    int     alignment;
};

QT_WARNING_POP

class VLabelTemplate : public VDomDocument
{
public:
    VLabelTemplate();
    virtual ~VLabelTemplate() Q_DECL_EQ_DEFAULT;

    static const QString TagTemplate;
    static const QString TagLines;
    static const QString TagLine;

    static const QString AttrText;
    static const QString AttrBold;
    static const QString AttrItalic;
    static const QString AttrAlignment;

    void CreateEmptyTemplate();

    void AddLines(const QVector<VLabelTemplateLine> &lines);
private:
    Q_DISABLE_COPY(VLabelTemplate)
};

#endif // VLABELTEMPLATE_H
