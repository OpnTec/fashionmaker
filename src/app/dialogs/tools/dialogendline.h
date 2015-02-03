/************************************************************************
 **
 **  @file   dialogendline.h
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

#ifndef DIALOGENDLINE_H
#define DIALOGENDLINE_H

#include "dialogtool.h"

namespace Ui
{
    class DialogEndLine;
}

class VisToolEndLine;

/**
 * @brief The DialogEndLine class dialog for ToolEndLine. Help create point and edit option.
 */
class DialogEndLine : public DialogTool
{
    Q_OBJECT
public:
    DialogEndLine(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    ~DialogEndLine();

    void              setPointName(const QString &value);

    QString           getTypeLine() const;
    void              setTypeLine(const QString &value);

    QString           getFormula() const;
    void              setFormula(const QString &value);

    QString           getAngle() const;
    void              setAngle(const QString &value);

    quint32           getBasePointId() const;
    void              setBasePointId(const quint32 &value);

    virtual void      ShowDialog(bool click);
public slots:
    virtual void      ChosenObject(quint32 id, const SceneObject &type);
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void              DeployFormulaTextEdit();
    /**
     * @brief FormulaTextChanged when formula text changes for validation and calc
     */
    void             FormulaTextChanged();
    void             PutAngle();
    void             EvalAngle();
    void             AngleTextChanged();
    void             DeployAngleTextEdit();
protected:
    virtual void     ShowVisualization();
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void     SaveData();
    virtual void     closeEvent(QCloseEvent *event);
private:
    Q_DISABLE_COPY(DialogEndLine)

    /** @brief ui keeps information about user interface */
    Ui::DialogEndLine *ui;

    /** @brief typeLine type of line */
    QString           typeLine;

    /** @brief formula formula */
    QString           formulaLength;

    /** @brief angle angle of line */
    QString           formulaAngle;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int               formulaBaseHeight;
    int               formulaBaseHeightAngle;

    VisToolEndLine    *line;
};

#endif // DIALOGENDLINE_H
