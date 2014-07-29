/************************************************************************
 **
 **  @file   dialogcutarc.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2014
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

#ifndef DIALOGCUTARC_H
#define DIALOGCUTARC_H

#include "dialogtool.h"

namespace Ui
{
    class DialogCutArc;
}

/**
 * @brief The DialogCutArc class dialog for ToolCutArc.
 */
class DialogCutArc : public DialogTool
{
    Q_OBJECT
public:

    DialogCutArc(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    ~DialogCutArc();

    QString           getPointName() const;
    void              setPointName(const QString &value);

    QString           getFormula() const;
    void              setFormula(const QString &value);

    quint32           getArcId() const;
    void              setArcId(const quint32 &value, const quint32 &id);
public slots:
    virtual void      ChosenObject(quint32 id, const SceneObject &type);
    virtual void      DialogAccepted();
    /**
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void      DialogApply();
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void DeployFormulaTextEdit();
    /**
     * @brief FormulaTextChanged when formula text changes for validation and calc
     */
    void FormulaTextChanged();
private:
    Q_DISABLE_COPY(DialogCutArc)
    /** @brief ui keeps information about user interface */
    Ui::DialogCutArc  *ui;

    /** @brief pointName name of created point */
    QString           pointName;

    /** @brief formula string with formula */
    QString           formula;

    /** @brief arcId keep id of arc */
    quint32            arcId;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int formulaBaseHeight;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    void SaveData();
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getPointName return name point on arc
 * @return name
 */
inline QString DialogCutArc::getPointName() const
{
    return pointName;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getArcId return id of arc
 * @return id
 */
inline quint32 DialogCutArc::getArcId() const
{
    return arcId;
}

#endif // DIALOGCUTARC_H
