/************************************************************************
 **
 **  @file   dialogfinalmeasurements.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   26 9, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#ifndef DIALOGFINALMEASUREMENTS_H
#define DIALOGFINALMEASUREMENTS_H

#include "../vmisc/diagnostic.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wstrict-overflow")
// suppress warning Wstrict-overflow because of method QVector::move()

#include <QDialog>

QT_WARNING_POP

#include "../vmisc/vtablesearch.h"
#include "../vpatterndb/vcontainer.h"
#include "../xml/vpattern.h"

namespace Ui
{
    class DialogFinalMeasurements;
}

class DialogFinalMeasurements : public QDialog
{
    Q_OBJECT

public:
    DialogFinalMeasurements(VPattern *doc, QWidget *parent = nullptr);
    virtual ~DialogFinalMeasurements();

    QVector<VFinalMeasurement> FinalMeasurements() const;

protected:
    virtual void closeEvent ( QCloseEvent * event ) Q_DECL_OVERRIDE;
    virtual void changeEvent ( QEvent * event) Q_DECL_OVERRIDE;
    virtual bool eventFilter(QObject *object, QEvent *event) Q_DECL_OVERRIDE;
    virtual void showEvent( QShowEvent *event ) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
private slots:
    void ShowFinalMeasurementDetails();
    void Add();
    void Remove();
    void MoveUp();
    void MoveDown();
    void SaveName(const QString &text);
    void SaveDescription();
    void SaveFormula();
    void DeployFormula();
    void Fx();
    void FullUpdateFromFile();
private:
    Q_DISABLE_COPY(DialogFinalMeasurements)
    Ui::DialogFinalMeasurements *ui;
    /** @brief doc dom document container */
    VPattern                    *m_doc;
    VContainer                   m_data;
    QVector<VFinalMeasurement>   m_measurements;
    QSharedPointer<VTableSearch> m_search;
    int                          formulaBaseHeight;
    bool                         m_isInitialized;

    void FillFinalMeasurements(bool freshCall = false);

    void ShowUnits();

    void AddCell(const QString &text, int row, int column, int aligment, bool ok = true);
    bool EvalUserFormula(const QString &formula, bool fromUser);
    void Controls();
    void EnableDetails(bool enabled);

    void UpdateTree();

    qreal EvalFormula(const QString &formula, bool &ok);
};

//---------------------------------------------------------------------------------------------------------------------
inline QVector<VFinalMeasurement> DialogFinalMeasurements::FinalMeasurements() const
{
    return m_measurements;
}

#endif // DIALOGFINALMEASUREMENTS_H
