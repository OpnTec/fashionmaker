/************************************************************************
 **
 **  @file   dialogspline.h
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

#ifndef DIALOGSPLINE_H
#define DIALOGSPLINE_H

#include "dialogtool.h"
#include "../vgeometry/vspline.h"

namespace Ui
{
    class DialogSpline;
}

/**
 * @brief The DialogSpline class dialog for ToolSpline. Help create spline and edit option.
 */
class DialogSpline : public DialogTool
{
    Q_OBJECT
public:
    DialogSpline(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogSpline() Q_DECL_OVERRIDE;

    VSpline GetSpline() const;
    void    SetSpline(const VSpline &spline);

    QString GetColor() const;
    void    SetColor(const QString &value);
public slots:
    virtual void  ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    virtual void  PointNameChanged() Q_DECL_OVERRIDE;
    virtual void  ShowDialog(bool click) Q_DECL_OVERRIDE;
protected:
    virtual void  ShowVisualization() Q_DECL_OVERRIDE;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void  SaveData() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(DialogSpline)

    /** @brief ui keeps information about user interface */
    Ui::DialogSpline *ui;

    /** @brief spl spline */
    VSpline spl;

    const QSharedPointer<VPointF> GetP1() const;
    const QSharedPointer<VPointF> GetP4() const;
};

#endif // DIALOGSPLINE_H
