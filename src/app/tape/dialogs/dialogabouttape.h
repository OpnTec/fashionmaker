/************************************************************************
 **
 **  @file   dialogabouttape.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 7, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#ifndef DIALOGABOUTTAPE_H
#define DIALOGABOUTTAPE_H

#include <QDialog>

namespace Ui
{
    class DialogAboutTape;
}

class DialogAboutTape : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAboutTape(QWidget *parent = nullptr);
    virtual ~DialogAboutTape();

protected:
    virtual void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(DialogAboutTape)
    Ui::DialogAboutTape *ui;
    bool isInitialized;

    void FontPointSize(QWidget *w, int pointSize);

    void RetranslateUi();
};

#endif // DIALOGABOUTTAPE_H
