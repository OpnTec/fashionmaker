/************************************************************************
 **
 **  @file   dialogtruedarts.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 6, 2015
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

#ifndef DIALOGTRUEDARTS_H
#define DIALOGTRUEDARTS_H

#include "dialogtool.h"

namespace Ui
{
    class DialogTrueDarts;
}

class DialogTrueDarts : public DialogTool
{
    Q_OBJECT

public:
    DialogTrueDarts(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    ~DialogTrueDarts();

    QString            GetFirstNewDartPointName();
    QString            GetSecondNewDartPointName();
    void               SetNewDartPointNames(const QString &firstPoint, const QString &secondPoint);

    quint32            GetFirstBasePointId() const;
    void               SetFirstBasePointId(const quint32 &value);

    quint32            GetSecondBasePointId() const;
    void               SetSecondBasePointId(const quint32 &value);

    quint32            GetFirstDartPointId() const;
    void               SetFirstDartPointId(const quint32 &value);

    quint32            GetSecondDartPointId() const;
    void               SetSecondDartPointId(const quint32 &value);

    quint32            GetThirdDartPointId() const;
    void               SetThirdDartPointId(const quint32 &value);

    void               SetChildrenId(const quint32 &ch1, const quint32 &ch2);
public slots:
    virtual void       ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    virtual void       PointNameChanged() Q_DECL_OVERRIDE;
    void               NameDartPoint1Changed();
    void               NameDartPoint2Changed();
protected:
    virtual void       ShowVisualization() Q_DECL_OVERRIDE;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void       SaveData() Q_DECL_OVERRIDE;
    virtual void       CheckState() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(DialogTrueDarts)
    Ui::DialogTrueDarts *ui;

    QString d1PointName;
    QString d2PointName;

    quint32 ch1;
    quint32 ch2;

    bool flagName1;
    bool flagName2;

    void NameChanged(QLabel *labelEditNamePoint, const QString &pointD1Name, const QString &pointD2Name,
                     QLineEdit *secondPointName, bool &flagName);

    void FillComboBoxs(const quint32 &ch1, const quint32 &ch2);

    void CheckName(QLineEdit* edit, QLabel *labelEditNamePoint, const QString &pointD1Name, const QString &pointD2Name,
                   QLineEdit *secondPointName, bool &flagName);
};

#endif // DIALOGTRUEDARTS_H
