#ifndef DIALOGLINEINTERSECT_H
#define DIALOGLINEINTERSECT_H

#include "dialogtool.h"

namespace Ui {
class DialogLineIntersect;
}

class DialogLineIntersect : public DialogTool
{
    Q_OBJECT
public:
    explicit                DialogLineIntersect(const VContainer *data, QWidget *parent = 0);
                            ~DialogLineIntersect();
    qint64                  getP1Line1() const;
    void                    setP1Line1(const qint64 &value);
    qint64                  getP2Line1() const;
    void                    setP2Line1(const qint64 &value);
    qint64                  getP1Line2() const;
    void                    setP1Line2(const qint64 &value);
    qint64                  getP2Line2() const;
    void                    setP2Line2(const qint64 &value);
    QString                 getPointName() const;
    void                    setPointName(const QString &value);
public slots:
    virtual void            ChoosedObject(qint64 id, Scene::Type type);
    virtual void            DialogAccepted();
    void                    P1Line1Changed( int index);
    void                    P2Line1Changed( int index);
    void                    P1Line2Changed( int index);
    void                    P2Line2Changed( int index);
private:
    Ui::DialogLineIntersect *ui;
    qint32                  number;
    QString                 pointName;
    qint64                  p1Line1;
    qint64                  p2Line1;
    qint64                  p1Line2;
    qint64                  p2Line2;
    bool                    flagPoint;
    virtual void            CheckState();
    bool                    CheckIntersecion();
    DialogLineIntersect(const DialogLineIntersect &dialog);
    const DialogLineIntersect &operator=(const DialogLineIntersect& dialog);
};

#endif // DIALOGLINEINTERSECT_H
