#include "vpropertytreeview.h"

#include "vpropertydelegate.h"
#include "vpropertymodel.h"

#include "vpropertytreeview_p.h"

using namespace VPE;

VPropertyTreeView::VPropertyTreeView(QWidget *parent)
    : QTreeView(parent), d_ptr(new VPropertyTreeViewPrivate())
{
    init();
}

VPropertyTreeView::VPropertyTreeView(VPropertyModel *model, QWidget *parent)
    : QTreeView(parent), d_ptr(new VPropertyTreeViewPrivate())
{
    init();

    if(model)
        setModel(model);
}

VPropertyTreeView::VPropertyTreeView(VPropertyTreeViewPrivate *d, bool init_, QWidget *parent)
    : QTreeView(parent), d_ptr(d)
{
    if(init_)
        init();
}

VPropertyTreeView::~VPropertyTreeView()
{
    delete d_ptr;
}


void VPropertyTreeView::setRowHeight(int height, bool add_to_standard)
{
    d_ptr->PropertyDelegate->setRowHeight(height, add_to_standard);
}

void VPropertyTreeView::init()
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


