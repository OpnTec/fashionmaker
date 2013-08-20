#ifndef DIALOGSINGLEPOINT_H
#define DIALOGSINGLEPOINT_H

#pragma GCC diagnostic ignored "-Weffc++"
#include "dialogtool.h"
#pragma GCC diagnostic warning "-Weffc++"
#include "../container/vcontainer.h"

namespace Ui {
class DialogSinglePoint;
}

class DialogSinglePoint : public DialogTool
{
    Q_OBJECT 
public:
    explicit              DialogSinglePoint(const VContainer *data, QWidget *parent = 0);
    void                  setData(const QString name, const QPointF point);
    QString               getName()const;
    QPointF               getPoint()const;
                          ~DialogSinglePoint();
public slots:
    void                  mousePress(QPointF scenePos);
    virtual void          DialogAccepted();
private:
    Ui::DialogSinglePoint *ui;
    QString               name;
    QPointF               point;
    DialogSinglePoint(const DialogSinglePoint &dialog);
    const DialogSinglePoint &operator=(const DialogSinglePoint& dialog);
};

#endif // DIALOGSINGLEPOINT_H
