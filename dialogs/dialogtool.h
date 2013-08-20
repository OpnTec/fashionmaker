#ifndef DIALOGTOOL_H
#define DIALOGTOOL_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QRadioButton>
#include <QTimer>
#pragma GCC diagnostic pop
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
    virtual void     ChoosedObject(qint64 id, Scene::Type type);
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
    virtual void     ValChenged(int row);
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
    virtual void     closeEvent ( QCloseEvent * event );
    virtual void     showEvent( QShowEvent *event );
    void             FillComboBoxPoints(QComboBox *box, const qint64 &id = 0)const;
    void             FillComboBoxTypeLine(QComboBox *box) const;
    virtual void     CheckState();
    QString          GetTypeLine(const QComboBox *box)const;
    template <class key, class val> void ShowVariable(const QMap<key, val> *var);
    void             SetupTypeLine(QComboBox *box, const QString &value);
    void             ChangeCurrentText(QComboBox *box, const QString &value);
    void             ChangeCurrentData(QComboBox *box, const qint64 &value) const;
    void             PutValHere(QLineEdit *lineEdit, QListWidget *listWidget);
    void             ValFormulaChanged(bool &flag, QLineEdit *edit, QTimer * timer);
    void             Eval(QLineEdit *edit, bool &flag, QTimer *timer, QLabel *label);
    void             setCurrentPointId(QComboBox *box, qint64 &pointId, const qint64 &value, const qint64 &id) const;
    qint64           getCurrentPointId(QComboBox *box) const;
    DialogTool(const DialogTool &dialog);
    const DialogTool &operator=(const DialogTool &dialog);
};

#endif // DIALOGTOOL_H
