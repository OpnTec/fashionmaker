/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef DIALOGPOINTOFCONTACT_H
#define DIALOGPOINTOFCONTACT_H

#include "ui_dialogpointofcontact.h"
#include "dialogtool.h"
#include "container/calculator.h"

class DialogPointOfContact : public DialogTool
{
    Q_OBJECT
    
public:
    explicit DialogPointOfContact(const VContainer *data, Draw::Draws mode = Draw::Calculation,
                                  QWidget *parent = 0);
    QString getPointName() const;
    void setPointName(const QString &value);

    QString getRadius() const;
    void setRadius(const QString &value);

    qint64 getCenter() const;
    void setCenter(const qint64 &value, const qint64 &id);

    qint64 getFirstPoint() const;
    void setFirstPoint(const qint64 &value, const qint64 &id);

    qint64 getSecondPoint() const;
    void setSecondPoint(const qint64 &value, const qint64 &id);

public slots:
    virtual void            ChoosedObject(qint64 id, Scene::Scenes type);
    virtual void            DialogAccepted();
private:
    Q_DISABLE_COPY(DialogPointOfContact)
    Ui::DialogPointOfContact ui;
    qint32                  number;
    QString                 pointName;
    QString                 radius;
    qint64                  center;
    qint64                  firstPoint;
    qint64                  secondPoint;
};

#endif // DIALOGPOINTOFCONTACT_H
