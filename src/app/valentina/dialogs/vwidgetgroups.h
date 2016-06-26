/************************************************************************
 **
 **  @file   vwidgetgroups.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 4, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef VWIDGETGROUPS_H
#define VWIDGETGROUPS_H

#include <QWidget>
#include "../ifc/xml/vabstractpattern.h"

namespace Ui
{
    class VWidgetGroups;
}

class VWidgetGroups : public QWidget
{
    Q_OBJECT

public:
    explicit VWidgetGroups(VAbstractPattern *doc, QWidget *parent = nullptr);
    virtual ~VWidgetGroups();

public slots:
    void UpdateGroups();

private slots:
    void GroupVisibilityChanged(int row, int column);
    void RenameGroup(int row, int column);
    void CtxMenu(const QPoint &pos);
private:
    Q_DISABLE_COPY(VWidgetGroups)
    Ui::VWidgetGroups *ui;
    VAbstractPattern *doc;

    void FillTable(const QMap<quint32, QPair<QString, bool> > &groups);
};

#endif // VWIDGETGROUPS_H
