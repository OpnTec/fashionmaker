#ifndef DIALOGTOOL_H
#define DIALOGTOOL_H

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QRadioButton>
#include <QTimer>
#include "../container/vcontainer.h"
#include "../options.h"

class DialogTool : public QDialog
{
    Q_OBJECT
public:
                     DialogTool(const VContainer *data, QWidget *parent = 0);
    virtual          ~DialogTool();
signals:
    void             DialogClosed(int result);
public slots:
    virtual void     ChoosedPoint(qint64 id, Scene::Type type);
    void             NamePointChanged();
    virtual void     DialogAccepted();
    virtual void     DialogRejected();
    void             FormulaChanged();
    void             ArrowUp();
    void             ArrowDown();
    void             ArrowLeft();
    void             ArrowRight();
    void             ArrowLeftUp();
    void             ArrowLeftDown();
    void             ArrowRightUp();
    void             ArrowRightDown();
    void             EvalFormula();
    void             SizeGrowth();
    void             StandartTable();
    void             LengthLines();
    void             Increments();
    void             PutHere();
    void             PutVal(QListWidgetItem * item);
    void             ValChenged(int row);
    void             UpdateList();
protected:
    const VContainer *data;
    bool             isInitialized;
    bool             flagName;
    bool             flagFormula;
    QTimer           *timerFormula;
    QPushButton      *bOk;
    QSpinBox         *spinBoxAngle;
    QLineEdit        *lineEditFormula;
    QListWidget      *listWidget;
    QLabel           *labelResultCalculation;
    QLabel           *labelDescription;
    QRadioButton     *radioButtonSizeGrowth;
    QRadioButton     *radioButtonStandartTable;
    QRadioButton     *radioButtonIncrements;
    QRadioButton     *radioButtonLengthLine;
    void             closeEvent ( QCloseEvent * event );
    void             showEvent( QShowEvent *event );
    void             FillComboBoxPoints(QComboBox *box)const;
    void             FillComboBoxTypeLine(QComboBox *box) const;
    void             CheckState();
    QString          GetTypeLine(const QComboBox *box)const;
    void             ShowBase();
    void             ShowStandartTable();
    void             ShowIncrementTable();
    void             ShowLengthLines();
};

#endif // DIALOGTOOL_H
