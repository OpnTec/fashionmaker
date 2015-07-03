/************************************************************************
 **
 **  @file   vvector3dproperty.h
 **  @author hedgeware <internal(at)hedgeware.net>
 **  @date
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

#ifndef VVECTOR3DPROPERTY_H
#define VVECTOR3DPROPERTY_H

#include "../../vpropertyexplorer_global.h"
#include "../../vproperty.h"

namespace VPE
{

struct VPROPERTYEXPLORERSHARED_EXPORT Vector3D
{
public:
    Vector3D()
        :X(0), Y(0), Z(0)
    {}

    Vector3D(const Vector3D& other)
        :X(other.X), Y(other.Y), Z(other.Z)
    {}

    ~Vector3D() {}

    float X, Y, Z;
};
/*
}

Q_DECLARE_METATYPE(QPE::Vector3D)   // todo

*/

class VPROPERTYEXPLORERSHARED_EXPORT QVector3DProperty : public VProperty
{
    Q_OBJECT
public:
    QVector3DProperty(const QString& name);

    virtual ~QVector3DProperty() Q_DECL_OVERRIDE {}

    //! Get the data how it should be displayed
    virtual QVariant data (int column = DPC_Name, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    //! Returns item flags
    Qt::ItemFlags flags(int column = DPC_Name) const;

    //! Returns the Vector3d
    virtual Vector3D getVector() const;

    //! Sets the Vector3d
    virtual void setVector(const Vector3D& vect);

    //! Sets the Vector3d
    virtual void setVector(float x, float y, float z);

    //! Returns a string containing the type of the property
    virtual QString type() const Q_DECL_OVERRIDE;

    //! Clones this property
    //! \param include_children Indicates whether to also clone the children
    //! \param container If a property is being passed here, no new VProperty is being created but instead it is tried
    //! to fill all the data into container. This can also be used when subclassing this function.
    //! \return Returns the newly created property (or container, if it was not NULL)
    virtual VProperty* clone(bool include_children = true, VProperty* container = NULL) const Q_DECL_OVERRIDE;

    //! Sets the value of the property
    virtual void setValue(const QVariant& value) Q_DECL_OVERRIDE;

    //! Returns the value of the property as a QVariant
    virtual QVariant getValue() const Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QVector3DProperty)
};

}

#endif // VVECTOR3DPROPERTY_H
