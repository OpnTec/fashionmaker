/************************************************************************
 **
 **  @file   dialogpatternproperties.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 2, 2014
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

#ifndef DIALOGPATTERNPROPERTIES_H
#define DIALOGPATTERNPROPERTIES_H

#include <QDialog>
#include "../../options.h"
#include <QMap>

class VPattern;
class QCheckBox;

namespace Ui
{
    class DialogPatternProperties;
}

class DialogPatternProperties : public QDialog
{
    Q_OBJECT
public:
    DialogPatternProperties(VPattern *doc, QWidget *parent = nullptr);
    virtual ~DialogPatternProperties();
signals:
    void         UpdateGradation();
public slots:
    void         Apply();
    void         Ok();
    void         SelectAll(int state);
    void         CheckStateHeight(int state);
    void         CheckStateSize(int state);
    void         DescEdited();
private:
    Q_DISABLE_COPY(DialogPatternProperties)
    Ui::DialogPatternProperties *ui;
    VPattern               *doc;
    char                   heightsChecked;
    char                   sizesChecked;
    QMap<GHeights, bool>   heights;
    QMap<GSizes, bool>     sizes;
    QMap<QCheckBox *, int> data;
    bool                   descriptionChanged;
    bool                   gradationChanged;

    void         SetHeightsChecked(bool enabled);
    void         SetSizesChecked(bool enabled);
    void         InitHeights();
    void         InitSizes();
    template<typename Func>
    void         Init(QCheckBox *check, int val, Func slot);
    template<typename GVal>
    void         SetOptions(const QMap<GVal, bool> &option);
    void         CheckApplyOk();
    void         SaveDescription();
    void         SaveGradation();
};

#endif // DIALOGPATTERNPROPERTIES_H
