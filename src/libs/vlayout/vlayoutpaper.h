/************************************************************************
 **
 **  @file   vlayoutpaper.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2015
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

#ifndef VLAYOUTPAPER_H
#define VLAYOUTPAPER_H

#include <qcompilerdetection.h>
#include <QSharedDataPointer>
#include <QTypeInfo>
#include <QtGlobal>
#include <atomic>
#include <QGraphicsPathItem>

#include "vlayoutdef.h"

class VBestSquare;
class VLayoutPaperData;
class VLayoutPiece;
class QGraphicsRectItem;
class QRectF;
class QGraphicsItem;
template <typename T> class QList;
template <typename T> class QVector;

class VLayoutPaper
{
public:
    VLayoutPaper();
    VLayoutPaper(int height, int width, qreal layoutWidth);
    VLayoutPaper(const VLayoutPaper &paper);

    ~VLayoutPaper();

    VLayoutPaper &operator=(const VLayoutPaper &paper);
#ifdef Q_COMPILER_RVALUE_REFS
    VLayoutPaper(const VLayoutPaper &&paper) Q_DECL_NOTHROW;
    VLayoutPaper &operator=(VLayoutPaper &&paper) Q_DECL_NOTHROW;
#endif

    int  GetHeight() const;
    void SetHeight(int height);

    int  GetWidth() const;
    void SetWidth(int width);

    qreal GetLayoutWidth() const;
    void  SetLayoutWidth(qreal width);

    qreal GetShift() const;
    void    SetShift(qreal shift);

    bool GetRotate() const;
    void SetRotate(bool value);

    bool GetFollowGrainline() const;
    void SetFollowGrainline(bool value);

    int GetRotationNumber() const;
    void SetRotationNumber(int value);

    bool IsSaveLength() const;
    void SetSaveLength(bool value);

    void SetPaperIndex(quint32 index);

    bool IsOriginPaperPortrait() const;
    void SetOriginPaperPortrait(bool portrait);

    bool ArrangeDetail(const VLayoutPiece &detail, std::atomic_bool &stop);
    int  Count() const;
    Q_REQUIRED_RESULT QGraphicsRectItem *GetPaperItem(bool autoCropLength, bool autoCropWidth, bool textAsPaths) const;
    Q_REQUIRED_RESULT QGraphicsPathItem *GetGlobalContour() const;
    Q_REQUIRED_RESULT QList<QGraphicsItem *> GetItemDetails(bool textAsPaths) const;

    QVector<VLayoutPiece> GetDetails() const;
    void                  SetDetails(const QList<VLayoutPiece>& details);

    QRectF DetailsBoundingRect() const;

    qreal Efficiency() const;

private:
    QSharedDataPointer<VLayoutPaperData> d;

    bool SaveResult(const VBestSquare &bestResult, const VLayoutPiece &detail);

};

Q_DECLARE_TYPEINFO(VLayoutPaper, Q_MOVABLE_TYPE);

#endif // VLAYOUTPAPER_H
