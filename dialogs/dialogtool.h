/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef DIALOGTOOL_H
#define DIALOGTOOL_H

#include <QDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QRadioButton>
#include <QTimer>
#include "container/vcontainer.h"
#include "options.h"

class DialogTool : public QDialog
{
    Q_OBJECT
public:
                     DialogTool(const VContainer *data, Draw::Draws mode = Draw::Calculation,
                                QWidget *parent = 0);
    virtual          ~DialogTool();
    qint64           getIdDetail() const;
    void             setIdDetail(const qint64 &value);
signals:
    void             DialogClosed(int result);
public slots:
    virtual void     ChoosedObject(qint64 id, Scene::Scenes type);
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
    Q_DISABLE_COPY(DialogTool)
    const VContainer *data;
    bool             isInitialized;
    bool             flagName;
    bool             flagFormula;
    QTimer           *timerFormula;
    QPushButton      *bOk;
    QDoubleSpinBox   *spinBoxAngle;
    QLineEdit        *lineEditFormula;
    QListWidget      *listWidget;
    QLabel           *labelResultCalculation;
    QLabel           *labelDescription;
    QRadioButton     *radioButtonSizeGrowth;
    QRadioButton     *radioButtonStandartTable;
    QRadioButton     *radioButtonIncrements;
    QRadioButton     *radioButtonLengthLine;
    qint64           idDetail;
    Draw::Draws       mode;
    bool             CheckObject(const qint64 &id);
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
};

#endif // DIALOGTOOL_H
