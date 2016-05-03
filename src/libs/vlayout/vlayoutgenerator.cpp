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
#include "../vmisc/def.h"

#include <QRectF>
#include <QImage>
#include <QDir>
#include <QGraphicsItem>
#include <QThreadPool>
#include <QtCore/qmath.h>

//---------------------------------------------------------------------------------------------------------------------
VLayoutGenerator::VLayoutGenerator(QObject *parent)
    :QObject(parent), papers(QVector<VLayoutPaper>()), bank(new VBank()), paperHeight(0), paperWidth(0), margins(),
      stopGeneration(false), state(LayoutErrors::NoError), shift(0), rotate(true), rotationIncrease(180),
      autoCrop(false), saveLength(false), unitePages(false), stripOptimizationEnabled(false), multiplier(1),
      stripOptimization(false)
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
        const int width = PageWidth();
        int height = PageHeight();

        if (stripOptimization)
        {
            const qreal b = bank->GetBiggestDiagonal() * multiplier + bank->GetLayoutWidth();

            if (height >= b*2)
            {
                stripOptimizationEnabled = true;
                height = qFloor(height / qFloor(height/b));
            }
        }

        while (bank->AllDetailsCount() > 0)
        {
            if (stopGeneration)
            {
                break;
            }

            VLayoutPaper paper(height, width);
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

    if (stripOptimizationEnabled)
    {
        GatherPages();
    }

    if (IsUnitePages())
    {
        UnitePages();
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
        list.append(papers.at(i).GetItemDetails());
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
bool VLayoutGenerator::IsStripOptimization() const
{
    return stripOptimization;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetStripOptimization(bool value)
{
    stripOptimization = value;
}

//---------------------------------------------------------------------------------------------------------------------
quint8 VLayoutGenerator::GetMultiplier() const
{
    return multiplier;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetMultiplier(const quint8 &value)
{
    if (value > 10)
    {
        multiplier = 10;
    }
    else if (value == 0)
    {
        multiplier = 1;
    }
    else
    {
        multiplier = value;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutGenerator::PageHeight() const
{
    return static_cast<int>(paperHeight - (margins.top() + margins.bottom()));
}

//---------------------------------------------------------------------------------------------------------------------
int VLayoutGenerator::PageWidth() const
{
    return static_cast<int>(paperWidth - (margins.left() + margins.right()));
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::GatherPages()
{
    if (papers.size() < 2)
    {
        return;
    }

    QList<QList<VLayoutDetail>> nDetails;
    qreal length = 0;
    int j = 0; // papers count

    for (int i = 0; i < papers.size(); ++i)
    {
        const int paperHeight = qRound(papers.at(i).BoundingRect().height());
        if (length + paperHeight <= PageHeight())
        {
            UniteDetails(j, nDetails, length, i);
            length += paperHeight;
        }
        else
        {
            length = 0; // Start new paper
            ++j;// New paper
            UniteDetails(j, nDetails, length, i);
            length += paperHeight;
        }
    }

    QVector<VLayoutPaper> nPapers;
    for (int i = 0; i < nDetails.size(); ++i)
    {
        VLayoutPaper paper(PageHeight(), PageWidth());
        paper.SetShift(shift);
        paper.SetLayoutWidth(bank->GetLayoutWidth());
        paper.SetPaperIndex(static_cast<quint32>(i));
        paper.SetRotate(rotate);
        paper.SetRotationIncrease(rotationIncrease);
        paper.SetSaveLength(saveLength);
        paper.SetDetails(nDetails.at(i));

        nPapers.append(paper);
    }

    papers.clear();
    papers = nPapers;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::UnitePages()
{
    if (papers.size() < 2)
    {
        return;
    }

    QList<qreal> papersLength;
    QList<QList<VLayoutDetail> > nDetails;
    qreal length = 0;
    int j = 0; // papers count

    for (int i = 0; i < papers.size(); ++i)
    {
        int paperHeight = 0;
        if (autoCrop)
        {
            paperHeight = qRound(papers.at(i).BoundingRect().height());
        }
        else
        {
            paperHeight = papers.at(i).GetHeight();
        }

        paperHeight = qRound(paperHeight + bank->GetLayoutWidth());

        if (length + paperHeight <= QIMAGE_MAX)
        {
            UniteDetails(j, nDetails, length, i);
            length += paperHeight;
            UnitePapers(j, papersLength, length);
        }
        else
        {
            length = 0; // Start new paper
            ++j;// New paper
            UniteDetails(j, nDetails, length, i);
            length += paperHeight;
            UnitePapers(j, papersLength, length);
        }
    }

    QVector<VLayoutPaper> nPapers;
    for (int i = 0; i < nDetails.size(); ++i)
    {
        VLayoutPaper paper(qFloor(papersLength.at(i)), PageWidth());
        paper.SetShift(shift);
        paper.SetLayoutWidth(bank->GetLayoutWidth());
        paper.SetPaperIndex(static_cast<quint32>(i));
        paper.SetRotate(rotate);
        paper.SetRotationIncrease(rotationIncrease);
        paper.SetSaveLength(saveLength);
        paper.SetDetails(nDetails.at(i));

        nPapers.append(paper);
    }

    papers.clear();
    papers = nPapers;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::UniteDetails(int j, QList<QList<VLayoutDetail> > &nDetails, qreal length, int i)
{
    if ((j == 0 && nDetails.isEmpty()) || j >= nDetails.size())
    {//First or new details in paper
        nDetails.insert(j, MoveDetails(length, papers.at(i).GetDetails()));
    }
    else
    {
        nDetails[j].append(MoveDetails(length, papers.at(i).GetDetails()));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::UnitePapers(int j, QList<qreal> &papersLength, qreal length)
{
    if ((j == 0 && papersLength.isEmpty()) || j >= papersLength.size())
    {
        papersLength.insert(j, length);
    }
    else
    {
        papersLength[j] = length;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QList<VLayoutDetail> VLayoutGenerator::MoveDetails(qreal length, const QVector<VLayoutDetail> &details)
{
    if (qFuzzyIsNull(length))
    {
        return details.toList();
    }

    QList<VLayoutDetail> newDetails;
    for (int i = 0; i < details.size(); ++i)
    {
        VLayoutDetail d = details.at(i);
        d.Translate(0, length);
        newDetails.append(d);
    }

    return newDetails;
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
qreal VLayoutGenerator::GetPaperWidth() const
{
    return paperWidth;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetPaperWidth(qreal value)
{
    paperWidth = value;
}

//---------------------------------------------------------------------------------------------------------------------
QMarginsF VLayoutGenerator::GetFields() const
{
    return margins;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetFields(const QMarginsF &value)
{
    margins = value;
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VLayoutGenerator::GetShift() const
{
    return shift;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetShift(quint32 shift)
{
    this->shift = shift;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VLayoutGenerator::GetPaperHeight() const
{
    return paperHeight;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetPaperHeight(qreal value)
{
    paperHeight = value;
}
