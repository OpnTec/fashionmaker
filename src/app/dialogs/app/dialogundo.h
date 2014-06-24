/************************************************************************
 **
 **  @file   dialogundo.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 6, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef DIALOGUNDO_H
#define DIALOGUNDO_H

#include <QDialog>

namespace Ui
{
    class DialogUndo;
}

enum class UndoButton {Undo, Fix, Cancel};

class DialogUndo : public QDialog
{
    Q_OBJECT
public:
    explicit DialogUndo(QWidget *parent = 0);
    UndoButton Result() const;
    ~DialogUndo();
public slots:
    void Undo();
    void Fix();
    void Cancel();
protected:
    virtual void closeEvent ( QCloseEvent * event );
private:
    Q_DISABLE_COPY(DialogUndo)
    Ui::DialogUndo *ui;
    UndoButton result;
};

inline UndoButton DialogUndo::Result() const
{
    return result;
}

#endif // DIALOGUNDO_H
