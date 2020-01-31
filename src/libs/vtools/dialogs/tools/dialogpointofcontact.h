/************************************************************************
 **
 **  @file   dialogpointofcontact.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#ifndef DIALOGPOINTOFCONTACT_H
#define DIALOGPOINTOFCONTACT_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "dialogtool.h"
#include "ui_dialogpointofcontact.h"

namespace Ui
{
    class DialogPointOfContact;
}

/**
 * @brief The DialogPointOfContact class dialog for ToolPointOfContact. Help create point and edit option.
 */
class DialogPointOfContact : public DialogTool
{
    Q_OBJECT
public:
    DialogPointOfContact(const VContainer *data, quint32 toolId, QWidget *parent = nullptr);
    virtual ~DialogPointOfContact() override;

    QString        GetPointName() const;
    void           SetPointName(const QString &value);

    QString        getRadius() const;
    void           setRadius(const QString &value);

    quint32        getCenter() const;
    void           setCenter(quint32 value);

    quint32        GetFirstPoint() const;
    void           SetFirstPoint(quint32 value);

    quint32        GetSecondPoint() const;
    void           SetSecondPoint(quint32 value);
public slots:
    virtual void   ChosenObject(quint32 id, const SceneObject &type) override;
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void           DeployFormulaTextEdit();
    virtual void   PointNameChanged() override;
    void           FXRadius();
    void           EvalFormula();
protected:
    virtual void   ShowVisualization() override;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void   SaveData() override;
    virtual void   closeEvent(QCloseEvent *event) override;
    virtual bool   IsValid() const final;
private:
    Q_DISABLE_COPY(DialogPointOfContact)

    /** @brief ui keeps information about user interface */
    Ui::DialogPointOfContact *ui;

    /** @brief radius radius of arc */
    QString radius;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int formulaBaseHeight;

    QString pointName;

    QTimer *timerFormula;

    bool flagFormula;
    bool flagName;
    bool flagError;
};

//---------------------------------------------------------------------------------------------------------------------
inline bool DialogPointOfContact::IsValid() const
{
    return flagFormula && flagName && flagError;
}

#endif // DIALOGPOINTOFCONTACT_H
