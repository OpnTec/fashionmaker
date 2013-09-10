#ifndef DIALOGLINE_H
#define DIALOGLINE_H

#include "dialogtool.h"
#include <QComboBox>
#include "container/vcontainer.h"
#include "options.h"


namespace Ui {
class DialogLine;
}

class DialogLine : public DialogTool
{
    Q_OBJECT
public:
    explicit         DialogLine(const VContainer *data, Draw::Mode mode = Draw::Calculation,
                                QWidget *parent = 0);
                     ~DialogLine();   
    qint64           getFirstPoint() const;
    void             setFirstPoint(const qint64 &value);
    qint64           getSecondPoint() const;
    void             setSecondPoint(const qint64 &value);
public slots:
    void             ChoosedObject(qint64 id, Scene::Type type);
    virtual void     DialogAccepted();
private:
    Ui::DialogLine   *ui;
    qint32           number;
    qint64           firstPoint;
    qint64           secondPoint;
    DialogLine(const DialogLine &dialog);
    const DialogLine &operator=(const DialogLine& dialog);
};

#endif // DIALOGLINE_H
