/************************************************************************
 **
 **  @file   dialogrotation.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 4, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef DIALOGROTATION_H
#define DIALOGROTATION_H

#include <qcompilerdetection.h>
#include <QList>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui
{
    class DialogRotation;
}

class DialogRotation : public DialogTool
{
    Q_OBJECT
public:
    explicit DialogRotation(const VContainer *data, quint32 toolId, QWidget *parent = nullptr);
    virtual ~DialogRotation();

    quint32 GetOrigPointId() const;
    void    SetOrigPointId(quint32 value);

    QString GetAngle() const;
    void    SetAngle(const QString &value);

    QString GetSuffix() const;
    void    SetSuffix(const QString &value);

    QVector<quint32> GetObjects() const;

    virtual void ShowDialog(bool click) override;

public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) override;
    virtual void SelectedObject(bool selected, quint32 object, quint32 tool) override;

private slots:
    /** @brief DeployAngleTextEdit grow or shrink formula input */
    void DeployAngleTextEdit();
    void FXAngle();
    void SuffixChanged();
    void EvalAngle();

protected:
    virtual void ShowVisualization() override;

    /** @brief SaveData Put dialog data in local variables */
    virtual void SaveData() override;
    virtual void closeEvent(QCloseEvent *event) override;
    virtual bool IsValid() const final;

private slots:
    void PointChanged();

private:
    Q_DISABLE_COPY(DialogRotation)
    Ui::DialogRotation *ui;

    /** @brief timerAngle timer of check formula of angle */
    QTimer  *timerAngle;

    /** @brief angle formula of angle */
    QString formulaAngle;

    /** @brief formulaBaseHeightAngle base height defined by dialogui */
    int     formulaBaseHeightAngle;

    QList<quint32> objects;

    bool stage1;

    QString m_suffix;

    bool m_firstRelease;

    /** @brief flagAngle true if value of angle is correct */
    bool flagAngle;
    bool flagName;
    bool flagError;
};

//---------------------------------------------------------------------------------------------------------------------
inline bool DialogRotation::IsValid() const
{
    return flagAngle && flagName && flagError;
}

#endif // DIALOGROTATION_H
