#ifndef DIALOGENDLINE_H
#define DIALOGENDLINE_H

#include "dialogtool.h"
#include <QPushButton>
#include <QListWidgetItem>
#include <QTimer>

#include "../options.h"
#include "../container/vcontainer.h"
#include "../container/calculator.h"

namespace Ui {
class DialogEndLine;
}

class DialogEndLine : public DialogTool
{
    Q_OBJECT 
public:
    explicit          DialogEndLine(const VContainer *data, QWidget *parent = 0);
                      ~DialogEndLine();
    QString           getPointName() const;
    void              setPointName(const QString &value);
    QString           getTypeLine() const;
    void              setTypeLine(const QString &value);
    QString           getFormula() const;
    void              setFormula(const QString &value);
    qint32            getAngle() const;
    void              setAngle(const qint32 &value);
    qint64            getBasePointId() const;
    void              setBasePointId(const qint64 &value);
public slots:
    virtual void      ChoosedObject(qint64 id, Scene::Type type);
    virtual void      DialogAccepted();
private:
    Ui::DialogEndLine *ui;
    QString           pointName;
    QString           typeLine;
    QString           formula;
    qint32            angle;
    qint64            basePointId;
};

#endif // DIALOGENDLINE_H
