#ifndef VPROPERTYDELEGATE_H
#define VPROPERTYDELEGATE_H

#include "vpropertyexplorer_global.h"

#include <QStyledItemDelegate>

namespace VPE {

class VPROPERTYEXPLORERSHARED_EXPORT VPropertyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit VPropertyDelegate(QObject *parent = 0);
    virtual ~VPropertyDelegate();

    //! Creates the editor widget
    virtual QWidget* createEditor (QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    //! Sets the index data to the editor
    virtual void setEditorData (QWidget * editor, const QModelIndex & index) const;

    //! Updates the index data
    virtual void setModelData (QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;

    //! Returns the size hint
    virtual QSize sizeHint (const QStyleOptionViewItem& option, const QModelIndex& index) const;

    //! Sets the row height. Set this to 0 and the standard will be taken
    void setRowHeight(int height = 0, bool add_to_standard = false);

    //! Renders the delegate using the given painter and style option for the item specified by index.
    virtual void paint (QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

protected:
    int RowHeight;
    bool AddRowHeight;


};

}

#endif // VPROPERTYDELEGATE_H
