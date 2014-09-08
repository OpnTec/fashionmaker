#ifndef VCOLORPROPERTYEDITOR_H
#define VCOLORPROPERTYEDITOR_H

#include "vpropertyexplorer_global.h"

#include <QWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>
#include <QSpacerItem>

namespace VPE{

class VPROPERTYEXPLORERSHARED_EXPORT VColorPropertyEditor : public QWidget
{
    Q_OBJECT

public:
    //! Constructor taking a widget as parent
    VColorPropertyEditor(QWidget *parent);

    //! Destructor
    virtual ~VColorPropertyEditor();


    //! Returns the color currently set
    QColor getColor();

    //! A little helper function generating an image to represent a color
    //! \param color The color to fill the image with
    //! \size The size of the generated pixmap
    //! \return Returns a QPixmap
    static QPixmap getColorPixmap(const QColor& color, unsigned int size = 16);

    //! A helper function to convert a color into a string.
    //! \param color The color to fill the image with
    //! \return The color as string, usually in the format [RRR, GGG, BBB] (AAA)
    static QString getColorString(const QColor& color);

    //! Needed for proper event handling
    bool eventFilter(QObject *obj, QEvent *ev);

signals:
    //! This is emitted, when the user changes the color
    void dataChangedByUser(const QColor &getColor, VColorPropertyEditor* editor);

    void dataChanged();

public slots:
    //! Sets the color of the widget
    void setColor(const QColor &color_);

private slots:
    void onToolButtonClicked();

private:
    QColor Color;
    QToolButton* ToolButton;
    QLineEdit* Lineedit;
    QLabel* TextLabel;
    QLabel* ColorLabel;
    QSpacerItem* Spacer;
};

}

#endif // QFILEPROPERTYEDITOR_H
