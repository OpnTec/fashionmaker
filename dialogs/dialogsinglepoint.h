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
    explicit              DialogSinglePoint(QWidget *parent = 0);
    void                  setData(const QString name, const QPointF point);
    QString               getName()const;
    QPointF               getPoint()const;
                          ~DialogSinglePoint();
signals:
    void                  ToolCanseled();
    void                  SinglePointCreated(const QString name, const QPointF point);
public slots:
    void                  mousePress(QPointF scenePos);
    void                  NameChanged();
    void                  CanselOperation();
    void                  OkOperation();
protected:
    void                  showEvent( QShowEvent *event );
    void                  closeEvent ( QCloseEvent * event );
private:
    Ui::DialogSinglePoint *ui;
    bool                  isInitialized;
    QString               name;
    QPointF               point;
};

#endif // DIALOGSINGLEPOINT_H
