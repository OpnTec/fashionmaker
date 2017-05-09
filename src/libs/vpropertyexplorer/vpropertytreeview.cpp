/************************************************************************
 **
 **  @file   vpropertytreeview.cpp
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

#include "vpropertytreeview.h"

#include <QAbstractItemView>
#include <QFlags>

#include "vpropertydelegate.h"
#include "vpropertymodel.h"
#include "vpropertytreeview_p.h"

VPE::VPropertyTreeView::VPropertyTreeView(QWidget *parent)
    : QTreeView(parent), d_ptr(new VPropertyTreeViewPrivate())
{
    init();
}

VPE::VPropertyTreeView::VPropertyTreeView(VPropertyModel *model, QWidget *parent)
    : QTreeView(parent), d_ptr(new VPropertyTreeViewPrivate())
{
    init();

    if (model)
    {
        setModel(model);
    }
}

VPE::VPropertyTreeView::VPropertyTreeView(VPropertyTreeViewPrivate *d, bool init_, QWidget *parent)
    : QTreeView(parent), d_ptr(d)
{
    if (init_)
    {
        init();
    }
}

VPE::VPropertyTreeView::~VPropertyTreeView()
{
    delete d_ptr;
}


void VPE::VPropertyTreeView::setRowHeight(int height, bool add_to_standard)
{
    d_ptr->PropertyDelegate->setRowHeight(height, add_to_standard);
}

void VPE::VPropertyTreeView::init()
{
    setAlternatingRowColors(true);
    setUniformRowHeights(true);
    d_ptr->PropertyDelegate = new VPropertyDelegate(this);
    setItemDelegate(d_ptr->PropertyDelegate);

    setSelectionMode(QTreeView::SingleSelection);
    setSelectionBehavior(QTreeView::SelectRows);
    setRootIsDecorated(true);

    setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::SelectedClicked);
}
