/************************************************************************
 **
 **  @file   dialogbisector.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef DIALOGBISECTOR_H
#define DIALOGBISECTOR_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui
{
    class DialogBisector;
}

/**
 * @brief The DialogBisector class dialog for ToolBisector. Help create point and edit option.
 */
class DialogBisector : public DialogTool
{
    Q_OBJECT
public:

    DialogBisector(const VContainer *data, quint32 toolId, QWidget *parent = nullptr);
    virtual ~DialogBisector() override;

    QString            GetPointName() const;
    void               SetPointName(const QString &value);

    QString            GetTypeLine() const;
    void               SetTypeLine(const QString &value);

    QString            GetFormula() const;
    void               SetFormula(const QString &value);

    quint32            GetFirstPointId() const;
    void               SetFirstPointId(const quint32 &value);

    quint32            GetSecondPointId() const;
    void               SetSecondPointId(const quint32 &value);

    quint32            GetThirdPointId() const;
    void               SetThirdPointId(const quint32 &value);

    QString            GetLineColor() const;
    void               SetLineColor(const QString &value);
public slots:
    virtual void       ChosenObject(quint32 id, const SceneObject &type) override;
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void               DeployFormulaTextEdit();

    virtual void       PointNameChanged() override;
    void               FXLength();
    void               EvalFormula();
protected:
    virtual void       ShowVisualization() override;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void       SaveData() override;
    virtual void       closeEvent(QCloseEvent *event) override;
    virtual bool       IsValid() const final;
private:
    Q_DISABLE_COPY(DialogBisector)

    /** @brief ui keeps information about user interface */
    Ui::DialogBisector *ui;

    /** @brief formula formula */
    QString formula;
    QString pointName;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int                formulaBaseHeight;

    QTimer *timerFormula;

    bool flagFormula;
    bool flagError;
    bool flagName;
};

//---------------------------------------------------------------------------------------------------------------------
inline bool DialogBisector::IsValid() const
{
    return flagFormula && flagError && flagName;
}

#endif // DIALOGBISECTOR_H
