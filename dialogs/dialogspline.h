#ifndef DIALOGSPLINE_H
#define DIALOGSPLINE_H

#include "dialogtool.h"
#include "container/vcontainer.h"

namespace Ui {
class DialogSpline;
}

class DialogSpline : public DialogTool
{
    Q_OBJECT  
public:
    explicit         DialogSpline(const VContainer *data, Draw::Mode mode = Draw::Calculation,
                                  QWidget *parent = 0);
                     ~DialogSpline();
    qint64           getP1() const;
    void             setP1(const qint64 &value);
    qint64           getP4() const;
    void             setP4(const qint64 &value);
    qreal            getAngle1() const;
    void             setAngle1(const qreal &value);
    qreal            getAngle2() const;
    void             setAngle2(const qreal &value);
    qreal            getKAsm1() const;
    void             setKAsm1(const qreal &value);
    qreal            getKAsm2() const;
    void             setKAsm2(const qreal &value);
    qreal            getKCurve() const;
    void             setKCurve(const qreal &value);
public slots:
    virtual void     ChoosedObject(qint64 id, Scene::Type type);
    virtual void     DialogAccepted();
private:
    Ui::DialogSpline *ui;
    qint32           number;
    qint64           p1;			// перша точка
    qint64           p4;			// четверта точка
    qreal            angle1;		// кут нахилу дотичної в першій точці
    qreal            angle2;		// кут нахилу дотичної в другій точці
    qreal            kAsm1;
    qreal            kAsm2;
    qreal            kCurve;
    DialogSpline(const DialogSpline &dialog);
    const DialogSpline &operator=(const DialogSpline& dialog);
};

#endif // DIALOGSPLINE_H
