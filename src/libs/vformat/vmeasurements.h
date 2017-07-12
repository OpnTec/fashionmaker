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

#include <qcompilerdetection.h>
#include <QCoreApplication>
#include <QDomElement>
#include <QString>
#include <QStringList>
#include <QtGlobal>

#include "../ifc/xml/vdomdocument.h"
#include "../vmisc/def.h"
#include "../vpatterndb/vcontainer.h"

enum class GenderType : char { Male, Female, Unknown };

class VMeasurements : public VDomDocument
{
    Q_DECLARE_TR_FUNCTIONS(VMeasurements)
public:
    explicit VMeasurements(VContainer *data);
    VMeasurements(Unit unit, VContainer *data);
    VMeasurements(Unit unit, int baseSize, int baseHeight, VContainer *data);
    virtual ~VMeasurements() Q_DECL_EQ_DEFAULT;

    virtual void setXMLContent(const QString &fileName) Q_DECL_OVERRIDE;
    virtual bool SaveDocument(const QString &fileName, QString &error) Q_DECL_OVERRIDE;

    void AddEmpty(const QString &name, const QString &formula = QString());
    void AddEmptyAfter(const QString &after, const QString &name, const QString &formula = QString());
    void Remove(const QString &name);
    void MoveTop(const QString &name);
    void MoveUp(const QString &name);
    void MoveDown(const QString &name);
    void MoveBottom(const QString &name);

    void ReadMeasurements() const;
    void ClearForExport();

    MeasurementsType Type() const;
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

    GenderType Gender() const;
    void    SetGender(const GenderType &gender);

    QString PMSystem() const;
    void    SetPMSystem(const QString &system);

    QString Email() const;
    void    SetEmail(const QString &text);

    bool    IsReadOnly() const;
    void    SetReadOnly(bool ro);

    void SetMName(const QString &name, const QString &text);
    void SetMValue(const QString &name, const QString &text);
    void SetMBaseValue(const QString &name, double value);
    void SetMSizeIncrease(const QString &name, double value);
    void SetMHeightIncrease(const QString &name, double value);
    void SetMDescription(const QString &name, const QString &text);
    void SetMFullName(const QString &name, const QString &text);

    static const QString TagVST;
    static const QString TagVIT;
    static const QString TagBodyMeasurements;
    static const QString TagNotes;
    static const QString TagSize;
    static const QString TagHeight;
    static const QString TagPersonal;
    static const QString TagFamilyName;
    static const QString TagGivenName;
    static const QString TagBirthDate;
    static const QString TagGender;
    static const QString TagPMSystem;
    static const QString TagEmail;
    static const QString TagReadOnly;
    static const QString TagMeasurement;

    static const QString AttrBase;
    static const QString AttrValue;
    static const QString AttrSizeIncrease;
    static const QString AttrHeightIncrease;
    static const QString AttrDescription;
    static const QString AttrName;
    static const QString AttrFullName;

    static const QString GenderMale;
    static const QString GenderFemale;
    static const QString GenderUnknown;

    static QString GenderToStr(const GenderType &sex);
    static GenderType StrToGender(const QString &sex);

    QStringList ListAll() const;
    QStringList ListKnown() const;

    bool IsDefinedKnownNamesValid() const;

    void SetDataSize();
    void SetDataHeight();

private:
    Q_DISABLE_COPY(VMeasurements)

    /** @brief data container with data. */
    VContainer     *data;
    MeasurementsType type;

    void CreateEmptyMultisizeFile(Unit unit, int baseSize, int baseHeight);
    void CreateEmptyIndividualFile(Unit unit);

    qreal UniqueTagAttr(const QString &tag, const QString &attr, qreal defValue) const;

    QDomElement MakeEmpty(const QString &name, const QString &formula);
    QDomElement FindM(const QString &name) const;
    MeasurementsType ReadType() const;

    qreal EvalFormula(VContainer *data, const QString &formula, bool *ok) const;

    QString ClearPMCode(const QString &code) const;
};

#endif // VMEASUREMENTS_H
