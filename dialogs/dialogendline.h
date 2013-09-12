#ifndef DIALOGENDLINE_H
#define DIALOGENDLINE_H

#include "dialogtool.h"
#include <QPushButton>
#include <QListWidgetItem>
#include <QTimer>
#include "options.h"
#include "container/vcontainer.h"
#include "container/calculator.h"

namespace Ui {
class DialogEndLine;
}

class DialogEndLine : public DialogTool
{
    Q_OBJECT 
public:
    explicit          DialogEndLine(const VContainer *data, Draw::Mode mode = Draw::Calculation,
                                    QWidget *parent = 0);
                      ~DialogEndLine();
    QString           getPointName() const;
    void              setPointName(const QString &value);
    QString           getTypeLine() const;
    void              setTypeLine(const QString &value);
    QString           getFormula() const;
    void              setFormula(const QString &value);
    qreal             getAngle() const;
    void              setAngle(const qreal &value);
    qint64            getBasePointId() const;
    void              setBasePointId(const qint64 &value, const qint64 &id);
public slots:
    virtual void      ChoosedObject(qint64 id, Scene::Type type);
    virtual void      DialogAccepted();
private:
    Ui::DialogEndLine *ui;
    QString           pointName;
    QString           typeLine;
    QString           formula;
    qreal             angle;
    qint64            basePointId;
    DialogEndLine(const DialogEndLine &dialog);
    const DialogEndLine &operator=(const DialogEndLine& dialog);
};

#endif // DIALOGENDLINE_H
