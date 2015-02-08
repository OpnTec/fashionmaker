/************************************************************************
 **
 **  @file   dialogshoulderpoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef DIALOGSHOULDERPOINT_H
#define DIALOGSHOULDERPOINT_H

#include "dialogtool.h"

namespace Ui
{
    class DialogShoulderPoint;
}

class VisToolShoulderPoint;

/**
 * @brief The DialogShoulderPoint class dialog for ToolShoulderPoint. Help create point and edit option.
 */
class DialogShoulderPoint : public DialogTool
{
    Q_OBJECT
public:
    DialogShoulderPoint(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    ~DialogShoulderPoint();

    void           SetPointName(const QString &value);

    QString        GetTypeLine() const;
    void           SetTypeLine(const QString &value);

    QString        GetFormula() const;
    void           SetFormula(const QString &value);

    quint32        GetP1Line() const;
    void           SetP1Line(const quint32 &value);

    quint32        GetP2Line() const;
    void           SetP2Line(const quint32 &value);

    quint32        GetP3() const;
    void           SetP3(const quint32 &value);

    QString        GetLineColor() const;
    void           SetLineColor(const QString &value);
public slots:
    virtual void   ChosenObject(quint32 id, const SceneObject &type);
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void           DeployFormulaTextEdit();
    /**
     * @brief FormulaTextChanged when formula text changes for validation and calc
     */
    void           FormulaTextChanged();
    virtual void   PointNameChanged();
protected:
    virtual void   ShowVisualization();
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void   SaveData();
    virtual void   closeEvent(QCloseEvent *event);
private:
    Q_DISABLE_COPY(DialogShoulderPoint)

    /** @brief ui keeps information about user interface */
    Ui::DialogShoulderPoint *ui;

    /** @brief formula formula */
    QString        formula;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int             formulaBaseHeight;
    VisToolShoulderPoint *line;
};

#endif // DIALOGSHOULDERPOINT_H
