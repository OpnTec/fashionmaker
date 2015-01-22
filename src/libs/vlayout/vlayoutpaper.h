/************************************************************************
 **
 **  @file   vlayoutpaper.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2015
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

#ifndef VLAYOUTPAPER_H
#define VLAYOUTPAPER_H

#include <QSharedDataPointer>
#include "vlayoutdef.h"

class VLayoutPaperData;
class VLayoutDetail;
class QGraphicsItem;
class VBestSquare;

class VLayoutPaper
{
public:
    VLayoutPaper();
    VLayoutPaper(int height, int width);
    VLayoutPaper(const VLayoutPaper &paper);
    VLayoutPaper &operator=(const VLayoutPaper &paper);
    virtual ~VLayoutPaper();

    int  GetHeight() const;
    void SetHeight(int height);

    int  GetWidth() const;
    void SetWidth(int width);

    qreal GetLayoutWidth() const;
    void  SetLayoutWidth(qreal width);

    unsigned int GetShift() const;
    void         SetShift(unsigned int shift);

    bool GetRotate() const;
    void SetRotate(bool value);

    int GetRotationIncrease() const;
    void SetRotationIncrease(int value);

    void SetPaperIndex(quint32 index);

    bool ArrangeDetail(const VLayoutDetail &detail, bool &stop);
    int  Count() const;
    QGraphicsItem *GetItem() const;

private:
    QSharedDataPointer<VLayoutPaperData> d;

    bool AddToSheet(const VLayoutDetail &detail, bool &stop);

    bool SaveResult(const VBestSquare &bestResult, const VLayoutDetail &detail);
    void SaveCandidate(VBestSquare &bestResult, const VLayoutDetail &detail, int globalI, int detJ, BestFrom type);

};

#endif // VLAYOUTPAPER_H
