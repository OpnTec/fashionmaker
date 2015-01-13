/************************************************************************
 **
 **  @file   vlayoutgenerator.h
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

#ifndef VLAYOUTGENERATOR_H
#define VLAYOUTGENERATOR_H

#include <QObject>
#include <QList>

#include "vlayoutdef.h"
#include "vbank.h"

class VLayoutPaper;
class VLayoutDetail;
class QGraphicsItem;

class VLayoutGenerator :public QObject
{
    Q_OBJECT
public:
    VLayoutGenerator(QObject *parent = 0);
    virtual ~VLayoutGenerator();

    void SetDetails(const QVector<VLayoutDetail> &details);
    void SetLayoutWidth(qreal width);
    void SetCaseType(Cases caseType);
    int DetailsCount();

    int GetPaperHeight() const;
    void SetPaperHeight(int value);

    int GetPaperWidth() const;
    void SetPaperWidth(int value);

    unsigned int GetShift() const;
    void         SetShift(unsigned int shift);

    void Generate();

    LayoutErrors State() const;

    QList<QGraphicsItem *> GetItems() const;

signals:
    void Start();
    void Arranged(int count);
    void Error(const LayoutErrors &state);
    void Finished();

public slots:
    void Abort();

private:
    Q_DISABLE_COPY(VLayoutGenerator)
    QVector<VLayoutPaper> papers;
    VBank *bank;
    int paperHeight;
    int paperWidth;
    bool stopGeneration;
    LayoutErrors state;
    unsigned int shift;

    void CheckDetailsSize();
};

#endif // VLAYOUTGENERATOR_H
