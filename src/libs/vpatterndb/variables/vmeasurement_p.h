/************************************************************************
 **
 **  @file   vmeasurement_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 8, 2014
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

#ifndef VMEASUREMENT_P_H
#define VMEASUREMENT_P_H

#include <QSharedData>

#include "vcontainer.h"

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VMeasurementData : public QSharedData
{
public:

    VMeasurementData(quint32 index, const QString &gui_text, const QString &tagName)
        :data(VContainer(nullptr, nullptr)), index(index), gui_text(gui_text), _tagName(tagName), formulaOk(true)
    {}

    VMeasurementData(VContainer *data, quint32 index, const QString &formula, bool ok, const QString &gui_text,
                     const QString &tagName)
        :data(*data), index(index), formula(formula), gui_text(gui_text), _tagName(tagName), formulaOk(ok)
    {}

    VMeasurementData(const VMeasurementData &m)
        :QSharedData(m), data(m.data), index(m.index), formula(m.formula), gui_text(m.gui_text), _tagName(m._tagName),
          formulaOk(m.formulaOk)
    {}

    virtual  ~VMeasurementData();

    VContainer data;
    quint32 index;
    QString formula;
    QString gui_text;
    QString _tagName;
    bool formulaOk;
};

VMeasurementData::~VMeasurementData()
{}

#ifdef Q_CC_GNU
#pragma GCC diagnostic pop
#endif

#endif // VMEASUREMENT_P_H
