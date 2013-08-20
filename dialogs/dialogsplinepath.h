#ifndef DIALOGSPLINEPATH_H
#define DIALOGSPLINEPATH_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "dialogtool.h"
#pragma GCC diagnostic pop
#include "../container/vcontainer.h"
#include "../geometry/vsplinepath.h"

namespace Ui {
class DialogSplinePath;
}

class DialogSplinePath : public DialogTool
{
    Q_OBJECT
public:
    explicit DialogSplinePath(const VContainer *data, QWidget *parent = 0);
    ~DialogSplinePath();
    VSplinePath      GetPath() const;
    void             SetPath(const VSplinePath &value);
public slots:
    virtual void     ChoosedObject(qint64 id, Scene::Type type);
    virtual void     DialogAccepted();
    void             PointChenged(int row);
    void             currentPointChanged( int index );
    void             Angle1Changed( int index );
    void             Angle2Changed( int index );
    void             KAsm1Changed(qreal d);
    void             KAsm2Changed(qreal d);
private:
    Ui::DialogSplinePath *ui;
    VSplinePath      path;
    void             NewItem(qint64 id, qreal kAsm1, qreal angle, qreal kAsm2);
    void             DataPoint(qint64 id, qreal kAsm1, qreal angle1, qreal kAsm2, qreal angle2);
    void             EnableFields();
    void             SetAngle(qint32 angle);
    DialogSplinePath(const DialogSplinePath &dialog);
    const DialogSplinePath &operator=(const DialogSplinePath& dialog);
};

#endif // DIALOGSPLINEPATH_H
