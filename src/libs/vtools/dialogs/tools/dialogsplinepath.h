/************************************************************************
 **
 **  @file   dialogsplinepath.h
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

#ifndef DIALOGSPLINEPATH_H
#define DIALOGSPLINEPATH_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QSet>
#include <QString>
#include <QVector>
#include <QtGlobal>

#include "../vgeometry/vsplinepath.h"
#include "../vmisc/def.h"
#include "dialogtool.h"

class QCloseEvent;
class QWidget;
class VContainer;
class VSplinePoint;

namespace Ui
{
    class DialogSplinePath;
}

/**
 * @brief The DialogSplinePath class dialog for ToolSplinePath. Help create spline path and edit option.
 */
class DialogSplinePath : public DialogTool
{
    Q_OBJECT
public:
    DialogSplinePath(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogSplinePath() Q_DECL_OVERRIDE;

    VSplinePath GetPath() const;
    void        SetPath(const VSplinePath &value);

    QString GetColor() const;
    void    SetColor(const QString &value);
public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    virtual void ShowDialog(bool click) Q_DECL_OVERRIDE;
    void         PathUpdated(const VSplinePath &path);
protected:
    virtual void ShowVisualization() Q_DECL_OVERRIDE;
    virtual void SaveData() Q_DECL_OVERRIDE;
    virtual void CheckState() Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
private slots:
    void PointChanged(int row);
    void currentPointChanged(int index);

    void DeployAngle1TextEdit();
    void DeployAngle2TextEdit();
    void DeployLength1TextEdit();
    void DeployLength2TextEdit();

    void Angle1Changed();
    void Angle2Changed();
    void Length1Changed();
    void Length2Changed();

    void FXAngle1();
    void FXAngle2();
    void FXLength1();
    void FXLength2();
private:
    Q_DISABLE_COPY(DialogSplinePath)

    /** @brief ui keeps information about user interface */
    Ui::DialogSplinePath *ui;

    /** @brief path spline path */
    VSplinePath path;

    qint32 newDuplicate;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int formulaBaseHeightAngle1;
    int formulaBaseHeightAngle2;
    int formulaBaseHeightLength1;
    int formulaBaseHeightLength2;

    /** @brief flagAngle1 true if value of first angle is correct */
    QVector<bool> flagAngle1;
    QVector<bool> flagAngle2;
    QVector<bool> flagLength1;
    QVector<bool> flagLength2;

    void EvalAngle1();
    void EvalAngle2();
    void EvalLength1();
    void EvalLength2();

    void          NewItem(const VSplinePoint &point);
    void          DataPoint(const VSplinePoint &p);
    void          SavePath();
    QSet<quint32> AllIds() const;
    bool          IsPathValid() const;
    VSplinePath   ExtractPath() const;
    void          ShowPointIssue(const QString &pName);
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetPath return spline path
 * @return path
 */
inline VSplinePath DialogSplinePath::GetPath() const
{
    return path;
}

#endif // DIALOGSPLINEPATH_H
