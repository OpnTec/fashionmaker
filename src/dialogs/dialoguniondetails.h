/************************************************************************
 **
 **  @file   dialoguniondetails.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   23 12, 2013
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

#ifndef DIALOGUNIONDETAILS_H
#define DIALOGUNIONDETAILS_H

#include "dialogtool.h"

namespace Ui
{
    class DialogUnionDetails;
}

/**
 * @brief The DialogUnionDetails class dialog for ToolUnionDetails.
 */
class DialogUnionDetails : public DialogTool
{
    Q_OBJECT
public:
    /**
     * @brief DialogUnionDetails create dialog
     * @param data container with data
     * @param parent parent widget
     */
    explicit DialogUnionDetails(const VContainer *data, QWidget *parent = 0);
             ~DialogUnionDetails();
    /**
     * @brief getD1 return id first detail
     * @return id
     */
    inline qint64    getD1() const {return d1;}
    /**
     * @brief getD2 return id second detail
     * @return id
     */
    inline qint64    getD2() const {return d2;}
    /**
     * @brief getIndexD1 return index edge first detail
     * @return index
     */
    inline ptrdiff_t getIndexD1() const {return indexD1;}
    /**
     * @brief getIndexD2 return index edge second detail
     * @return index
     */
    inline ptrdiff_t getIndexD2() const {return indexD2;}
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save correct data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    void             ChoosedObject(qint64 id, const Scene::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void     DialogAccepted();
private:
    Q_DISABLE_COPY(DialogUnionDetails)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogUnionDetails *ui;
    /**
     * @brief indexD1 index edge first detail
     */
    ptrdiff_t        indexD1;
    /**
     * @brief indexD2 index edge second detail
     */
    ptrdiff_t        indexD2;
    /**
     * @brief d1 id first detail
     */
    qint64           d1;
    /**
     * @brief d2 id second detail
     */
    qint64           d2;
    /**
     * @brief numberD number of detail, what we already have
     */
    qint32           numberD;
    /**
     * @brief numberP number of points, what we already have
     */
    qint32           numberP;
    /**
     * @brief p1 id first point of detail
     */
    qint64           p1;
    /**
     * @brief p2 id second point of detail
     */
    qint64           p2;
    /**
     * @brief CheckObject check if detail contain this id
     * @param id id of item
     * @param idDetail detail id
     * @return true if contain
     */
    bool             CheckObject(const qint64 &id, const qint64 &idDetail) const;
    /**
     * @brief ChoosedDetail help save information about detail and points on detail
     * @param id id selected object
     * @param type type selected object
     * @param idDetail id detail
     * @param index index of edge
     */
    void             ChoosedDetail(const qint64 &id, const Scene::Scenes &type, qint64 &idDetail, ptrdiff_t &index);
};

#endif // DIALOGUNIONDETAILS_H
