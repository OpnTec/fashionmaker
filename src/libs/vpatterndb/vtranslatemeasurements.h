/************************************************************************
 **
 **  @file   vtranslatemeasurements.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 8, 2015
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

#ifndef VTRANSLATEMEASUREMENTS_H
#define VTRANSLATEMEASUREMENTS_H

#include <QMap>
#include "../qmuparser/qmutranslation.h"

class VTranslateMeasurements
{
public:
    VTranslateMeasurements();
    virtual ~VTranslateMeasurements();

    bool MeasurementsFromUser(QString &newFormula, int position, const QString &token, int &bias) const;

    QString MToUser(const QString &measurement) const;
    QString MNumber(const QString &measurement) const;
    QString MFormula(const QString &measurement) const;
    QString GuiText(const QString &measurement) const;
    QString Description(const QString &measurement) const;

    virtual void Retranslate();

protected:
    QMap<QString, qmu::QmuTranslation> measurements;

private:
    Q_DISABLE_COPY(VTranslateMeasurements)
    QMap<QString, qmu::QmuTranslation> guiTexts;
    QMap<QString, qmu::QmuTranslation> descriptions;
    QMap<QString, QString> numbers;
    QMap<QString, QString> formulas;

    void InitGroupA(); // Direct Height
    void InitGroupB(); // Direct Width
    void InitGroupC(); // Indentation
    void InitGroupD(); // Circumference and Arc
    void InitGroupE(); // Vertical
    void InitGroupF(); // Horizontal
    void InitGroupG(); // Bust
    void InitGroupH(); // Balance
    void InitGroupI(); // Arm
    void InitGroupJ(); // Leg
    void InitGroupK(); // Crotch and Rise
    void InitGroupL(); // Hand
    void InitGroupM(); // Foot
    void InitGroupN(); // Head
    void InitGroupO(); // Men & Tailoring
    void InitGroupP(); // Historical & Specialty
    void InitGroupQ(); // Patternmaking measurements

    void InitMeasurements();

    void InitMeasurement(const QString &name, const qmu::QmuTranslation &m, const qmu::QmuTranslation &g,
                         const qmu::QmuTranslation &d, const QString &number, const QString &formula = QString());
};

#endif // VTRANSLATEMEASUREMENTS_H
