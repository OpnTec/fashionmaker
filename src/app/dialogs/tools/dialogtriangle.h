/************************************************************************
 **
 **  @file   dialogtriangle.h
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

#ifndef DIALOGTRIANGLE_H
#define DIALOGTRIANGLE_H

#include "dialogtool.h"

namespace Ui
{
    class DialogTriangle;
}

class VisToolTriangle;

/**
 * @brief The DialogTriangle class dialog for ToolTriangle. Help create point and edit option.
 */
class DialogTriangle : public DialogTool
{
    Q_OBJECT
public:
    DialogTriangle(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    ~DialogTriangle();

    quint32        getAxisP1Id() const;
    void           setAxisP1Id(const quint32 &value);

    quint32        getAxisP2Id() const;
    void           setAxisP2Id(const quint32 &value);

    quint32        getFirstPointId() const;
    void           setFirstPointId(const quint32 &value);

    quint32        getSecondPointId() const;
    void           setSecondPointId(const quint32 &value);

    void           setPointName(const QString &value);
public slots:
    virtual void   ChosenObject(quint32 id, const SceneObject &type);
    virtual void   PointNameChanged();
    virtual void   UpdateList();
protected:
    virtual void   ShowVisualization();
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void   SaveData();
private:
    Q_DISABLE_COPY(DialogTriangle)

    /** @brief ui keeps information about user interface */
    Ui::DialogTriangle *ui;

    /** @brief number number of handled objects */
    qint32         number;

    VisToolTriangle *line;
};

#endif // DIALOGTRIANGLE_H
