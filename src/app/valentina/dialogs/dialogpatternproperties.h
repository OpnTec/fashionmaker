/************************************************************************
 **
 **  @file   dialogpatternproperties.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 2, 2014
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

#ifndef DIALOGPATTERNPROPERTIES_H
#define DIALOGPATTERNPROPERTIES_H

#include <QDialog>
#include <QMap>

#include "../vmisc/def.h"
#include "../ifc/ifcdef.h"

class VPattern;
class VContainer;
class QCheckBox;

namespace Ui
{
    class DialogPatternProperties;
}

class DialogPatternProperties : public QDialog
{
    Q_OBJECT
public:
    explicit DialogPatternProperties(VPattern *doc, VContainer *pattern, QWidget *parent = nullptr);
    virtual ~DialogPatternProperties() override;
signals:
    void UpdateGradation();
private slots:
    void DefValueChanged();
    void LabelDataChanged();
    void Apply();
    void Ok();
    void SelectAll(int state);
    void CheckStateHeight(int state);
    void CheckStateSize(int state);
    void DescEdited();
    void ChangeImage();
    void SaveImage();
    void EditLabel();
    void ManagePatternMaterials();
private:
    Q_DISABLE_COPY(DialogPatternProperties)
    Ui::DialogPatternProperties *ui;
    VPattern               *doc;
    VContainer             *pattern;
    int                    heightsChecked;
    int                    sizesChecked;
    QMap<GHeights, bool>   heights;
    QMap<GSizes, bool>     sizes;
    QMap<QCheckBox *, int> data;
    bool                   descriptionChanged;
    bool                   gradationChanged;
    bool                   defaultChanged;
    bool                   securityChanged;
    bool                   labelDataChanged;
    bool                   askSaveLabelData;
    bool                   templateDataChanged;
    bool                   patternMaterialsChanged;
    QAction                *deleteAction;
    QAction                *changeImageAction;
    QAction                *saveImageAction;
    QAction                *showImageAction;

    QVector<VLabelTemplateLine> templateLines;
    QMap<int, QString>          patternMaterials;

    void         SetHeightsChecked(bool enabled);
    void         SetSizesChecked(bool enabled);
    void         InitHeights();
    void         InitSizes();
    template<typename Func>
    void         Init(QCheckBox *check, int val, Func slot);
    template<typename GVal>
    void         SetOptions(const QMap<GVal, bool> &option);
    template<typename GVal>
    void         InitComboBox(QComboBox *box, const QMap<GVal, bool> &option);
    void         InitComboBoxFormats(QComboBox *box, const QStringList &items, const QString &currentFormat);
    void         CheckApplyOk();
    void         SaveDescription();
    void         SaveGradation();
    void         SaveDefValues();
    void         SaveLabelData();
    void         SaveTemplateData();
    void         SaveMaterialData();
    void         SaveReadOnlyState();

    void         SetDefaultHeight(const QString &def);
    void         SetDefaultSize(const QString &def);

    void         UpdateDefHeight();
    void         UpdateDefSize();
    void         InitImage();
    QImage       GetImage();
};

#endif // DIALOGPATTERNPROPERTIES_H
