/************************************************************************
 **
 **  @file   dialogindividualmeasurements.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   22 2, 2014
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

#ifndef DIALOGINDIVIDUALMEASUREMENTS_H
#define DIALOGINDIVIDUALMEASUREMENTS_H

#include <QDialog>

namespace Ui {
class DialogIndividualMeasurements;
}

class DialogIndividualMeasurements : public QDialog
{
    Q_OBJECT

public:
    explicit DialogIndividualMeasurements(QWidget *parent = 0);
    ~DialogIndividualMeasurements();
    inline QString name() const {return _name;}
    inline QString tablePath() const{return _tablePath;}
private:
    Q_DISABLE_COPY(DialogIndividualMeasurements)
    Ui::DialogIndividualMeasurements *ui;
    QString _name;
    QString _tablePath;
    void DialogAccepted();
    void DialogRejected();
    void CheckState();
};

#endif // DIALOGINDIVIDUALMEASUREMENTS_H
