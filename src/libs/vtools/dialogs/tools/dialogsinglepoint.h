/************************************************************************
 **
 **  @file   dialogsinglepoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef DIALOGSINGLEPOINT_H
#define DIALOGSINGLEPOINT_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>

#include "dialogtool.h"

class QPointF;
class QWidget;
class VContainer;

namespace Ui
{
    class DialogSinglePoint;
}

/**
 * @brief The DialogSinglePoint class dialog for ToolSinglePoint. Help create point and edit option.
 */
class DialogSinglePoint : public DialogTool
{
    Q_OBJECT
public:
    DialogSinglePoint(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogSinglePoint() Q_DECL_OVERRIDE;

    void           SetData(const QString &name, const QPointF &point);
    QPointF        GetPoint()const;

public slots:
    void           mousePress(const QPointF &scenePos);
protected:
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void   SaveData() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(DialogSinglePoint)

    /** @brief ui keeps information about user interface */
    Ui::DialogSinglePoint *ui;

    /** @brief point data of point */
    QPointF        point;
};

#endif // DIALOGSINGLEPOINT_H
