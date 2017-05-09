/************************************************************************
 **
 **  @file   dialogflippingbyline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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
    explicit DialogFlippingByLine(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogFlippingByLine();

    quint32 GetFirstLinePointId() const;
    void    SetFirstLinePointId(quint32 value);

    quint32 GetSecondLinePointId() const;
    void    SetSecondLinePointId(quint32 value);

    QString GetSuffix() const;
    void    SetSuffix(const QString &value);

    QVector<quint32> GetObjects() const;

    virtual void ShowDialog(bool click) Q_DECL_OVERRIDE;

public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    virtual void SelectedObject(bool selected, quint32 object, quint32 tool) Q_DECL_OVERRIDE;

private slots:
    void SuffixChanged();

protected:
    virtual void CheckState() Q_DECL_FINAL;
    virtual void ShowVisualization() Q_DECL_OVERRIDE;

    /** @brief SaveData Put dialog data in local variables */
    virtual void SaveData() Q_DECL_OVERRIDE;

private slots:
    void PointChanged();

private:
    Q_DISABLE_COPY(DialogFlippingByLine)

    Ui::DialogFlippingByLine *ui;

    QList<quint32> objects;

    bool stage1;

    QString m_suffix;
};

#endif // DIALOGFLIPPINGBYLINE_H
