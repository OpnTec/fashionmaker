/************************************************************************
 **
 **  @file   dialognormal.h
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

#ifndef DIALOGNORMAL_H
#define DIALOGNORMAL_H

#include "dialogtool.h"

namespace Ui
{
    class DialogNormal;
}

/**
 * @brief The DialogNormal class dialog for ToolNormal. Help create point and edit option.
 */
class DialogNormal : public DialogTool
{
    Q_OBJECT
public:
    DialogNormal(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogNormal() Q_DECL_OVERRIDE;

    void             SetPointName(const QString &value);

    QString          GetTypeLine() const;
    void             SetTypeLine(const QString &value);

    QString          GetFormula() const;
    void             SetFormula(const QString &value);

    qreal            GetAngle() const;
    void             SetAngle(const qreal &value);

    quint32          GetFirstPointId() const;
    void             SetFirstPointId(const quint32 &value);

    quint32          GetSecondPointId() const;
    void             SetSecondPointId(const quint32 &value);

    QString          GetLineColor() const;
    void             SetLineColor(const QString &value);
public slots:
    virtual void     ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void             DeployFormulaTextEdit();
    /**
     * @brief FormulaTextChanged when formula text changes for validation and calc
     */
    void             FormulaTextChanged();
    virtual void     PointNameChanged() Q_DECL_OVERRIDE;
    void             FXLength();
protected:
    virtual void     ShowVisualization() Q_DECL_OVERRIDE;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void     SaveData() Q_DECL_OVERRIDE;
    virtual void     closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(DialogNormal)

    /** @brief ui keeps information about user interface */
    Ui::DialogNormal *ui;

    /** @brief formula formula */
    QString          formula;

    /** @brief angle aditional angle of normal */
    qreal            angle;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int              formulaBaseHeight;
};

#endif // DIALOGNORMAL_H
