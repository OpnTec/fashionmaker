/************************************************************************
 **
 **  @file   vlayoutgenerator.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include <QElapsedTimer>
#include <QGraphicsRectItem>
#include <QRectF>
#include <QThreadPool>

#include "../vmisc/def.h"
#include "../vmisc/vmath.h"
#include "vlayoutpiece.h"
#include "vlayoutpaper.h"

//---------------------------------------------------------------------------------------------------------------------
VLayoutGenerator::VLayoutGenerator(QObject *parent)
    : QObject(parent),
      papers(),
      bank(new VBank()),
      paperHeight(0),
      paperWidth(0),
      margins(),
      usePrinterFields(true),
#ifdef Q_CC_MSVC
      // See https://stackoverflow.com/questions/15750917/initializing-stdatomic-bool
      stopGeneration(ATOMIC_VAR_INIT(false)),
#else
      stopGeneration(false),
#endif
      state(LayoutErrors::NoError),
      shift(0),
      rotate(true),
      followGrainline(false),
      rotationNumber(2),
      autoCrop(false),
      saveLength(false),
      unitePages(false),
      stripOptimizationEnabled(false),
      multiplier(1),
      stripOptimization(false),
      textAsPaths(false)
{}

//---------------------------------------------------------------------------------------------------------------------
VLayoutGenerator::~VLayoutGenerator()
{
    delete bank;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetDetails(const QVector<VLayoutPiece> &details)
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
void VLayoutGenerator::Generate(QElapsedTimer timer, qint64 timeout)
{
    stopGeneration.store(false);
    papers.clear();
    bank->Reset();
    state = LayoutErrors::NoError;

    if (VFuzzyComparePossibleNulls(shift, -1))
    {
        if (bank->PrepareDetails())
        {
            SetShift(bank->DetailsBiggestEdge() + 1);
        }
        else
        {
            state = LayoutErrors::PrepareLayoutError;
            return;
        }

        int width = PageWidth();
        int height = PageHeight();

        if (stripOptimization)
        {
            const qreal b = bank->GetBiggestDiagonal() * multiplier + bank->GetLayoutWidth();

            auto SetStrip = [this, b](int &side)
            {
                if (side >= b*2)
                {
                    stripOptimizationEnabled = true;
                    side = qFloor(side / qFloor(side/b));
                }
            };

            IsPortrait() ? SetStrip(height) : SetStrip(width);
        }
    }

    if (timer.hasExpired(timeout))
    {
        state = LayoutErrors::Timeout;
        return;
    }

#ifdef LAYOUT_DEBUG
    const QString path = QDir::homePath()+QStringLiteral("/LayoutDebug");
    QDir debugDir(path);
    debugDir.removeRecursively();
    debugDir.mkpath(path);
#endif

    if (bank->PrepareUnsorted())
    {
        if (timer.hasExpired(timeout))
        {
            state = LayoutErrors::Timeout;
            return;
        }

        while (bank->AllDetailsCount() > 0)
        {
            if (stopGeneration.load())
            {
                return;
            }

            if (timer.hasExpired(timeout))
            {
                state = LayoutErrors::Timeout;
                return;
            }

            VLayoutPaper paper(PageHeight(), PageWidth(), bank->GetLayoutWidth());
            paper.SetShift(shift);
            paper.SetPaperIndex(static_cast<quint32>(papers.count()));
            paper.SetRotate(rotate);
            paper.SetFollowGrainline(followGrainline);
            paper.SetRotationNumber(rotationNumber);
            paper.SetSaveLength(saveLength);
            do
            {
                const int index = bank->GetNext();
                if (paper.ArrangeDetail(bank->GetDetail(index), stopGeneration))
                {
                    bank->Arranged(index);
                }
                else
                {
                    bank->NotArranged(index);
                }

                QCoreApplication::processEvents();

                if (stopGeneration.load())
                {
                    break;
                }

                if (timer.hasExpired(timeout))
                {
                    state = LayoutErrors::Timeout;
                    return;
                }
            } while(bank->LeftToArrange() > 0);

            if (stopGeneration.load())
            {
                return;
            }

            if (timer.hasExpired(timeout))
            {
                state = LayoutErrors::Timeout;
                return;
            }

            if (paper.Count() > 0)
            {
                papers.append(paper);
            }
            else
            {
                state = LayoutErrors::EmptyPaperError;
                return;
            }
        }
    }
    else
    {
        state = LayoutErrors::PrepareLayoutError;
        return;
    }

    if (timer.hasExpired(timeout))
    {
        state = LayoutErrors::Timeout;
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
}

//---------------------------------------------------------------------------------------------------------------------
qreal VLayoutGenerator::LayoutEfficiency() const
{
    qreal efficiency = 0;
    if (not papers.isEmpty())
    {
        for(auto &paper : papers)
        {
            efficiency += paper.Efficiency();
        }

        efficiency /= papers.size();
    }
    return efficiency;
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
    for (auto &paper : papers)
    {
        list.append(paper.GetPaperItem(autoCrop, IsTestAsPaths()));
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QList<QGraphicsItem *> VLayoutGenerator::GetGlobalContours() const
{
    QList<QGraphicsItem *> list;
    for (auto &paper : papers)
    {
        list.append(paper.GetGlobalContour());
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QList<QList<QGraphicsItem *> > VLayoutGenerator::GetAllDetailsItems() const
{
    QList<QList<QGraphicsItem *> > list;
    for (auto &paper : papers)
    {
        list.append(paper.GetItemDetails(IsTestAsPaths()));
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QVector<VLayoutPiece> > VLayoutGenerator::GetAllDetails() const
{
    QVector<QVector<VLayoutPiece> > list;
    for (auto &paper : papers)
    {
        list.append(paper.GetDetails());
    }
    return list;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::Abort()
{
    stopGeneration.store(true);
    state = LayoutErrors::ProcessStoped;
    QThreadPool::globalInstance()->clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::Timeout()
{
    stopGeneration.store(true);
    state = LayoutErrors::Timeout;
    QThreadPool::globalInstance()->clear();
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
bool VLayoutGenerator::IsTestAsPaths() const
{
    return textAsPaths;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetTextAsPaths(bool value)
{
    textAsPaths = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutGenerator::IsRotationNeeded() const
{
    if (followGrainline)
    {
        return bank->IsRotationNeeded();
    }
    else
    {
        return true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
quint8 VLayoutGenerator::GetMultiplier() const
{
    return multiplier;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetMultiplier(quint8 value)
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
bool VLayoutGenerator::IsPortrait() const
{
    return PageHeight() >= PageWidth();
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::GatherPages()
{
    if (papers.size() < 2)
    {
        return;
    }

    QList<QList<VLayoutPiece>> nDetails;
    qreal length = 0;
    int j = 0; // papers count

    for (int i = 0; i < papers.size(); ++i)
    {
        if (IsPortrait())
        {
            int paperHeight = qRound(papers.at(i).DetailsBoundingRect().height());

            if (i != papers.size()-1)
            {
                paperHeight += qRound(bank->GetLayoutWidth()*2);
            }

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
        else
        {
            int paperWidth = qRound(papers.at(i).DetailsBoundingRect().width());

            if (i != papers.size()-1)
            {
                paperWidth += qRound(bank->GetLayoutWidth()*2);
            }

            if (length + paperWidth <= PageWidth())
            {
                UniteDetails(j, nDetails, length, i);
                length += paperWidth;
            }
            else
            {
                length = 0; // Start new paper
                ++j;// New paper
                UniteDetails(j, nDetails, length, i);
                length += paperWidth;
            }
        }
    }

    QVector<VLayoutPaper> nPapers;
    for (int i = 0; i < nDetails.size(); ++i)
    {
        VLayoutPaper paper(PageHeight(), PageWidth(), bank->GetLayoutWidth());
        paper.SetShift(shift);
        paper.SetPaperIndex(static_cast<quint32>(i));
        paper.SetRotate(rotate);
        paper.SetFollowGrainline(followGrainline);
        paper.SetRotationNumber(rotationNumber);
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
    QList<QList<VLayoutPiece> > nDetails;
    qreal length = 0;
    int j = 0; // papers count

    for (int i = 0; i < papers.size(); ++i)
    {
        if (IsPortrait())
        {
            int paperHeight = 0;
            if (autoCrop)
            {
                paperHeight = qCeil(papers.at(i).DetailsBoundingRect().height());
            }
            else
            {
                paperHeight = papers.at(i).GetHeight();
            }

            if (i != papers.size()-1)
            {
                paperHeight = qRound(paperHeight + bank->GetLayoutWidth()*2);
            }

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
        else
        {
            int paperWidth = 0;
            if (autoCrop)
            {
                paperWidth = qCeil(papers.at(i).DetailsBoundingRect().width());
            }
            else
            {
                paperWidth = papers.at(i).GetWidth();
            }

            if (i != papers.size()-1)
            {
                paperWidth = qRound(paperWidth + bank->GetLayoutWidth()*2);
            }

            if (length + paperWidth <= QIMAGE_MAX)
            {
                UniteDetails(j, nDetails, length, i);
                length += paperWidth;
                UnitePapers(j, papersLength, length);
            }
            else
            {
                length = 0; // Start new paper
                ++j;// New paper
                UniteDetails(j, nDetails, length, i);
                length += paperWidth;
                UnitePapers(j, papersLength, length);
            }
        }
    }

    QVector<VLayoutPaper> nPapers;
    for (int i = 0; i < nDetails.size(); ++i)
    {
        const int height = IsPortrait() ? qCeil(papersLength.at(i)) : PageHeight();
        const int width = IsPortrait() ? PageWidth() : qCeil(papersLength.at(i));

        VLayoutPaper paper(height, width, bank->GetLayoutWidth());
        paper.SetShift(shift);
        paper.SetPaperIndex(static_cast<quint32>(i));
        paper.SetRotate(rotate);
        paper.SetFollowGrainline(followGrainline);
        paper.SetRotationNumber(rotationNumber);
        paper.SetSaveLength(saveLength);
        paper.SetDetails(nDetails.at(i));

        nPapers.append(paper);
    }

    papers.clear();
    papers = nPapers;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::UniteDetails(int j, QList<QList<VLayoutPiece> > &nDetails, qreal length, int i)
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
QList<VLayoutPiece> VLayoutGenerator::MoveDetails(qreal length, const QVector<VLayoutPiece> &details)
{
    if (qFuzzyIsNull(length))
    {
        return details.toList();
    }

    QList<VLayoutPiece> newDetails;
    for (auto d : details)
    {
        IsPortrait() ? d.Translate(0, length) : d.Translate(length, 0);
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
int VLayoutGenerator::GetRotationNumber() const
{
    return rotationNumber;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetRotationNumber(int value)
{
    rotationNumber = value;

    if (rotationNumber > 360 || rotationNumber < 1)
    {
        rotationNumber = 2;
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
bool VLayoutGenerator::GetFollowGrainline() const
{
    return followGrainline;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetFollowGrainline(bool value)
{
    followGrainline = value;
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
int VLayoutGenerator::GetNestingTime() const
{
    return nestingTime;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetNestingTime(int value)
{
    nestingTime = value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VLayoutGenerator::GetEfficiencyCoefficient() const
{
    return efficiencyCoefficient;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetEfficiencyCoefficient(qreal coefficient)
{
    efficiencyCoefficient = coefficient;
}

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutGenerator::IsUsePrinterFields() const
{
    return usePrinterFields;
}

//---------------------------------------------------------------------------------------------------------------------
QMarginsF VLayoutGenerator::GetPrinterFields() const
{
    return margins;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetPrinterFields(bool usePrinterFields, const QMarginsF &value)
{
    this->usePrinterFields = usePrinterFields;
    margins = value;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VLayoutGenerator::GetShift() const
{
    return shift;
}

//---------------------------------------------------------------------------------------------------------------------
void VLayoutGenerator::SetShift(qreal shift)
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
