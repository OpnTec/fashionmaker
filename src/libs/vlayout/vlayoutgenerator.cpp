/************************************************************************
 **
 **  @file   vlayoutgenerator.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 1, 2015
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

#include "vlayoutgenerator.h"
#include "vlayoutpaper.h"
#include "vlayoutdetail.h"

#include <QRectF>

//---------------------------------------------------------------------------------------------------------------------
VLayoutGenerator::VLayoutGenerator(QObject *parent)
    :QObject(parent), papers(QVector<VLayoutPaper>()), bank(new VBank()), paperHeight(0), paperWidth(0),
      stopGeneration(false), state(LayoutErrors::NoError)
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutGenerator::~VLayoutGenerator()
{
    delete bank;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetDetails(const QVector<VLayoutDetail> &details)
{
    bank->SetDetails(details);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetLayoutWidth(qreal width)
{
    bank->SetLayoutWidth(width);
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetCaseType(Cases caseType)
{
    bank->SetCaseType(caseType);
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutGenerator::DetailsCount()
{
    return bank->AllDetailsCount();
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::Generate()
{
    stopGeneration = false;
    papers.clear();
    state = LayoutErrors::NoError;
    emit Start();

    if (bank->Prepare())
    {
        CheckDetailsSize();
        do
        {
            if (stopGeneration)
            {
                state = LayoutErrors::ProcessStoped;
                break;
            }

            VLayoutPaper paper(paperHeight, paperWidth);
            if (bank->LeftArrange() > 0)
            {
                const int index = bank->GetTiket();
                if (paper.ArrangeDetail(bank->GetDetail(index)))
                {
                    bank->Arranged(index);
                }
                else
                {
                    bank->NotArranged(index);
                }
            }
            else
            {
                if (paper.Count() > 0)
                {
                    papers.append(paper);
                }
                else
                {
                    state = LayoutErrors::EmptyPaperError;
                    emit Error(state);
                    return;
                }
            }

        } while (bank->AllDetailsCount() > 0);
    }
    else
    {
        state = LayoutErrors::PrepareLayoutError;
        emit Error(state);
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
LayoutErrors VLayoutGenerator::State() const
{
    return state;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::Abort()
{
    stopGeneration = true;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::CheckDetailsSize()
{
    const QRectF rec = bank->GetBiggestBoundingRect();
    if (rec.width() > paperWidth || rec.height() > paperHeight)
    {
        state = LayoutErrors::PaperSizeError;
        emit Error(state);
        stopGeneration = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutGenerator::GetPaperWidth() const
{
    return paperWidth;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetPaperWidth(int value)
{
    paperWidth = value;
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutGenerator::GetPaperHeight() const
{
    return paperHeight;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetPaperHeight(int value)
{
    paperHeight = value;
}

