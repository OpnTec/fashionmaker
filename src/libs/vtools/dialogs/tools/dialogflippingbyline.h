/************************************************************************
 **
 **  @file   dialogflippingbyline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#ifndef DIALOGFLIPPINGBYLINE_H
#define DIALOGFLIPPINGBYLINE_H

#include "dialogtool.h"

#include <qcompilerdetection.h>
#include <QList>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtGlobal>

#include "../vmisc/def.h"

namespace Ui
{
    class DialogFlippingByLine;
}

class DialogFlippingByLine : public DialogTool
{
    Q_OBJECT

public:
    explicit DialogFlippingByLine(const VContainer *data, quint32 toolId, QWidget *parent = nullptr);
    virtual ~DialogFlippingByLine();

    quint32 GetFirstLinePointId() const;
    void    SetFirstLinePointId(quint32 value);

    quint32 GetSecondLinePointId() const;
    void    SetSecondLinePointId(quint32 value);

    QString GetSuffix() const;
    void    SetSuffix(const QString &value);

    QVector<quint32> GetObjects() const;

    QString GetVisibilityGroupName() const;
    void    SetVisibilityGroupName(const QString &name);

    bool HasLinkedVisibilityGroup() const;
    void SetHasLinkedVisibilityGroup(bool linked);

    void        SetVisibilityGroupTags(const QStringList &tags);
    QStringList GetVisibilityGroupTags() const;

    virtual void SetGroupCategories(const QStringList &categories) override;

    virtual void ShowDialog(bool click) override;

public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) override;
    virtual void SelectedObject(bool selected, quint32 object, quint32 tool) override;

private slots:
    void SuffixChanged();
    void GroupNameChanged();

protected:
    virtual void ShowVisualization() override;

    /** @brief SaveData Put dialog data in local variables */
    virtual void SaveData() override;
    virtual bool IsValid() const final;

private slots:
    void PointChanged();

private:
    Q_DISABLE_COPY(DialogFlippingByLine)

    Ui::DialogFlippingByLine *ui;

    QList<quint32> objects;

    bool stage1;

    QString m_suffix;

    bool flagName;
    bool flagGroupName;
    bool flagError;

    QStringList m_groupTags{};
};

//---------------------------------------------------------------------------------------------------------------------
inline bool DialogFlippingByLine::IsValid() const
{
    return flagError && flagName && flagGroupName;
}

#endif // DIALOGFLIPPINGBYLINE_H
