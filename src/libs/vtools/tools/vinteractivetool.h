/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 4, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#ifndef VINTERACTIVETOOL_H
#define VINTERACTIVETOOL_H

#include "vabstracttool.h"
#include "../dialogs/tools/dialogtool.h"

#include <QtCore/qglobal.h>

class VInteractiveTool : public VAbstractTool
{
    Q_OBJECT
public:
    VInteractiveTool(VAbstractPattern *doc, VContainer *data, quint32 id, QObject *parent = nullptr);
    virtual ~VInteractiveTool() Q_DECL_EQ_DEFAULT;

    void DialogLinkDestroy();

public slots:
    void FullUpdateFromGuiOk(int result);
    void FullUpdateFromGuiApply();

protected:
    /** @brief m_dialog tool's dialog options.*/
    QSharedPointer<DialogTool> m_dialog;

    /** @brief setDialog set dialog when user want change tool option. */
    virtual void setDialog() {/*do nothing by default*/}
    virtual void SaveDialogChange()=0;

private:
    Q_DISABLE_COPY(VInteractiveTool)
};

#endif // VINTERACTIVETOOL_H
