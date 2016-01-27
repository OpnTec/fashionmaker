/************************************************************************
 **
 **  @file   dialogpointofintersectionarcs.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef DIALOGPOINTOFINTERSECTIONARCS_H
#define DIALOGPOINTOFINTERSECTIONARCS_H

#include "dialogtool.h"

namespace Ui
{
    class DialogPointOfIntersectionArcs;
}

class DialogPointOfIntersectionArcs : public DialogTool
{
    Q_OBJECT

public:
    DialogPointOfIntersectionArcs(const VContainer *data, const quint32 &toolId, QWidget *parent = 0);
    virtual ~DialogPointOfIntersectionArcs() Q_DECL_OVERRIDE;

    void           SetPointName(const QString &value);

    quint32        GetFirstArcId() const;
    void           SetFirstArcId(const quint32 &value);

    quint32        GetSecondArcId() const;
    void           SetSecondArcId(const quint32 &value);

    CrossCirclesPoint GetCrossArcPoint() const;
    void              SetCrossArcPoint(const CrossCirclesPoint &p);

public slots:
    virtual void   ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    virtual void   ArcChanged();

protected:
    virtual void   ShowVisualization() Q_DECL_OVERRIDE;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void   SaveData() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(DialogPointOfIntersectionArcs)

    Ui::DialogPointOfIntersectionArcs *ui;
};

#endif // DIALOGPOINTOFINTERSECTIONARCS_H
