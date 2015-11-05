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

#ifndef VLAYOUTGENERATOR_H
#define VLAYOUTGENERATOR_H

#include <QObject>
#include <QList>

#include "vlayoutdef.h"
#include "vbank.h"
#include <memory>

class VLayoutPaper;
class VLayoutDetail;
class QGraphicsItem;

class VLayoutGenerator :public QObject
{
    Q_OBJECT
public:
    explicit VLayoutGenerator(QObject *parent = 0);
    virtual ~VLayoutGenerator() Q_DECL_OVERRIDE;

    void SetDetails(const QVector<VLayoutDetail> &details);
    void SetLayoutWidth(qreal width);
    void SetCaseType(Cases caseType);
    int DetailsCount();

    int GetPaperHeight() const;
    void SetPaperHeight(int value);

    int GetPaperWidth() const;
    void SetPaperWidth(int value);

    quint32 GetShift() const;
    void    SetShift(quint32 shift);

    void Generate();

    LayoutErrors State() const;

    QList<QGraphicsItem *> GetPapersItems() const;
    QList<QList<QGraphicsItem *>> GetAllDetails() const;

    bool GetRotate() const;
    void SetRotate(bool value);

    int GetRotationIncrease() const;
    void SetRotationIncrease(int value);

    bool GetAutoCrop() const;
    void SetAutoCrop(bool value);

    bool IsSaveLength() const;
    void SetSaveLength(bool value);

    bool IsUnitePages() const;
    void SetUnitePages(bool value);

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
    volatile bool stopGeneration;
    LayoutErrors state;
    quint32 shift;
    bool rotate;
    int rotationIncrease;
    bool autoCrop;
    bool saveLength;
    bool unitePages;
};

typedef std::shared_ptr<VLayoutGenerator> VLayoutGeneratorPtr;

#endif // VLAYOUTGENERATOR_H
