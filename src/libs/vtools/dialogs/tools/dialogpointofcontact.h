/************************************************************************
 **
 **  @file   dialogpointofcontact.h
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

#ifndef DIALOGPOINTOFCONTACT_H
#define DIALOGPOINTOFCONTACT_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "dialogs/tools/../support/../tools/dialogtool.h"
#include "dialogtool.h"
#include "ui_dialogpointofcontact.h"

class QCloseEvent;
class QWidget;
class VContainer;

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
    DialogPointOfContact(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogPointOfContact() Q_DECL_OVERRIDE;

    void           SetPointName(const QString &value);

    QString        getRadius() const;
    void           setRadius(const QString &value);

    quint32        getCenter() const;
    void           setCenter(const quint32 &value);

    quint32        GetFirstPoint() const;
    void           SetFirstPoint(const quint32 &value);

    quint32        GetSecondPoint() const;
    void           SetSecondPoint(const quint32 &value);
public slots:
    virtual void   ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void           DeployFormulaTextEdit();
    /**
     * @brief FormulaTextChanged when formula text changes for validation and calc
     */
    void           FormulaTextChanged();
    virtual void   PointNameChanged() Q_DECL_OVERRIDE;
    void           FXRadius();
protected:
    virtual void   ShowVisualization() Q_DECL_OVERRIDE;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void   SaveData() Q_DECL_OVERRIDE;
    virtual void   closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(DialogPointOfContact)

    /** @brief ui keeps information about user interface */
    Ui::DialogPointOfContact *ui;

    /** @brief radius radius of arc */
    QString        radius;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int             formulaBaseHeight;
};

#endif // DIALOGPOINTOFCONTACT_H
