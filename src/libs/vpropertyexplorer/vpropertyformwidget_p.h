#ifndef VPROPERTYFORMWIDGET_P_H
#define VPROPERTYFORMWIDGET_P_H

// ONLY INCLUDE THIS IN .CPP FILES

#include <QList>
#include "vproperty.h"

namespace VPE {

class VPropertyFormWidgetPrivate {
public:
    //! Stores either another VPropertyFormWidget (then Editor is null) or an editor widget (then FormWidget is null)
    struct SEditorWidget {
        SEditorWidget() : FormWidget(nullptr), Editor(nullptr) {}
        SEditorWidget(VPropertyFormWidget* form_widget) : FormWidget(form_widget), Editor(nullptr) {}
        SEditorWidget(QWidget* editor_widget) : FormWidget(nullptr), Editor(editor_widget) {}

        VPropertyFormWidget* FormWidget;
        QWidget* Editor;
    };

    //! The root property to use
    QList<VProperty*> Properties;

    //! Binds the properties to their editors
    QList<SEditorWidget> EditorWidgets;

    //! Determines the behaviour of the editors. If this is true, when a focus out event etc. happens, the data will be submitted to the VProperty. If false, you will have to call commitData() yourself.
    bool UpdateEditors;

    //! Default constructor
    VPropertyFormWidgetPrivate()
        : UpdateEditors(true)
    {}

    //! Constructor
    VPropertyFormWidgetPrivate(const QList<VProperty*>& properties)
        : Properties(properties), UpdateEditors(true) {}
};

}

#endif // VPROPERTYFORMWIDGET_P_H
