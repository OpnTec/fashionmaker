/************************************************************************
 **
 **  @file   dialogcubicbezier.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 3, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
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

#ifndef DIALOGCUBICBEZIER_H
#define DIALOGCUBICBEZIER_H

#include "dialogtool.h"
#include "../vgeometry/vcubicbezier.h"

namespace Ui
{
    class DialogCubicBezier;
}

class DialogCubicBezier : public DialogTool
{
    Q_OBJECT

public:
    explicit DialogCubicBezier(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogCubicBezier();

    VCubicBezier GetSpline() const;
    void         SetSpline(const VCubicBezier &spline);

    QString GetColor() const;
    void    SetColor(const QString &value);
public slots:
    virtual void  ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    virtual void  PointNameChanged() Q_DECL_OVERRIDE;
protected:
    virtual void  ShowVisualization() Q_DECL_OVERRIDE;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void  SaveData() Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(DialogCubicBezier)
    Ui::DialogCubicBezier *ui;

    /** @brief spl spline */
    VCubicBezier spl;

    qint32 newDuplicate;

    const QSharedPointer<VPointF> GetP1() const;
    const QSharedPointer<VPointF> GetP2() const;
    const QSharedPointer<VPointF> GetP3() const;
    const QSharedPointer<VPointF> GetP4() const;
};

#endif // DIALOGCUBICBEZIER_H
