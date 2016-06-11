/************************************************************************
 **
 **  @file   dialogincrements.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef DIALOGINCREMENTS_H
#define DIALOGINCREMENTS_H

#include "../vtools/dialogs/tools/dialogtool.h"
#include "../xml/vpattern.h"
#include "../vmisc/vtablesearch.h"

class VIndividualMeasurements;
class QTableWidget;
class QTableWidgetItem;

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
     virtual ~DialogIncrements() Q_DECL_OVERRIDE;

signals:
    /**
     * @brief FullUpdateTree signal update data for dom document
     */
    void                 FullUpdateTree(const Document &parse);

protected:
    virtual void         closeEvent ( QCloseEvent * event ) Q_DECL_OVERRIDE;
    virtual void         changeEvent ( QEvent * event) Q_DECL_OVERRIDE;
private slots:
    void ShowIncrementDetails();
    void AddIncrement();
    void RemoveIncrement();
    void MoveUp();
    void MoveDown();
    void SaveIncrName();
    void SaveIncrDescription();
    void SaveIncrFormula();
    void DeployFormula();
    void Fx();
    void FullUpdateFromFile();
    void Find(const QString &term);
    void FindPrevious();
    void FindNext();

private:
    Q_DISABLE_COPY(DialogIncrements)

    /** @brief ui keeps information about user interface */
    Ui::DialogIncrements *ui;

    /** @brief data container with data */
    VContainer           *data; // need because we must change data //-V703

    /** @brief doc dom document container */
    VPattern             *doc;

    int                  formulaBaseHeight;

    QSharedPointer<VTableSearch> search;

    template <typename T>
    void                 FillTable(const QMap<QString, T> &varTable, QTableWidget *table);

    void                 FillIncrements();
    void                 FillLengthsLines();
    void                 FillLengthLinesAngles();
    void                 FillLengthsCurves();
    void                 FillRadiusesArcs();
    void                 FillAnglesCurves();

    void                 ShowUnits();
    void                 ShowHeaderUnits(QTableWidget *table, int column, const QString &unit);

    void AddCell(QTableWidget *table, const QString &text, int row, int column, int aligment, bool ok = true);

    QString ClearIncrementName(const QString &name) const;
    bool    EvalIncrementFormula(const QString &formula, bool fromUser, VContainer *data, QLabel *label);
    void    Controls();
    void    EnableDetails(bool enabled);
};

#endif // DIALOGINCREMENTS_H
