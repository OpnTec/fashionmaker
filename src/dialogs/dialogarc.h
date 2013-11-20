/************************************************************************
 **
 **  @file   dialogarc.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
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

#ifndef DIALOGARC_H
#define DIALOGARC_H

#include "dialogtool.h"

namespace Ui
{
    class DialogArc;
}

/**
 * @brief The DialogArc class dialog for ToolArc. Help create arc and edit option.
 */
class DialogArc : public DialogTool
{
    Q_OBJECT
public:
                     /**
                      * @brief DialogArc create dialog
                      * @param data container with data
                      * @param mode mode of creation tool
                      * @param parent parent widget
                      */
                     DialogArc(const VContainer *data, Draw::Draws mode = Draw::Calculation, QWidget *parent = 0);
                     ~DialogArc();
    /**
     * @brief GetCenter return id of center point
     * @return id id
     */
    inline qint64    GetCenter() const {return center;}
    /**
     * @brief SetCenter set id of center point
     * @param value id
     */
    void             SetCenter(const qint64 &value);
    /**
     * @brief GetRadius return formula of radius
     * @return formula
     */
    inline QString   GetRadius() const {return radius;}
    /**
     * @brief SetRadius set formula of radius
     * @param value formula
     */
    void             SetRadius(const QString &value);
    /**
     * @brief GetF1 return formula first angle of arc
     * @return formula
     */
    inline QString   GetF1() const {return f1;}
    /**
     * @brief SetF1 set formula first angle of arc
     * @param value formula
     */
    void             SetF1(const QString &value);
    /**
     * @brief GetF2 return formula second angle of arc
     * @return formula
     */
    inline QString   GetF2() const {return f2;}
    /**
     * @brief SetF2 set formula second angle of arc
     * @param value formula
     */
    void             SetF2(const QString &value);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void     ChoosedObject(qint64 id, const Scene::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void     DialogAccepted();
    /**
     * @brief ValChenged show description angles of lines
     * @param row number of row
     */
    virtual void     ValChenged(int row);
    /**
     * @brief PutRadius put variable into formula of radius
     */
    void             PutRadius();
    /**
     * @brief PutF1 put variable into formula of first angle
     */
    void             PutF1();
    /**
     * @brief PutF2 put variable into formula of second angle
     */
    void             PutF2();
    /**
     * @brief LineAngles show variable angles of lines
     */
    void             LineAngles();
    /**
     * @brief RadiusChanged after change formula of radius calculate value and show result
     */
    void             RadiusChanged();
    /**
     * @brief F1Changed after change formula of first angle calculate value and show result
     */
    void             F1Changed();
    /**
     * @brief F2Changed after change formula of second angle calculate value and show result
     */
    void             F2Changed();
protected:
    /**
     * @brief CheckState if all is right enable button ok
     */
    virtual void     CheckState();
private:
    Q_DISABLE_COPY(DialogArc)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogArc    *ui;
    /**
     * @brief flagRadius true if value of radius is correct
     */
    bool             flagRadius;
    /**
     * @brief flagF1 true if value of first angle is correct
     */
    bool             flagF1;
    /**
     * @brief flagF2 true if value of second angle is correct
     */
    bool             flagF2;
    /**
     * @brief timerRadius timer of check formula of radius
     */
    QTimer           *timerRadius;
    /**
     * @brief timerF1 timer of check formula of first angle
     */
    QTimer           *timerF1;
    /**
     * @brief timerF2 timer of check formula of second angle
     */
    QTimer           *timerF2;
    /**
     * @brief center id of center point
     */
    qint64           center;
    /**
     * @brief radius formula of radius
     */
    QString          radius;
    /**
     * @brief f1 formula of first angle
     */
    QString          f1;
    /**
     * @brief f2 formula of second angle
     */
    QString          f2;
    /**
     * @brief EvalRadius calculate value of radius
     */
    void             EvalRadius();
    /**
     * @brief EvalF1 calculate value of first angle
     */
    void             EvalF1();
    /**
     * @brief EvalF2 calculate value of second angle
     */
    void             EvalF2();
    /**
     * @brief ShowLineAngles show varibles angles of lines
     */
    void             ShowLineAngles();
};

#endif // DIALOGARC_H
