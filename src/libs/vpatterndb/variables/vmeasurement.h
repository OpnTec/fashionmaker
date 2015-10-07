/************************************************************************
 **
 **  @file   vstandardtablecell.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef VSTANDARDTABLEROW_H
#define VSTANDARDTABLEROW_H

#include "vvariable.h"
#include "../ifc/ifcdef.h"

#include <QStringList>

class VMeasurementData;
class VContainer;

/**
 * @brief The VMeasurement class keep data row of standard table
 */
class VMeasurement :public VVariable
{
public:
    VMeasurement(quint32 index, const QString &name, qreal baseSize, qreal baseHeight, const qreal &base,
                 const qreal &ksize, const qreal &kheight, const QString &gui_text = QString(),
                 const QString &description = QString(), const QString &TagName = QString());
    VMeasurement(VContainer *data, quint32 index, const QString &name, const qreal &base, const QString &formula,
                 bool ok, const QString &gui_text = QString(), const QString &description = QString(),
                 const QString &TagName = QString());
    VMeasurement(const VMeasurement &m);
    VMeasurement &operator=(const VMeasurement &m);
    virtual ~VMeasurement() Q_DECL_OVERRIDE;

    QString GetGuiText() const;

    QString TagName() const;
    void    setTagName(const QString &TagName);

    QString GetFormula() const;

    bool    IsCustom() const;

    int     Index() const;
    bool    IsFormulaOk() const;

    VContainer *GetData();

    static QStringList ListHeights(QMap<GHeights, bool> heights, Unit patternUnit);
    static QStringList ListSizes(QMap<GSizes, bool> sizes, Unit patternUnit);
    static QStringList WholeListHeights(Unit patternUnit);
    static QStringList WholeListSizes(Unit patternUnit);
    static bool IsGradationSizeValid(const QString &size);
    static bool IsGradationHeightValid(const QString &height);
private:
    QSharedDataPointer<VMeasurementData> d;

    static void        ListValue(QStringList &list, qreal value, Unit patternUnit);
};

#endif // VSTANDARDTABLEROW_H
