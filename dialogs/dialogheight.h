#ifndef DIALOGHEIGHT_H
#define DIALOGHEIGHT_H

#include "dialogtool.h"

namespace Ui {
class DialogHeight;
}

class DialogHeight : public DialogTool{
    Q_OBJECT
public:
    explicit         DialogHeight(const VContainer *data, Draw::Draws mode = Draw::Calculation,
                                  QWidget *parent = 0);
                     ~DialogHeight();
    QString          getPointName() const;
    void             setPointName(const QString &value);
    QString          getTypeLine() const;
    void             setTypeLine(const QString &value);
    qint64           getBasePointId() const;
    void             setBasePointId(const qint64 &value, const qint64 &id);
    qint64           getP1LineId() const;
    void             setP1LineId(const qint64 &value, const qint64 &id);
    qint64           getP2LineId() const;
    void             setP2LineId(const qint64 &value, const qint64 &id);
public slots:
    virtual void     ChoosedObject(qint64 id, Scene::Scenes type);
    virtual void     DialogAccepted();
private:
    Q_DISABLE_COPY(DialogHeight)
    Ui::DialogHeight *ui;
    qint32           number;
    QString          pointName;
    QString          typeLine;
    qint64           basePointId;
    qint64           p1LineId;
    qint64           p2LineId;
};

#endif // DIALOGHEIGHT_H
