#ifndef DIALOGNORMAL_H
#define DIALOGNORMAL_H

#include "dialogtool.h"
#include <QPushButton>
#include <QListWidgetItem>
#include <QTimer>

#include "../options.h"
#include "../container/vcontainer.h"
#include "../container/calculator.h"

namespace Ui {
class DialogNormal;
}

class DialogNormal : public DialogTool
{
    Q_OBJECT
public:
    explicit         DialogNormal(const VContainer *data, QWidget *parent = 0);
                     ~DialogNormal();
    QString          getPointName() const;
    void             setPointName(const QString &value);
    QString          getTypeLine() const;
    void             setTypeLine(const QString &value);
    QString          getFormula() const;
    void             setFormula(const QString &value);
    qint32           getAngle() const;
    void             setAngle(const qint32 &value);
    qint64           getFirstPointId() const;
    void             setFirstPointId(const qint64 &value, const qint64 &id);
    qint64           getSecondPointId() const;
    void             setSecondPointId(const qint64 &value, const qint64 &id);
public slots:
    virtual void     ChoosedObject(qint64 id, Scene::Type type);
    virtual void     DialogAccepted();
private:
    Ui::DialogNormal *ui;
    qint32           number;
    QString          pointName;
    QString          typeLine;
    QString          formula;
    qint32           angle;
    qint64           firstPointId;
    qint64           secondPointId;
    DialogNormal(const DialogNormal &dialog);
    const DialogNormal &operator=(const DialogNormal& dialog);
};

#endif // DIALOGNORMAL_H
