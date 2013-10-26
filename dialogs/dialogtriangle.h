#ifndef DIALOGTRIANGLE_H
#define DIALOGTRIANGLE_H

#include "dialogtool.h"

namespace Ui {
class DialogTriangle;
}

class DialogTriangle : public DialogTool{
    Q_OBJECT
public:
                   DialogTriangle(const VContainer *data, Draw::Draws mode = Draw::Calculation,
                                  QWidget *parent = 0);
                   ~DialogTriangle();
    inline qint64  getAxisP1Id() const {return axisP1Id;}
    void           setAxisP1Id(const qint64 &value, const qint64 &id);
    inline qint64  getAxisP2Id() const {return axisP2Id;}
    void           setAxisP2Id(const qint64 &value, const qint64 &id);
    inline qint64  getFirstPointId() const {return firstPointId;}
    void           setFirstPointId(const qint64 &value, const qint64 &id);
    inline qint64  getSecondPointId() const {return secondPointId;}
    void           setSecondPointId(const qint64 &value, const qint64 &id);
    inline QString getPointName() const {return pointName;}
    void           setPointName(const QString &value);
public slots:
    virtual void   ChoosedObject(qint64 id, Scene::Scenes type);
    virtual void   DialogAccepted();
private:
    Q_DISABLE_COPY(DialogTriangle)
    Ui::DialogTriangle *ui;
    qint32         number;
    QString        pointName;
    qint64         axisP1Id;
    qint64         axisP2Id;
    qint64         firstPointId;
    qint64         secondPointId;
};

#endif // DIALOGTRIANGLE_H
