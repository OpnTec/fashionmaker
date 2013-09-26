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

#ifndef DIALOGBISECTOR_H
#define DIALOGBISECTOR_H

#include "dialogtool.h"
#include <QPushButton>
#include <QListWidgetItem>
#include <QTimer>
#include "options.h"
#include "container/vcontainer.h"
#include "container/calculator.h"

namespace Ui {
class DialogBisector;
}

class DialogBisector : public DialogTool
{
    Q_OBJECT
    
public:
    explicit DialogBisector(const VContainer *data, Draw::Mode mode = Draw::Calculation,
                            QWidget *parent = 0);
    ~DialogBisector();
    QString           getPointName() const;
    void              setPointName(const QString &value);
    QString           getTypeLine() const;
    void              setTypeLine(const QString &value);
    QString           getFormula() const;
    void              setFormula(const QString &value);
    qint64            getFirstPointId() const;
    void              setFirstPointId(const qint64 &value, const qint64 &id);
    qint64            getSecondPointId() const;
    void              setSecondPointId(const qint64 &value, const qint64 &id);
    qint64            getThirdPointId() const;
    void              setThirdPointId(const qint64 &value, const qint64 &id);
public slots:
    virtual void      ChoosedObject(qint64 id, Scene::Type type);
    virtual void      DialogAccepted();
private:
    Q_DISABLE_COPY(DialogBisector)
    Ui::DialogBisector *ui;
    qint32             number;
    QString            pointName;
    QString            typeLine;
    QString            formula;
    qint64             firstPointId;
    qint64             secondPointId;
    qint64             thirdPointId;
};

#endif // DIALOGBISECTOR_H
