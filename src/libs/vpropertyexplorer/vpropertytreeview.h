#ifndef VPROPERTYTREEVIEW_H
#define VPROPERTYTREEVIEW_H

#include <QTreeView>
#include "vpropertyexplorer_global.h"

namespace VPE {

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
};

}

#endif // VPROPERTYTREEVIEWEEVIEW_H
