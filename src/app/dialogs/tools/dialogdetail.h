/************************************************************************
 **
 **  @file   dialogdetail.h
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

#ifndef DIALOGDETAIL_H
#define DIALOGDETAIL_H

#include "ui_dialogdetail.h"
#include "dialogtool.h"
#include "../../geometry/vdetail.h"

/**
 * @brief The DialogDetail class dialog for ToolDetai. Help create detail and edit option.
 */
class DialogDetail : public DialogTool
{
    Q_OBJECT
public:
    DialogDetail(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);

    VDetail          getDetail() const;
    void             setDetail(const VDetail &value);
public slots:
    virtual void     ChosenObject(quint32 id, const SceneObject &type);
    void             BiasXChanged(qreal d);
    void             BiasYChanged(qreal d);
    void             ClickedSeams(bool checked);
    void             ClickedClosed(bool checked);
    void             ClickedReverse(bool checked);
    void             ObjectChanged(int row);
    void             DeleteItem();
    virtual void     UpdateList();
protected:
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void     SaveData();
private:

    /** @brief ui keeps information about user interface */
    Ui::DialogDetail ui;

    /** @brief detail detail */
    VDetail          detail;

    /** @brief supplement keep option supplement of seams */
    bool             supplement;

    /** @brief closed keep option about equdistant (closed or not) */
    bool             closed;

    void             NewItem(quint32 id, const Tool &typeTool, const NodeDetail &typeNode,
                             qreal mx = 0, qreal my = 0, bool reverse = false);
    VDetail          CreateDetail() const;
    void             ValidObjects(bool value);
    void             EnableObjectGUI(bool value);
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getDetails return detail
 * @return detail
 */
inline VDetail DialogDetail::getDetail() const
{
    return detail;
}

#endif // DIALOGDETAIL_H
