/************************************************************************
 **
 **  @file   dialogcutsplinrpath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
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

#ifndef DIALOGCUTSPLINEPATH_H
#define DIALOGCUTSPLINEPATH_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "dialogs/tools/../support/../tools/dialogtool.h"
#include "dialogtool.h"

class QCloseEvent;
class QWidget;
class VContainer;

namespace Ui
{
    class DialogCutSplinePath;
}

/**
 * @brief The DialogCutSplinePath class dialog for ToolCutSplinePath.
 */
class DialogCutSplinePath : public DialogTool
{
    Q_OBJECT
public:
    DialogCutSplinePath(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogCutSplinePath() Q_DECL_OVERRIDE;

    void         SetPointName(const QString &value);

    QString      GetFormula() const;
    void         SetFormula(const QString &value);

    quint32      getSplinePathId() const;
    void         setSplinePathId(const quint32 &value);
public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void         DeployFormulaTextEdit();
    void         FXLength();
protected:
    virtual void ShowVisualization() Q_DECL_OVERRIDE;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void SaveData() Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(DialogCutSplinePath)

    /** @brief ui keeps information about user interface */
    Ui::DialogCutSplinePath *ui;

    /** @brief formula string with formula */
    QString      formula;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int formulaBaseHeight;
};

#endif // DIALOGCUTSPLINEPATH_H
