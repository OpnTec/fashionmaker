/************************************************************************
 **
 **  @file   vmeasurements.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 7, 2015
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

#ifndef VMEASUREMENTS_H
#define VMEASUREMENTS_H

#include "../ifc/xml/vdomdocument.h"
#include "../vpatterndb/vcontainer.h"

enum class SexType : char { Male, Female, Unknown };

class VMeasurements : public VDomDocument
{

public:
    VMeasurements(VContainer *data);
    VMeasurements(Unit unit, VContainer *data);
    VMeasurements(Unit unit, int baseSize, int baseHeight, VContainer *data);
    virtual ~VMeasurements() Q_DECL_OVERRIDE;

    MeasurementsType Type() const;
    Unit MUnit() const;
    int BaseSize() const;
    int BaseHeight() const;

    QString Notes() const;
    void    SetNotes(const QString &text);

    QString FamilyName() const;
    void    SetFamilyName(const QString &text);

    QString GivenName() const;
    void    SetGivenName(const QString &text);

    QDate   BirthDate() const;
    void    SetBirthDate(const QDate &date);

    SexType Sex() const;
    void    SetSex(const SexType &sex);

    QString Email() const;
    void    SetEmail(const QString &text);

    static const QString TagVST;
    static const QString TagVIT;
    static const QString TagBodyMeasurements;
    static const QString TagUnit;
    static const QString TagNotes;
    static const QString TagSize;
    static const QString TagHeight;
    static const QString TagPersonal;
    static const QString TagFamilyName;
    static const QString TagGivenName;
    static const QString TagBirthDate;
    static const QString TagSex;
    static const QString TagEmail;

    static const QString AttrBase;

    static const QString SexMale;
    static const QString SexFemale;
    static const QString SexUnknown;

    static QString GenderToStr(const SexType &sex);
    static SexType StrToGender(const QString &sex);

private:
    Q_DISABLE_COPY(VMeasurements)

    /** @brief data container with data. */
    VContainer     *data;
    MeasurementsType type;

    void CreateEmptyStandardFile(Unit unit, int baseSize, int baseHeight);
    void CreateEmptyIndividualFile(Unit unit);

    qreal UniqueTagAttr(const QString &tag, const QString &attr, qreal defValue) const;
};

#endif // VMEASUREMENTS_H
