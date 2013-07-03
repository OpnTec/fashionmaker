#ifndef DIALOGSINGLEPOINT_H
#define DIALOGSINGLEPOINT_H

#include <QDialog>

namespace Ui {
class DialogSinglePoint;
}

class DialogSinglePoint : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogSinglePoint(QWidget *parent = 0);
    ~DialogSinglePoint();
protected:
    void showEvent( QShowEvent *event );
public slots:
    void mousePress(QPointF scenePos);
    void NameChanged();
private:
    Ui::DialogSinglePoint *ui;
    bool isInitialized;
};

#endif // DIALOGSINGLEPOINT_H
