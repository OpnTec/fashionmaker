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

#ifndef DIALOGENDLINE_H
#define DIALOGENDLINE_H

#include "dialogtool.h"
#include <QPushButton>
#include <QListWidgetItem>
#include <QTimer>
#include "options.h"
#include "container/vcontainer.h"
#include "container/calculator.h"

namespace Ui {
class DialogEndLine;
}

class DialogEndLine : public DialogTool
{
    Q_OBJECT 
public:
    explicit          DialogEndLine(const VContainer *data, Draw::Mode mode = Draw::Calculation,
                                    QWidget *parent = 0);
                      ~DialogEndLine();
    QString           getPointName() const;
    void              setPointName(const QString &value);
    QString           getTypeLine() const;
    void              setTypeLine(const QString &value);
    QString           getFormula() const;
    void              setFormula(const QString &value);
    qreal             getAngle() const;
    void              setAngle(const qreal &value);
    qint64            getBasePointId() const;
    void              setBasePointId(const qint64 &value, const qint64 &id);
public slots:
    virtual void      ChoosedObject(qint64 id, Scene::Type type);
    virtual void      DialogAccepted();
private:
    Q_DISABLE_COPY(DialogEndLine)
    Ui::DialogEndLine *ui;
    QString           pointName;
    QString           typeLine;
    QString           formula;
    qreal             angle;
    qint64            basePointId;
};

#endif // DIALOGENDLINE_H
