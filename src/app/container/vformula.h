/************************************************************************
 **
 **  @file   vformula.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef VFORMULA_H
#define VFORMULA_H

#include <QCoreApplication>

enum class FormulaType : char{ToUser, FromUser};

class VContainer;

class VFormula
{
    Q_DECLARE_TR_FUNCTIONS(VFormula)
public:
    VFormula();
    VFormula(const QString &formula, const VContainer *container);
    VFormula &operator=(const VFormula &formula);
    VFormula(const VFormula &formula);
    bool operator==(const VFormula &formula) const;
    bool operator!=(const VFormula &formula) const;

    QString getFormula(FormulaType type = FormulaType::ToUser) const;
    void setFormula(const QString &value, FormulaType type = FormulaType::ToUser);

    QString getStringValue() const;
    qreal   getDoubleValue() const;

    bool getCheckZero() const;
    void setCheckZero(bool value);

    const VContainer *getData() const;
    void setData(const VContainer *value);

    quint32 getToolId() const;
    void setToolId(const quint32 &value);

    QString getPostfix() const;
    void setPostfix(const QString &value);

    bool error() const;

    static int FormulaTypeId();
private:
    QString formula;
    QString value;
    bool checkZero;
    const VContainer *data;
    quint32 toolId;
    QString postfix;
    bool _error;
    qreal dValue;

    void Eval();
};
Q_DECLARE_METATYPE(VFormula)

#endif // VFORMULA_H
