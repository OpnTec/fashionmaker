#ifndef DIALOGALONGLINE_H
#define DIALOGALONGLINE_H

#include "dialogtool.h"
#include "../options.h"
#include "../container/vcontainer.h"
#include "../container/calculator.h"

namespace Ui {
class DialogAlongLine;
}

class DialogAlongLine : public DialogTool
{
    Q_OBJECT    
public:
    explicit            DialogAlongLine(const VContainer *data, QWidget *parent = 0);
                        ~DialogAlongLine();
    QString             getPointName() const;
    void                setPointName(const QString &value);
    QString             getTypeLine() const;
    void                setTypeLine(const QString &value);
    QString             getFormula() const;
    void                setFormula(const QString &value);
    qint64              getFirstPointId() const;
    void                setFirstPointId(const qint64 &value, const qint64 &id);
    qint64              getSecondPointId() const;
    void                setSecondPointId(const qint64 &value, const qint64 &id);
public slots:
    virtual void        ChoosedObject(qint64 id, Scene::Type type);
    virtual void        DialogAccepted();
private:
    Ui::DialogAlongLine *ui;
    qint32              number;
    QString             pointName;
    QString             typeLine;
    QString             formula;
    qint64              firstPointId;
    qint64              secondPointId;
    DialogAlongLine(const DialogAlongLine &dialog);
    const DialogAlongLine &operator=(const DialogAlongLine& dialog);
};

#endif // DIALOGALONGLINE_H
