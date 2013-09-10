#ifndef DIALOGDETAIL_H
#define DIALOGDETAIL_H

#include "ui_dialogdetail.h"
#include "dialogtool.h"
#include "container/vcontainer.h"
#include "geometry/vdetail.h"

class DialogDetail : public DialogTool
{
    Q_OBJECT  
public:
    explicit DialogDetail(const VContainer *data, Draw::Mode mode, QWidget *parent = 0);
    VDetail getDetails() const;
    void setDetails(const VDetail &value);

public slots:
    virtual void      ChoosedObject(qint64 id, Scene::Type type);
    virtual void      DialogAccepted();
private:
    Ui::DialogDetail ui;
    VDetail details;
    void             NewItem(qint64 id, Tools::Enum typeTool, Draw::Mode mode, NodeDetail::Type typeNode);
};

#endif // DIALOGDETAIL_H
