/************************************************************************
 **
 **  @file   vpropertytreeview.h
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

#ifndef VPROPERTYTREEVIEW_H
#define VPROPERTYTREEVIEW_H

#include <QTreeView>
#include "vpropertyexplorer_global.h"

namespace VPE
{

class VPropertyTreeViewPrivate;

class VPROPERTYEXPLORERSHARED_EXPORT VPropertyDelegate;
class VPROPERTYEXPLORERSHARED_EXPORT VPropertyModel;

class VPROPERTYEXPLORERSHARED_EXPORT VPropertyTreeView : public QTreeView
{
    Q_OBJECT
public:
    //! Default constructor
    explicit VPropertyTreeView(QWidget *parent = 0);

    //! The destructor, taking a model and setting it to the tree view
    //! \param The model to set as model for this tree view
    VPropertyTreeView(VPropertyModel* model, QWidget *parent = 0);

    //! Destructor
    virtual ~VPropertyTreeView();

    //! Sets the height for each row. Set this to 0 in order to let the standard delegate decide
    void setRowHeight(int height = 0, bool add_to_standard = false);


protected:
    //! This method is called by the constructors to initialize the view
    virtual void init();

    //! protected constructor
    explicit VPropertyTreeView(VPropertyTreeViewPrivate* d, bool init_, QWidget *parent = 0);

    //! The protected data
    VPropertyTreeViewPrivate* d_ptr;

private:
    Q_DISABLE_COPY(VPropertyTreeView)
};

}

#endif // VPROPERTYTREEVIEWEEVIEW_H
