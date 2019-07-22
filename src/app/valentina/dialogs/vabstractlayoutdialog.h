/************************************************************************
 **
 **  @file   dialogsavelayout.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 10, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2017 Valentina project
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

#ifndef VABSTRACTLAYOUTDIALOG_H
#define VABSTRACTLAYOUTDIALOG_H

#include <QComboBox>
#include <QDialog>

enum class Unit : char;

class VAbstractLayoutDialog : public QDialog
{
public:
    enum class PaperSizeTemplate : char { A0 = 0,
                                          A1,
                                          A2,
                                          A3,
                                          A4,
                                          Letter,
                                          Legal,
                                          Tabloid,
                                          Roll24in, // Be carefull when change order for roll types
                                          Roll30in, // Used also for showing icon
                                          Roll36in,
                                          Roll42in,
                                          Roll44in,
                                          Roll48in,
                                          Roll62in,
                                          Roll72in,
                                          Custom};

    explicit VAbstractLayoutDialog(QWidget *parent = nullptr);

    static QSizeF GetTemplateSize(PaperSizeTemplate tmpl, Unit unit);

protected:
    typedef QStringList FormatsVector;
    const static FormatsVector pageFormatNames;
    typedef int VIndexType;

    static QSizeF RoundTemplateSize(qreal width, qreal height, Unit unit);

    void InitTemplates(QComboBox *comboBoxTemplates);
private:
    Q_DISABLE_COPY(VAbstractLayoutDialog)
};

#endif // VABSTRACTDIALOG_H
