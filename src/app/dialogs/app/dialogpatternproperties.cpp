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

//---------------------------------------------------------------------------------------------------------------------
DialogPatternProperties::DialogPatternProperties(VPattern *doc, QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogPatternProperties), doc(doc)
{
    ui->setupUi(this);

    SCASSERT(doc != nullptr);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName());
#ifdef Q_OS_WIN
    QString user = settings.value("pattern/user", QString::fromLocal8Bit(qgetenv("USERNAME").constData())).toString();
#else
    QString user = settings.value("pattern/user", QString::fromLocal8Bit(qgetenv("USER").constData())).toString();
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
void DialogPatternProperties::Write(const QString &tagName, const QString &text) const
{
    QDomNodeList nodeList = doc->elementsByTagName(tagName);
    if (nodeList.isEmpty())
    {
        QDomElement pattern = doc->documentElement();

        QDomElement tag = doc->createElement(tagName);
        QDomText domText = doc->createTextNode(text);
        tag.appendChild(domText);
        //Old pattern file doesn't have comment. But here we try insert tag after first child (comment).
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
