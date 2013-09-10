#ifndef DIALOGARC_H
#define DIALOGARC_H

#include "dialogtool.h"
#include "../container/vcontainer.h"

namespace Ui {
class DialogArc;
}

class DialogArc : public DialogTool
{
    Q_OBJECT
public:
    explicit DialogArc(const VContainer *data, Draw::Mode mode = Draw::Calculation, QWidget *parent = 0);
    ~DialogArc();
    qint64 GetCenter() const;
    void SetCenter(const qint64 &value);

    QString GetRadius() const;
    void SetRadius(const QString &value);

    QString GetF1() const;
    void SetF1(const QString &value);

    QString GetF2() const;
    void SetF2(const QString &value);

public slots:
    virtual void     ChoosedObject(qint64 id, Scene::Type type);
    virtual void     DialogAccepted();
    virtual void     ValChenged(int row);
    void             PutRadius();
    void             PutF1();
    void             PutF2();
    void             LineArcs();
    void             RadiusChanged();
    void             F1Changed();
    void             F2Changed();
protected:
    virtual void     CheckState();
private:
    Ui::DialogArc    *ui;
    bool             flagRadius;
    bool             flagF1;
    bool             flagF2;
    QTimer           *timerRadius;
    QTimer           *timerF1;
    QTimer           *timerF2;
    qint64           center;
    QString          radius;
    QString          f1;
    QString          f2;
    void             EvalRadius();
    void             EvalF1();
    void             EvalF2();
    void             ShowLineArcs();
    DialogArc(const DialogArc &dialog);
    const DialogArc &operator=(const DialogArc& dialog);
};

#endif // DIALOGARC_H
