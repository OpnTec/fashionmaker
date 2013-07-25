#ifndef DIALOGLINE_H
#define DIALOGLINE_H

#include <QDialog>
#include "../container/vcontainer.h"
#include "../options.h"
#include <QComboBox>

namespace Ui {
class DialogLine;
}

class DialogLine : public QDialog
{
    Q_OBJECT
    
public:
    explicit         DialogLine(const VContainer *data, QWidget *parent = 0);
                     ~DialogLine();
    
    qint64           getFirstPoint() const;
    void             setFirstPoint(const qint64 &value);

    qint64           getSecondPoint() const;
    void             setSecondPoint(const qint64 &value);
signals:
    void             DialogClosed(int result);
public slots:
    void             ChoosedPoint(qint64 id, Scene::Type type);
    void             DialogAccepted();
protected:
    void             closeEvent ( QCloseEvent * event );
    void             showEvent( QShowEvent *event );
private:
    Ui::DialogLine   *ui;
    const VContainer *data;
    qint32           number;
    qint64           firstPoint;
    qint64           secondPoint;
    bool             isInitialized;
    void             FillComboBox(QComboBox *box);
};

#endif // DIALOGLINE_H
