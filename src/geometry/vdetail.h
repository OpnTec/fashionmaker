/************************************************************************
 **
 **  @file   vdetail.h
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

#ifndef VDETAIL_H
#define VDETAIL_H

#include "vnodedetail.h"

#include <QString>
#include <QVector>

namespace Detail
{
    /**
     * @brief The Contour enum
     */
    enum Contour { OpenContour, CloseContour };
    Q_DECLARE_FLAGS(Contours, Contour)

    /**
     * @brief The Equidistant enum
     */
    enum Equidistant { OpenEquidistant, CloseEquidistant };
    Q_DECLARE_FLAGS(Equidistants, Equidistant)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(Detail::Contours)
Q_DECLARE_OPERATORS_FOR_FLAGS(Detail::Equidistants)

/**
 * @brief The VDetail class
 */
class VDetail
{
public:
                   /**
                    * @brief VDetail
                    */
                   VDetail();
                   /**
                    * @brief VDetail
                    * @param name
                    * @param nodes
                    */
                   VDetail(const QString &name, const QVector<VNodeDetail> &nodes);
                   /**
                    * @brief VDetail
                    * @param detail
                    */
                   VDetail(const VDetail &detail);
                   /**
                    * @brief operator =
                    * @param detail
                    * @return
                    */
                   VDetail &operator=(const VDetail &detail);
    /**
     * @brief append
     * @param node
     */
    inline void    append(const VNodeDetail &node) {nodes.append(node);}
    /**
     * @brief Clear
     */
    void           Clear();
    /**
     * @brief CountNode
     * @return
     */
    inline qint32  CountNode() const {return nodes.size();}
    /**
     * @brief Containes
     * @param id
     * @return
     */
    bool           Containes(const qint64 &id)const;
    /**
     * @brief operator []
     * @param indx
     * @return
     */
    VNodeDetail &  operator[](ptrdiff_t indx);
    const VNodeDetail & at ( ptrdiff_t indx ) const;
    /**
     * @brief getName
     * @return
     */
    inline QString getName() const {return name;}
    /**
     * @brief setName
     * @param value
     */
    inline void    setName(const QString &value) {name = value;}
    /**
     * @brief getMx
     * @return
     */
    inline qreal   getMx() const {return mx;}
    /**
     * @brief setMx
     * @param value
     */
    inline void    setMx(const qreal &value) {mx = value;}
    /**
     * @brief getMy
     * @return
     */
    inline qreal   getMy() const {return my;}
    /**
     * @brief setMy
     * @param value
     */
    inline void    setMy(const qreal &value) {my = value;}
    /**
     * @brief getSupplement
     * @return
     */
    inline bool    getSupplement() const {return supplement;}
    /**
     * @brief setSupplement
     * @param value
     */
    inline void    setSupplement(bool value) {supplement = value;}
    /**
     * @brief getClosed
     * @return
     */
    inline bool    getClosed() const {return closed;}
    /**
     * @brief setClosed
     * @param value
     */
    inline void    setClosed(bool value) {closed = value;}
    /**
     * @brief getWidth
     * @return
     */
    inline qreal   getWidth() const {return width;}
    /**
     * @brief setWidth
     * @param value
     */
    inline void    setWidth(const qreal &value) {width = value;}
    /**
     * @brief getNodes
     * @return
     */
    inline QVector<VNodeDetail> getNodes() const {return nodes;}
    /**
     * @brief setNodes
     * @param value
     */
    inline void    setNodes(const QVector<VNodeDetail> &value) {nodes = value;}
    ptrdiff_t      indexOfNode(const qint64 &id) const;
    qint64 id() const;
    void setId(const qint64 &id);

private:
    qint64         _id;
    /**
     * @brief nodes
     */
    QVector<VNodeDetail> nodes;
    /**
     * @brief name
     */
    QString        name;
    /**
     * @brief mx
     */
    qreal          mx;
    /**
     * @brief my
     */
    qreal          my;
    /**
     * @brief supplement
     */
    bool           supplement;
    /**
     * @brief closed
     */
    bool           closed;
    /**
     * @brief width
     */
    qreal          width;
};

#endif // VDETAIL_H
