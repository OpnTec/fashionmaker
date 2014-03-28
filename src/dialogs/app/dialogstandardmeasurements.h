/************************************************************************
 **
 **  @file   dialogstandardmeasurements.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   21 2, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef DIALOGSTANDARDMEASUREMENTS_H
#define DIALOGSTANDARDMEASUREMENTS_H

#include <QDialog>
#include "../../container/vcontainer.h"

namespace Ui
{
    class DialogStandardMeasurements;
}

class DialogStandardMeasurements : public QDialog
{
    Q_OBJECT
public:
    DialogStandardMeasurements(VContainer *data, const QString &patternPieceName, QWidget *parent = nullptr);
    ~DialogStandardMeasurements();
    QString name() const;
    QString tablePath() const;
public slots:
    void DialogAccepted();
    void DialogRejected();
    void CheckState();
private:
    Q_DISABLE_COPY(DialogStandardMeasurements)
    Ui::DialogStandardMeasurements *ui;
    VContainer *data;
    QString _name;
    QString _tablePath;
    void LoadStandardTables();
};

#endif // DIALOGSTANDARDMEASUREMENTS_H
