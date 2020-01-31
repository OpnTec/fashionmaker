/************************************************************************
 **
 **  @file   dialogincrements.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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

#ifndef DIALOGINCREMENTS_H
#define DIALOGINCREMENTS_H

#include "../vtools/dialogs/tools/dialogtool.h"
#include "../xml/vpattern.h"
#include "../vmisc/vtablesearch.h"

#include <QPair>

class VIndividualMeasurements;

namespace Ui
{
    class DialogIncrements;
}

/**
 * @brief The DialogIncrements class show dialog increments. Tables of all variables in program will be here.
 */
class DialogIncrements : public DialogTool
{
    Q_OBJECT
public:
    DialogIncrements(VContainer *data, VPattern *doc, QWidget *parent = nullptr);
    virtual ~DialogIncrements() override;

    void RestoreAfterClose();

signals:
    void UpdateProperties();

public slots:
    void FullUpdateFromFile();

protected:
    virtual void closeEvent ( QCloseEvent * event ) override;
    virtual void changeEvent ( QEvent * event) override;
    virtual bool eventFilter(QObject *object, QEvent *event) override;
    virtual void showEvent( QShowEvent *event ) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual bool IsValid() const final {return true;}
private slots:
    void ShowIncrementDetails();
    void AddIncrement();
    void AddSeparator();
    void RemoveIncrement();
    void MoveUp();
    void MoveDown();
    void SaveIncrName(const QString &text);
    void SaveIncrDescription();
    void SaveIncrFormula();
    void DeployFormula();
    void Fx();
    void RefreshPattern();

private:
    Q_DISABLE_COPY(DialogIncrements)

    /** @brief ui keeps information about user interface */
    Ui::DialogIncrements *ui;

    /** @brief data container with data */
    VContainer           *data; // need because we must change data //-V703

    /** @brief doc dom document container */
    VPattern             *doc;

    /** @brief m_completeData need to show all internal variables */
    VContainer           m_completeData;

    int                  formulaBaseHeight;
    int                  formulaBaseHeightPC;

    QSharedPointer<VTableSearch> search;
    QSharedPointer<VTableSearch> searchPC;

    bool hasChanges;

    QVector<QPair<QString, QString>> renameList;

    template <typename T>
    void                 FillTable(const QMap<QString, T> &varTable, QTableWidget *table);

    void                 FillIncrementsTable(QTableWidget *table,
                                             const QMap<QString, QSharedPointer<VIncrement> > &increments,
                                             bool takePreviewCalculations, bool freshCall = false);

    void                 FillIncrements(bool freshCall = false);
    void                 FillPreviewCalculations(bool freshCall = false);
    void                 FillLengthsLines();
    void                 FillLengthLinesAngles();
    void                 FillLengthsCurves();
    void                 FillCurvesCLengths();
    void                 FillRadiusesArcs();
    void                 FillAnglesCurves();

    void                 ShowUnits();
    void                 ShowHeaderUnits(QTableWidget *table, int column, const QString &unit);

    QTableWidgetItem* AddCell(QTableWidget *table, const QString &text, int row, int column, int aligment,
                              bool ok = true);
    QTableWidgetItem* AddSeparatorCell(QTableWidget *table, const QString &text, int row, int column, int aligment,
                                       bool ok = true);

    QString GetCustomName() const;
    QString ClearIncrementName(const QString &name) const;

    bool    EvalIncrementFormula(const QString &formula, bool fromUser, VContainer *data, QLabel *label);
    void    Controls(QTableWidget *table);
    void    EnableDetails(QTableWidget *table, bool enabled);

    void LocalUpdateTree();

    bool IncrementUsed(const QString &name) const;

    void CacheRename(const QString &name, const QString &newName);

    void ShowTableIncrementDetails(QTableWidget *table);

    QMenu *InitVarTypeMenu(QMenu *menu, bool incrementTab);

    void AddNewIncrement(IncrementType type);
};

#endif // DIALOGINCREMENTS_H
