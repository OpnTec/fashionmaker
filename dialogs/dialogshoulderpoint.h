#ifndef DIALOGSHOULDERPOINT_H
#define DIALOGSHOULDERPOINT_H

#include "dialogtool.h"
#include "../options.h"
#include "../container/vcontainer.h"
#include "../container/calculator.h"

namespace Ui {
class DialogShoulderPoint;
}

class DialogShoulderPoint : public DialogTool
{
    Q_OBJECT
public:
    explicit                DialogShoulderPoint(const VContainer *data, QWidget *parent = 0);
                            ~DialogShoulderPoint();
    QString                 getPointName() const;
    void                    setPointName(const QString &value);
    QString                 getTypeLine() const;
    void                    setTypeLine(const QString &value);
    QString                 getFormula() const;
    void                    setFormula(const QString &value);
    qint64                  getP1Line() const;
    void                    setP1Line(const qint64 &value);
    qint64                  getP2Line() const;
    void                    setP2Line(const qint64 &value);
    qint64                  getPShoulder() const;
    void                    setPShoulder(const qint64 &value);
public slots:
    virtual void            ChoosedPoint(qint64 id, Scene::Type type);
    virtual void            DialogAccepted();
private:
    Ui::DialogShoulderPoint *ui;
    qint32                  number;
    QString                 pointName;
    QString                 typeLine;
    QString                 formula;
    qint64                  p1Line;
    qint64                  p2Line;
    qint64                  pShoulder;
};

#endif // DIALOGSHOULDERPOINT_H
