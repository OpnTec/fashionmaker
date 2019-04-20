/************************************************************************
 **
 **  @file   dialogtoolbox.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 1, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/
#ifndef DIALOGTOOLBOX_H
#define DIALOGTOOLBOX_H

#include <QString>
#include <QtGlobal>
#include <QSharedPointer>

#include "../vpatterndb/variables/vinternalvariable.h"

class QPlainTextEdit;
class QPushButton;
class QDialog;
class QObject;
class QEvent;
class QLabel;
class QWidget;
class QColor;
class QLineEdit;
class VContainer;
class QListWidget;
class VPieceNode;

constexpr int formulaTimerTimeout = 300;

extern const QColor errorColor;

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")

struct FormulaData
{
    QString formula;
    const QHash<QString, QSharedPointer<VInternalVariable> > *variables{nullptr};
    QLabel *labelEditFormula{nullptr};
    QLabel *labelResult{nullptr};
    QString postfix;
    bool checkZero{true};
    bool checkLessThanZero{false};
};

QT_WARNING_POP

VPieceNode RowNode(QListWidget *listWidget, int i);
void   MoveCursorToEnd(QPlainTextEdit *plainTextEdit);
void   DeployFormula(QDialog *dialog, QPlainTextEdit *formula, QPushButton *buttonGrowLength, int formulaBaseHeight);
bool   FilterObject(QObject *object, QEvent *event);
qreal  EvalToolFormula(QDialog *dialog, const FormulaData &data, bool &flag);
void   ChangeColor(QWidget *widget, const QColor &color);
QColor OkColor(QWidget *widget);
void   CheckPointLabel(QDialog *dialog, QLineEdit* edit, QLabel *labelEditNamePoint, const QString &pointName,
                       const VContainer *data, bool &flag);
int    FindNotExcludedNodeDown(QListWidget *listWidget, int candidate);
int    FindNotExcludedNodeUp(QListWidget *listWidget, int candidate);
bool   FirstPointEqualLast(QListWidget *listWidget, const VContainer *data);
bool   DoublePoints(QListWidget *listWidget, const VContainer *data);
bool   DoubleCurves(QListWidget *listWidget);
bool   EachPointLabelIsUnique(QListWidget *listWidget);
QString DialogWarningIcon();
QFont  NodeFont(QFont font, bool nodeExcluded = false);

#endif // DIALOGTOOLBOX_H
