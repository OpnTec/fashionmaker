/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
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
 ****************************************************************************/

#ifndef VDETAIL_H
#define VDETAIL_H

#include "vnodedetail.h"

namespace Detail {
    enum Contour { OpenContour, CloseContour };
    Q_DECLARE_FLAGS(Contours, Contour)

    enum Equidistant { OpenEquidistant, CloseEquidistant };
    Q_DECLARE_FLAGS(Equidistants, Equidistant)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(Detail::Contours)
Q_DECLARE_OPERATORS_FOR_FLAGS(Detail::Equidistants)

class VDetail{
public:
                   VDetail();
                   VDetail(const QString &name, const QVector<VNodeDetail> &nodes);
                   VDetail(const VDetail &detail);
                   VDetail &operator=(const VDetail &detail);
    inline void    append(const VNodeDetail &node) {nodes.append(node);}
    void           Clear();
    inline qint32  CountNode() const {return nodes.size();}
    bool           Containes(const qint64 &id)const;
    VNodeDetail &  operator[](int indx);
    inline QString getName() const {return name;}
    inline void    setName(const QString &value) {name = value;}
    inline qreal   getMx() const {return mx;}
    inline void    setMx(const qreal &value) {mx = value;}
    inline qreal   getMy() const {return my;}
    inline void    setMy(const qreal &value) {my = value;}
    inline bool    getSupplement() const {return supplement;}
    inline void    setSupplement(bool value) {supplement = value;}
    inline bool    getClosed() const {return closed;}
    inline void    setClosed(bool value) {closed = value;}
    inline qreal   getWidth() const {return width;}
    inline void    setWidth(const qreal &value) {width = value;}
    inline QVector<VNodeDetail> getNodes() const {return nodes;}
    inline void    setNodes(const QVector<VNodeDetail> &value) {nodes = value;}
private:
    QVector<VNodeDetail> nodes;
    QString        name;
    qreal          mx;
    qreal          my;
    bool           supplement;
    bool           closed;
    qreal          width;
};

#endif // VDETAIL_H
