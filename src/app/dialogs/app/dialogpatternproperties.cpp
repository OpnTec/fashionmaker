/************************************************************************
 **
 **  @file   dialogpatternproperties.cpp
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

#include "dialogpatternproperties.h"
#include "ui_dialogpatternproperties.h"
#include <QSettings>
#include <QPushButton>
#include "../../xml/vpattern.h"
#include "../../widgets/vapplication.h"

#define MAX_HEIGHTS 18
#define MAX_SIZES 18

//---------------------------------------------------------------------------------------------------------------------
DialogPatternProperties::DialogPatternProperties(VPattern *doc, QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogPatternProperties), doc(doc), heightsChecked(MAX_HEIGHTS), sizesChecked(MAX_SIZES)
{
    ui->setupUi(this);

    SCASSERT(doc != nullptr);

    QSettings *settings = qApp->getSettings();
    SCASSERT(settings != nullptr);
#ifdef Q_OS_WIN
    QString user = settings->value("pattern/user", QString::fromLocal8Bit(qgetenv("USERNAME").constData())).toString();
#else
    QString user = settings->value("pattern/user", QString::fromLocal8Bit(qgetenv("USER").constData())).toString();
#endif

    ui->lineEditAuthor->setText(this->doc->UniqueTagText("author", user));
    ui->plainTextEditDescription->setPlainText(this->doc->UniqueTagText("description"));
    ui->plainTextEditTechNotes->setPlainText(this->doc->UniqueTagText("notes"));

    QPushButton *bOk = ui->buttonBox->button(QDialogButtonBox::Ok);
    SCASSERT(bOk != nullptr);
    connect(bOk, &QPushButton::clicked, this, &DialogPatternProperties::Apply);

    QPushButton *bCansel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    SCASSERT(bCansel != nullptr);
    connect(bCansel, &QPushButton::clicked, this, &DialogPatternProperties::close);

    connect(this, &DialogPatternProperties::haveChange, this->doc, &VPattern::haveLiteChange);

    ui->tabWidget->setCurrentIndex(0);
    if (qApp->patternType() == MeasurementsType::Individual)
    {
        ui->tabWidget->setTabEnabled(1, false);
    }
    connect(ui->checkBoxAllHeights, &QCheckBox::stateChanged, this, &DialogPatternProperties::SelectAll);
    connect(ui->checkBoxAllSizes, &QCheckBox::stateChanged, this, &DialogPatternProperties::SelectAll);

    connect(ui->checkBoxH92, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH98, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH104, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH110, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH116, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH122, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH128, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH134, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH140, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH146, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH152, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH158, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH164, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH170, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH176, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH182, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH188, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);
    connect(ui->checkBoxH194, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckHeight);

    connect(ui->checkBoxS22, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS24, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS26, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS28, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS30, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS32, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS34, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS36, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS38, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS40, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS42, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS44, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS46, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS48, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS50, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS52, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS54, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
    connect(ui->checkBoxS56, &QCheckBox::stateChanged, this, &DialogPatternProperties::UncheckSize);
}

//---------------------------------------------------------------------------------------------------------------------
DialogPatternProperties::~DialogPatternProperties()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::Apply()
{
    Write("notes", ui->plainTextEditTechNotes->document()->toPlainText());
    Write("description", ui->plainTextEditDescription->document()->toPlainText());
    Write("author", ui->lineEditAuthor->text());
    emit haveChange();
    close();
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SelectAll(int state)
{
    QCheckBox* box = qobject_cast<QCheckBox*>(sender());
    if (box)
    {
        if (box == ui->checkBoxAllHeights)
        {
            if (state == Qt::Checked)
            {
                ui->checkBoxH92->setChecked(true);
                ui->checkBoxH98->setChecked(true);
                ui->checkBoxH104->setChecked(true);
                ui->checkBoxH110->setChecked(true);
                ui->checkBoxH116->setChecked(true);
                ui->checkBoxH122->setChecked(true);
                ui->checkBoxH128->setChecked(true);
                ui->checkBoxH134->setChecked(true);
                ui->checkBoxH140->setChecked(true);
                ui->checkBoxH146->setChecked(true);
                ui->checkBoxH152->setChecked(true);
                ui->checkBoxH158->setChecked(true);
                ui->checkBoxH164->setChecked(true);
                ui->checkBoxH170->setChecked(true);
                ui->checkBoxH176->setChecked(true);
                ui->checkBoxH182->setChecked(true);
                ui->checkBoxH188->setChecked(true);
                ui->checkBoxH194->setChecked(true);

                SetHeightsEnabled(false);
            }
            else if (state == Qt::Unchecked)
            {
                SetHeightsEnabled(true);
            }
        }

        if (box == ui->checkBoxAllSizes)
        {
            if (state == Qt::Checked)
            {
                ui->checkBoxS22->setChecked(true);
                ui->checkBoxS24->setChecked(true);
                ui->checkBoxS26->setChecked(true);
                ui->checkBoxS28->setChecked(true);
                ui->checkBoxS30->setChecked(true);
                ui->checkBoxS32->setChecked(true);
                ui->checkBoxS34->setChecked(true);
                ui->checkBoxS36->setChecked(true);
                ui->checkBoxS38->setChecked(true);
                ui->checkBoxS40->setChecked(true);
                ui->checkBoxS42->setChecked(true);
                ui->checkBoxS44->setChecked(true);
                ui->checkBoxS46->setChecked(true);
                ui->checkBoxS48->setChecked(true);
                ui->checkBoxS50->setChecked(true);
                ui->checkBoxS52->setChecked(true);
                ui->checkBoxS54->setChecked(true);
                ui->checkBoxS56->setChecked(true);

                SetSizesEnabled(false);
            }
            else if (state == Qt::Unchecked)
            {
                SetSizesEnabled(true);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::UncheckHeight(int state)
{
    QCheckBox* box = qobject_cast<QCheckBox*>(sender());
    if (box)
    {
        if (box == ui->checkBoxH92  || box == ui->checkBoxH98  || box == ui->checkBoxH104 || box == ui->checkBoxH110 ||
            box == ui->checkBoxH116 || box == ui->checkBoxH122 || box == ui->checkBoxH128 || box == ui->checkBoxH134 ||
            box == ui->checkBoxH140 || box == ui->checkBoxH146 || box == ui->checkBoxH152 || box == ui->checkBoxH158 ||
            box == ui->checkBoxH164 || box == ui->checkBoxH170 || box == ui->checkBoxH176 || box == ui->checkBoxH182 ||
            box == ui->checkBoxH188 || box == ui->checkBoxH194)
        {
            if (state == Qt::Checked)
            {
                ++heightsChecked;
            }
            else if (state == Qt::Unchecked)
            {
                if (heightsChecked == 1)
                {
                    box->setCheckState(Qt::Checked);//Will call this method again with Qt::Checked state
                    --heightsChecked;// That's why we will increase.
                }
                else
                {
                    --heightsChecked;
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::UncheckSize(int state)
{
    QCheckBox* box = qobject_cast<QCheckBox*>(sender());
    if (box)
    {
        if (box == ui->checkBoxS22 || box == ui->checkBoxS24 || box == ui->checkBoxS26 || box == ui->checkBoxS28 ||
            box == ui->checkBoxS30 || box == ui->checkBoxS32 || box == ui->checkBoxS34 || box == ui->checkBoxS36 ||
            box == ui->checkBoxS38 || box == ui->checkBoxS40 || box == ui->checkBoxS42 || box == ui->checkBoxS44 ||
            box == ui->checkBoxS46 || box == ui->checkBoxS48 || box == ui->checkBoxS50 || box == ui->checkBoxS52 ||
            box == ui->checkBoxS54 || box == ui->checkBoxS56)
        {
            if (state == Qt::Checked)
            {
                ++sizesChecked;
            }
            else if (state == Qt::Unchecked)
            {
                if (sizesChecked == 1)
                {
                    box->setCheckState(Qt::Checked);//Will call this method again with Qt::Checked state
                    --sizesChecked; // That's why we will increase.
                }
                else
                {
                    --sizesChecked;
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::Write(const QString &tagName, const QString &text) const
{
    QDomNodeList nodeList = doc->elementsByTagName(tagName);
    if (nodeList.isEmpty())
    {
        QDomElement pattern = doc->documentElement();

        QDomElement tag = doc->createElement(tagName);
        QDomText domText = doc->createTextNode(text);
        tag.appendChild(domText);
        //The Old pattern file doesn't have comment. But here we try insert the tag after first child (comment).
        // <pattern>
        //      <!--Valentina pattern format.-->
        //      -->place for new tag<--
        // </pattern>
        if (pattern.firstChild().isComment())
        {
            pattern.insertAfter(tag, pattern.firstChild());
        }
        else
        {
            pattern.insertBefore(tag, pattern.firstChild());
        }
    }
    else
    {
        QDomElement oldTag = nodeList.at(0).toElement();
        if (oldTag.isElement())
        {
            QDomElement newTag = doc->createElement(tagName);
            QDomText domText = doc->createTextNode(text);
            newTag.appendChild(domText);

            QDomElement pattern = doc->documentElement();
            pattern.replaceChild(newTag, oldTag);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SetHeightsEnabled(bool enabled)
{
    ui->checkBoxH92->setEnabled(enabled);
    ui->checkBoxH98->setEnabled(enabled);
    ui->checkBoxH104->setEnabled(enabled);
    ui->checkBoxH110->setEnabled(enabled);
    ui->checkBoxH116->setEnabled(enabled);
    ui->checkBoxH122->setEnabled(enabled);
    ui->checkBoxH128->setEnabled(enabled);
    ui->checkBoxH134->setEnabled(enabled);
    ui->checkBoxH140->setEnabled(enabled);
    ui->checkBoxH146->setEnabled(enabled);
    ui->checkBoxH152->setEnabled(enabled);
    ui->checkBoxH158->setEnabled(enabled);
    ui->checkBoxH164->setEnabled(enabled);
    ui->checkBoxH170->setEnabled(enabled);
    ui->checkBoxH176->setEnabled(enabled);
    ui->checkBoxH182->setEnabled(enabled);
    ui->checkBoxH188->setEnabled(enabled);
    ui->checkBoxH194->setEnabled(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternProperties::SetSizesEnabled(bool enabled)
{
    ui->checkBoxS22->setEnabled(enabled);
    ui->checkBoxS24->setEnabled(enabled);
    ui->checkBoxS26->setEnabled(enabled);
    ui->checkBoxS28->setEnabled(enabled);
    ui->checkBoxS30->setEnabled(enabled);
    ui->checkBoxS32->setEnabled(enabled);
    ui->checkBoxS34->setEnabled(enabled);
    ui->checkBoxS36->setEnabled(enabled);
    ui->checkBoxS38->setEnabled(enabled);
    ui->checkBoxS40->setEnabled(enabled);
    ui->checkBoxS42->setEnabled(enabled);
    ui->checkBoxS44->setEnabled(enabled);
    ui->checkBoxS46->setEnabled(enabled);
    ui->checkBoxS48->setEnabled(enabled);
    ui->checkBoxS50->setEnabled(enabled);
    ui->checkBoxS52->setEnabled(enabled);
    ui->checkBoxS54->setEnabled(enabled);
    ui->checkBoxS56->setEnabled(enabled);
}
