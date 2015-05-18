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
 **  Copyright (C) 2013-2015 Valentina project
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
#include <QImage>
#include <QDir>
#include <QGraphicsItem>
#include <QThreadPool>

//---------------------------------------------------------------------------------------------------------------------
VLayoutGenerator::VLayoutGenerator(QObject *parent)
    :QObject(parent), papers(QVector<VLayoutPaper>()), bank(new VBank()), paperHeight(0), paperWidth(0),
      stopGeneration(false), state(LayoutErrors::NoError), shift(0), rotate(true), rotationIncrease(180),
      autoCrop(false), saveLength(false), unitePages(false)
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
// cppcheck-suppress unusedFunction
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

#ifdef LAYOUT_DEBUG
    const QString path = QDir::homePath()+QStringLiteral("/LayoutDebug");
    QDir debugDir(path);
    debugDir.removeRecursively();
    debugDir.mkpath(path);
#endif

    emit Start();

    if (bank->Prepare())
    {
        CheckDetailsSize();
        while (bank->AllDetailsCount() > 0)
        {
            if (stopGeneration)
            {
                break;
            }

            VLayoutPaper paper(paperHeight, paperWidth);
            paper.SetShift(shift);
            paper.SetLayoutWidth(bank->GetLayoutWidth());
            paper.SetPaperIndex(static_cast<quint32>(papers.count()));
            paper.SetRotate(rotate);
            paper.SetRotationIncrease(rotationIncrease);
            paper.SetSaveLength(saveLength);
            do
            {
                const int index = bank->GetTiket();
                if (paper.ArrangeDetail(bank->GetDetail(index), stopGeneration))
                {
                    bank->Arranged(index);
                    emit Arranged(bank->ArrangedCount());
                }
                else
                {
                    bank->NotArranged(index);
                }

                if (stopGeneration)
                {
                    break;
                }
            } while(bank->LeftArrange() > 0);

            if (stopGeneration)
            {
                break;
            }

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
    }
    else
    {
        state = LayoutErrors::PrepareLayoutError;
        emit Error(state);
        return;
    }
    emit Finished();
}

//---------------------------------------------------------------------------------------------------------------------
LayoutErrors VLayoutGenerator::State() const
{
    return state;
}

//---------------------------------------------------------------------------------------------------------------------
QList<QGraphicsItem *> VLayoutGenerator::GetPapersItems() const
{
    QList<QGraphicsItem *> list;
    for (int i=0; i < papers.count(); ++i)
    {
        list.append(papers.at(i).GetPaperItem(autoCrop));
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QList<QList<QGraphicsItem *> > VLayoutGenerator::GetAllDetails() const
{
    QList<QList<QGraphicsItem *> > list;
    for (int i=0; i < papers.count(); ++i)
    {
        list.append(papers.at(i).GetDetails());
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::Abort()
{
    stopGeneration = true;
    state = LayoutErrors::ProcessStoped;
#if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    QThreadPool::globalInstance()->clear();
#endif
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutGenerator::IsUnitePages() const
{
    return unitePages;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetUnitePages(bool value)
{
    unitePages = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutGenerator::IsSaveLength() const
{
    return saveLength;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetSaveLength(bool value)
{
    saveLength = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutGenerator::GetAutoCrop() const
{
    return autoCrop;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetAutoCrop(bool value)
{
    autoCrop = value;
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
int VLayoutGenerator::GetRotationIncrease() const
{
    return rotationIncrease;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetRotationIncrease(int value)
{
    rotationIncrease = value;

    if ((rotationIncrease >= 1 && rotationIncrease <= 180 && 360 % rotationIncrease == 0) == false)
    {
        rotationIncrease = 180;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutGenerator::GetRotate() const
{
    return rotate;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetRotate(bool value)
{
    rotate = value;
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
unsigned int VLayoutGenerator::GetShift() const
{
    return shift;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetShift(unsigned int shift)
{
    this->shift = shift;
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
