/************************************************************************
 **
 **  @file   dialogheight.h
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

#ifndef DIALOGHEIGHT_H
#define DIALOGHEIGHT_H

#include "dialogtool.h"

namespace Ui
{
    class DialogHeight;
}

class VisToolHeight;

/**
 * @brief The DialogHeight class dialog for ToolHeight. Help create point and edit option.
 */
class DialogHeight : public DialogTool
{
    Q_OBJECT
public:
    DialogHeight(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    ~DialogHeight();

    void             SetPointName(const QString &value);

    QString          GetTypeLine() const;
    void             SetTypeLine(const QString &value);

    quint32          GetBasePointId() const;
    void             SetBasePointId(const quint32 &value);

    quint32          GetP1LineId() const;
    void             SetP1LineId(const quint32 &value);

    quint32          GetP2LineId() const;
    void             SetP2LineId(const quint32 &value);

    QString          GetLineColor() const;
    void             SetLineColor(const QString &value);
public slots:
    virtual void     ChosenObject(quint32 id, const SceneObject &type);
    virtual void     PointNameChanged();
    virtual void     UpdateList();
protected:
    virtual void     ShowVisualization();
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void     SaveData();
private:
    Q_DISABLE_COPY(DialogHeight)

    /** @brief ui keeps information about user interface */
    Ui::DialogHeight *ui;

    VisToolHeight    *line;
};

#endif // DIALOGHEIGHT_H
