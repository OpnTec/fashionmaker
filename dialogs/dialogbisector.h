#ifndef DIALOGBISECTOR_H
#define DIALOGBISECTOR_H

#include "dialogtool.h"
#include <QPushButton>
#include <QListWidgetItem>
#include <QTimer>

#include "../options.h"
#include "../container/vcontainer.h"
#include "../container/calculator.h"

namespace Ui {
class DialogBisector;
}

class DialogBisector : public DialogTool
{
    Q_OBJECT
    
public:
    explicit DialogBisector(const VContainer *data, QWidget *parent = 0);
    ~DialogBisector();
    QString           getPointName() const;
    void              setPointName(const QString &value);
    QString           getTypeLine() const;
    void              setTypeLine(const QString &value);
    QString           getFormula() const;
    void              setFormula(const QString &value);
    qint64            getFirstPointId() const;
    void              setFirstPointId(const qint64 &value, const qint64 &id);
    qint64            getSecondPointId() const;
    void              setSecondPointId(const qint64 &value, const qint64 &id);
    qint64            getThirdPointId() const;
    void              setThirdPointId(const qint64 &value, const qint64 &id);
public slots:
    virtual void      ChoosedObject(qint64 id, Scene::Type type);
    virtual void      DialogAccepted();
private:
    Ui::DialogBisector *ui;
    qint32             number;
    QString            pointName;
    QString            typeLine;
    QString            formula;
    qint64             firstPointId;
    qint64             secondPointId;
    qint64             thirdPointId;
    DialogBisector(const DialogBisector &dialog);
    const DialogBisector &operator=(const DialogBisector& dialog);
};

#endif // DIALOGBISECTOR_H
