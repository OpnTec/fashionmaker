/************************************************************************
 **
 **  @file   vtablesearch.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 9, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "vtablesearch.h"

#include <QPalette>
#include <QStringBuilder>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <Qt>

#include "../vmisc/def.h"

//---------------------------------------------------------------------------------------------------------------------
VTableSearch::VTableSearch(QTableWidget *table, QObject *parent)
    : QObject(parent),
      table(table),
      searchIndex(-1),
      searchList()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VTableSearch::Clear()
{
    SCASSERT(table != nullptr)

    for(int i = 0; i < table->rowCount(); ++i)
    {
        for(int j = 0; j < table->columnCount(); ++j)
        {
            if (QTableWidgetItem *item = table->item(i, j))
            {
                if (item->row() % 2 != 0 && table->alternatingRowColors())
                {
                    item->setBackground(QPalette().alternateBase());
                }
                else
                {
                    item->setBackground(QPalette().base());
                }
            }
        }
    }

    searchList.clear();
    searchIndex = -1;

    emit HasResult(false);
}

//---------------------------------------------------------------------------------------------------------------------
void VTableSearch::ShowNext(int newIndex)
{
    if (not searchList.isEmpty())
    {
        QTableWidgetItem *item = searchList.at(searchIndex);
        item->setBackground(Qt::yellow);

        item = searchList.at(newIndex);
        item->setBackground(Qt::red);
        table->scrollToItem(item);
        searchIndex = newIndex;
    }
    else
    {
        Clear();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QList<QTableWidgetItem *> VTableSearch::FindTableItems(const QString& term)
{
    if (term.isEmpty())
    {
        return QList<QTableWidgetItem *>();
    }

    if (term.startsWith(QChar('/')))
    {
        QRegularExpression qre(QStringLiteral("^/(?<searchType>[^/]+)/(?<searchString>.+)$"));
        QScopedPointer<QRegularExpressionMatch> match(new QRegularExpressionMatch());
        if (!term.contains(qre, match.data()))
        {
            return QList<QTableWidgetItem *>();
        }

        auto searchType = match->capturedRef(QStringLiteral("searchType"));
        auto searchString = match->capturedRef(QStringLiteral("searchString"));
        if (searchType == QChar('r'))
        {
            QString reSearchString = ".*" % searchString % ".*";
            return table->findItems(reSearchString, Qt::MatchRegExp);
        }

        return QList<QTableWidgetItem *>();
    }

    return table->findItems(term, Qt::MatchContains);
}

//---------------------------------------------------------------------------------------------------------------------
void VTableSearch::Find(const QString &term)
{
    SCASSERT(table != nullptr)

    Clear();

    searchList = FindTableItems(term);

    if (not searchList.isEmpty())
    {
        for (auto item : qAsConst(searchList))
        {
            item->setBackground(Qt::yellow);
        }

        searchIndex = 0;
        QTableWidgetItem *item = searchList.at(searchIndex);
        item->setBackground(Qt::red);
        table->scrollToItem(item);

        emit HasResult(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTableSearch::FindPrevious()
{
    int newIndex = searchIndex - 1;

    if (newIndex < 0)
    {
        newIndex = searchList.size() - 1;
    }

    ShowNext(newIndex);
}

//---------------------------------------------------------------------------------------------------------------------
void VTableSearch::FindNext()
{
    int newIndex = searchIndex + 1;

    if (newIndex >= searchList.size())
    {
        newIndex = 0;
    }

    ShowNext(newIndex);
}

//---------------------------------------------------------------------------------------------------------------------
void VTableSearch::RemoveRow(int row)
{
    if (searchIndex < 0 || searchIndex >= searchList.size())
    {
        return;
    }

    const int indexRow = searchList.at(searchIndex)->row();

    if (row <= indexRow)
    {
        for (auto item : qAsConst(searchList))
        {
            if (item->row() == row)
            {
                --searchIndex;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTableSearch::AddRow(int row)
{
    if (searchIndex < 0 || searchIndex >= searchList.size())
    {
        return;
    }

    const int indexRow = searchList.at(searchIndex)->row();

    if (row <= indexRow)
    {
        for (auto item : qAsConst(searchList))
        {
            if (item->row() == row)
            {
                ++searchIndex;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTableSearch::RefreshList(const QString &term)
{
    SCASSERT(table != nullptr)

    searchList = FindTableItems(term);

    if (not searchList.isEmpty())
    {
        for (auto item : qAsConst(searchList))
        {
            item->setBackground(Qt::yellow);
        }

        if (searchIndex < 0)
        {
           searchIndex = searchList.size() - 1;
        }
        else if (searchIndex >= searchList.size())
        {
           searchIndex = 0;
        }

        QTableWidgetItem *item = searchList.at(searchIndex);
        item->setBackground(Qt::red);
        table->scrollToItem(item);

        emit HasResult(true);
    }
    else
    {
        emit HasResult(false);
    }
}
