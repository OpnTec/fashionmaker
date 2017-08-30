/************************************************************************
 **
 **  @file   dialogpatternmaterials.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 8, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#include "dialogpatternmaterials.h"
#include "ui_dialogpatternmaterials.h"

#include "../vmisc/def.h"
#include "../core/vapplication.h"

#include <QComboBox>
#include <QItemDelegate>

namespace
{
//---------------------------------------------------------------------------------------------------------------------
QStringList PrepareKnowMaterials(const QStringList &patternMaterials, bool rememberPM)
{
    QStringList knownMaterials = qApp->ValentinaSettings()->GetKnownMaterials();

    if (rememberPM)
    {
        for(int i=0; i < patternMaterials.size(); ++i)
        {
            if (not patternMaterials.at(i).isEmpty() && not knownMaterials.contains(patternMaterials.at(i)))
            {
                knownMaterials.append(patternMaterials.at(i));
            }
        }
    }
    return knownMaterials;
}
}

class VComboBoxDelegate : public QItemDelegate
{
public:
    VComboBoxDelegate(const QStringList &items, QObject *parent = nullptr)
        : QItemDelegate(parent),
          m_items(items)
    {
        m_items.prepend(QLatin1String("--") + tr("Select material") + QLatin1String("--"));
    }

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(option)
        Q_UNUSED(index)

        QComboBox *editor = new QComboBox(parent);
        editor->addItems(m_items);

        return editor;
    }

    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        const QString value = index.model()->data(index, Qt::EditRole).toString();

        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        const int cIndex = comboBox->findText(value);

        if (cIndex != -1)
        {
            comboBox->setCurrentIndex(cIndex);
        }
    }

    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString value;
        const int cIndex = comboBox->currentIndex();

        if (cIndex > 0)
        {
            value = comboBox->currentText();
        }

        model->setData(index, value, Qt::EditRole);
    }

    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(index)
        editor->setGeometry(option.rect);
    }
private:
    Q_DISABLE_COPY(VComboBoxDelegate)
    QStringList m_items;
};

//---------------------------------------------------------------------------------------------------------------------
DialogPatternMaterials::DialogPatternMaterials(const QMap<int, QString> &list, bool rememberPM, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogPatternMaterials),
      m_knownMaterials()
{
    ui->setupUi(this);

    m_knownMaterials = PrepareKnowMaterials(list.values(), rememberPM);
    SetPatternMaterials(list);

    ui->tableWidget->setItemDelegateForColumn(1, new VComboBoxDelegate(m_knownMaterials, this));
}

//---------------------------------------------------------------------------------------------------------------------
DialogPatternMaterials::~DialogPatternMaterials()
{
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
void DialogPatternMaterials::SetPatternMaterials(const QMap<int, QString> &list)
{
    ui->tableWidget->setRowCount(userMaterialPlaceholdersQuantity);
    QLatin1String per("%");

    for (int i = 0; i < userMaterialPlaceholdersQuantity; ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(per + qApp->TrVars()->PlaceholderToUser(pl_userMaterial) +
                                                      QString::number(i + 1) + per);
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        item->setTextAlignment(Qt::AlignLeft);

        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);

        ui->tableWidget->setItem(i, 0, item);

        QString value;
        if (list.contains(i + 1))
        {
            value = list.value(i + 1);
        }

        item = new QTableWidgetItem(value);
        item->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidget->setItem(i, 1, item);
    }

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);
}

//---------------------------------------------------------------------------------------------------------------------
QMap<int, QString> DialogPatternMaterials::GetPatternMaterials() const
{
    QMap<int, QString> materials;

    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        const QTableWidgetItem *item = ui->tableWidget->item(i, 1);
        if (not item->text().isEmpty())
        {
            materials.insert(i + 1, item->text());
        }
    }

    return materials;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList DialogPatternMaterials::GetKnownMaterials() const
{
    return m_knownMaterials;
}
