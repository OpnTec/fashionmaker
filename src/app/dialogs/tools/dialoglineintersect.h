/************************************************************************
 **
 **  @file   dialoglineintersect.h
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

#ifndef DIALOGLINEINTERSECT_H
#define DIALOGLINEINTERSECT_H

#include "dialogtool.h"

namespace Ui
{
    class DialogLineIntersect;
}

class VisToolLineIntersect;

/**
 * @brief The DialogLineIntersect class dialog for ToolLineIntersect. Help create point and edit option.
 */
class DialogLineIntersect : public DialogTool
{
    Q_OBJECT
public:
    DialogLineIntersect(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    ~DialogLineIntersect();

    quint32                 GetP1Line1() const;
    void                    SetP1Line1(const quint32 &value);

    quint32                 GetP2Line1() const;
    void                    SetP2Line1(const quint32 &value);

    quint32                 GetP1Line2() const;
    void                    SetP1Line2(const quint32 &value);

    quint32                 GetP2Line2() const;
    void                    SetP2Line2(const quint32 &value);

    void                    SetPointName(const QString &value);
public slots:
    virtual void            ChosenObject(quint32 id, const SceneObject &type);
    void                    PointChanged();
    virtual void            PointNameChanged();
protected:
    virtual void            ShowVisualization();
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void            SaveData();
private:
    Q_DISABLE_COPY(DialogLineIntersect)

    /** @brief ui keeps information about user interface */
    Ui::DialogLineIntersect *ui;

    /** @brief flagPoint keep state of point */
    bool                    flagPoint;

    VisToolLineIntersect    *line;

    virtual void            CheckState();
    bool                    CheckIntersecion();
};

#endif // DIALOGLINEINTERSECT_H
