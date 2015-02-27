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

#include "../tools/dialogtool.h"
#include "../../xml/vpattern.h"

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
     ~DialogIncrements();
public slots:
    void                 clickedToolButtonAdd();
    void                 clickedToolButtonRemove();
    void                 IncrementChanged ( qint32 row, qint32 column );
    void                 MeasurementChanged ( qint32 row, qint32 column );
    void                 FullUpdateFromFile();
    void                 SaveGivenName();
    void                 SaveFamilyName();
    void                 SaveEmail();
    void                 SaveSex(int index);
    void                 SaveBirthDate(const QDate & date);
    void                 OpenTable();
    void                 ShowMeasurements();
signals:
    /**
     * @brief FullUpdateTree signal update data for dom document
     */
    void                 FullUpdateTree(const Document &parse);
    /**
     * @brief haveLiteChange signal show sign of change
     */
    void                 haveLiteChange();
protected:
    virtual void         closeEvent ( QCloseEvent * event );
private:
    Q_DISABLE_COPY(DialogIncrements)

    /** @brief ui keeps information about user interface */
    Ui::DialogIncrements *ui;

    /** @brief data container with data */
    VContainer           *data; // need because we must change data

    /** @brief doc dom document container */
    VPattern             *doc;

    /** @brief row save number of row current selected cell */
    qint32               row;

    /** @brief column save number of column current selected cell */
    qint32               column;

    VIndividualMeasurements *m;

    void                 FillMeasurements();
    void                 FillIncrements();
    template <typename T>
    void                 FillTable(const QMap<QString, T> varTable, QTableWidget *table);
    void                 FillLengthLines();
    void                 FillLengthSplines();
    void                 FillLengthArcs();
    void                 AddIncrementToFile(const quint32 &id, const QString &name, const qreal &base,
                                            const qreal &ksize, const qreal &kheight, const QString &description);
    void                 HideColumns(QTableWidget *table);
    void                 SetItemViewOnly(QTableWidgetItem *item);
    void                 ShowUnits();
    void                 ShowHeaderUnits(QTableWidget *table, int column);
    void                 ShowSuccess() const;
};

#endif // DIALOGINCREMENTS_H
