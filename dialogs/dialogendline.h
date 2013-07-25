#ifndef DIALOGENDLINE_H
#define DIALOGENDLINE_H

#include <QDialog>
#include <QPushButton>
#include <QListWidgetItem>
#include <QTimer>

#include "../options.h"
#include "../container/vcontainer.h"
#include "../container/calculator.h"

namespace Ui {
class DialogEndLine;
}

class DialogEndLine : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogEndLine(const VContainer *data, QWidget *parent = 0);
    ~DialogEndLine();
    QString getPointName() const;
    void setPointName(const QString &value);

    QString getTypeLine() const;
    void setTypeLine(const QString &value);

    QString getFormula() const;
    void setFormula(const QString &value);

    qint32 getAngle() const;
    void setAngle(const qint32 &value);

    qint64 getBasePointId() const;
    void setBasePointId(const qint64 &value);
signals:
    void DialogClosed(int result);
public slots:
    void ChoosedPoint(qint64 id, Scene::Type type);
    void ArrowUp();
    void ArrowDown();
    void ArrowLeft();
    void ArrowRight();
    void ArrowLeftUp();
    void ArrowLeftDown();
    void ArrowRightUp();
    void ArrowRightDown();
    void ClickedSizeGrowth();
    void ClickedStandartTable();
    void ClickedLengthLines();
    void ClickedIncrements();
    void ClickedToolButtonPutHere();
    void PutVal(QListWidgetItem * item);
    void ValChenged(int row);
    void ClickedToolButtonEqual();
    void NameChanged();
    void FormulaChanged();
    void DialogAccepted();
    void UpdateList();
protected:
    void closeEvent ( QCloseEvent * event );
private:
    Ui::DialogEndLine *ui;
    const VContainer *data;
    QPushButton *bOk;
    QTimer *timerFormula;
    bool flagFormula;
    bool flagName;
    QString pointName;
    QString typeLine;
    QString formula;
    qint32 angle;
    qint64 basePointId;
    void FillComboBoxBasePoint();
    void ShowBase();
    void ShowStandartTable();
    void ShowIncrementTable();
    void ShowLengthLines();
    void CheackState();
};

#endif // DIALOGENDLINE_H
