/************************************************************************
 **
 **  @file   vpointfproperty.h
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

#ifndef VPOINTFPROPERTY_H
#define VPOINTFPROPERTY_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVariant>
#include <Qt>
#include <QtGlobal>

#include "../vproperty.h"
#include "../vpropertyexplorer_global.h"
#include "vproperty.h"
#include "vpropertyexplorer_global.h"

class QPointF;

namespace VPE
{

class VPROPERTYEXPLORERSHARED_EXPORT VPointFProperty : public VProperty
{
    Q_OBJECT
public:
    explicit VPointFProperty(const QString& name);

    virtual ~VPointFProperty() Q_DECL_OVERRIDE {}

    //! Get the data how it should be displayed
    virtual QVariant data (int column = DPC_Name, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    //! Returns item flags
    Qt::ItemFlags flags(int column = DPC_Name) const Q_DECL_OVERRIDE;

    //! Returns the QPointF
    virtual QPointF getPointF() const;

    //! Sets the QPointF
    virtual void setPointF(const QPointF& point);

    //! Sets the QPointF
    virtual void setPointF(qreal x, qreal y);

    //! Returns a string containing the type of the property
    virtual QString type() const Q_DECL_OVERRIDE;

    //! Clones this property
    //! \param include_children Indicates whether to also clone the children
    //! \param container If a property is being passed here, no new VProperty is being created but instead it is tried
    //! to fill all the data into container. This can also be used when subclassing this function.
    //! \return Returns the newly created property (or container, if it was not NULL)
    virtual VProperty* clone(bool include_children = true,
                             VProperty* container = nullptr) const Q_DECL_OVERRIDE Q_REQUIRED_RESULT;

    //! Sets the value of the property
    virtual void setValue(const QVariant& value) Q_DECL_OVERRIDE;

    //! Returns the value of the property as a QVariant
    virtual QVariant getValue() const Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VPointFProperty)
};

}

#endif // VPOINTFPROPERTY_H
