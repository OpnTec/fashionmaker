/************************************************************************
 **
 **  @file   vpointfproperty.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   27 8, 2014
 **
 **  @brief
 **  @copyright
 **  All rights reserved. This program and the accompanying materials
 **  are made available under the terms of the GNU Lesser General Public License
 **  (LGPL) version 2.1 which accompanies this distribution, and is available at
 **  http://www.gnu.org/licenses/lgpl-2.1.html
 **
 **  This library is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 **  Lesser General Public License for more details.
 **
 *************************************************************************/

#include "vpointfproperty.h"

using namespace VPE;

#include "../vproperty_p.h"
#include "vnumberproperty.h"
#include <QPointF>

VPE::VPointFProperty::VPointFProperty(const QString &name)
    : VProperty(name, QVariant::PointF)
{
    d_ptr->VariantValue.setValue(0);
    d_ptr->VariantValue.convert(QVariant::PointF);

    VDoubleProperty* tmpX = new VDoubleProperty("X");
    addChild(tmpX);
    tmpX->setUpdateBehaviour(true, false);

    VDoubleProperty* tmpY = new VDoubleProperty("Y");
    addChild(tmpY);
    tmpY->setUpdateBehaviour(true, false);

    setValue(QPointF());
}

QVariant VPointFProperty::data(int column, int role) const
{
    if (column == DPC_Data && Qt::DisplayRole == role)
    {
        return getPointF();
    }
    else
        return VProperty::data(column, role);
}

Qt::ItemFlags VPointFProperty::flags(int column) const
{
    if (column == DPC_Name || column == DPC_Data)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
    else
        return Qt::NoItemFlags;
}

QPointF VPointFProperty::getPointF() const
{
    QPointF tmpValue;

    if (d_ptr->Children.count() < 2)
    {
        return tmpValue;
    }

    tmpValue.setX(d_ptr->Children.at(0)->getValue().toDouble());
    tmpValue.setY(d_ptr->Children.at(1)->getValue().toDouble());

    return tmpValue;
}

void VPointFProperty::setPointF(const QPointF &point)
{
    setPointF(point.x(), point.y());
}

void VPointFProperty::setPointF(qreal x, qreal y)
{
    if (d_ptr->Children.count() < 2)
    {
        return;
    }

    QVariant tmpX(x);
    tmpX.convert(QVariant::Double);

    QVariant tmpY(y);
    tmpY.convert(QVariant::Double);

    d_ptr->Children.at(0)->setValue(tmpX);
    d_ptr->Children.at(1)->setValue(tmpY);
}

QString VPointFProperty::type() const
{
    return "pointF";
}

VProperty *VPointFProperty::clone(bool include_children, VProperty *container) const
{
    if (!container)
    {
        container = new VPointFProperty(getName());

        if (!include_children)
        {
            QList<VProperty*> tmpChildren = container->getChildren();
            foreach(VProperty* tmpChild, tmpChildren)
            {
                container->removeChild(tmpChild);
                delete tmpChild;
            }
        }
    }

    return VProperty::clone(false, container);  // Child
}

void VPointFProperty::setValue(const QVariant &value)
{
    QPointF tmpPoint = value.toPointF();
    setPointF(tmpPoint);
}

QVariant VPointFProperty::getValue() const
{
    QPointF tmpValue = getPointF();
    return QString("%1,%2").arg(QString::number(tmpValue.x()), QString::number(tmpValue.y()));
}
