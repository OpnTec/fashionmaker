/************************************************************************
 **
 **  @file   dialogpatternproperties.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   18 2, 2014
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

#ifndef DIALOGPATTERNPROPERTIES_H
#define DIALOGPATTERNPROPERTIES_H

#include <QDialog>
#include "../../xml/vdomdocument.h"

namespace Ui {
class DialogPatternProperties;
}

class DialogPatternProperties : public QDialog
{
    Q_OBJECT
public:
    DialogPatternProperties(VDomDocument *doc, QWidget *parent = 0);
    virtual ~DialogPatternProperties();
signals:
    void         haveChange();
public slots:
    void         Apply();
private:
    Q_DISABLE_COPY(DialogPatternProperties)
    Ui::DialogPatternProperties *ui;
    VDomDocument *doc;
    void         Write(const QString &tagName, const QString &text) const;
};

#endif // DIALOGPATTERNPROPERTIES_H
