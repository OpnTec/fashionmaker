/************************************************************************
 **
 **  @file   dialogline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef DIALOGLINE_H
#define DIALOGLINE_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui
{
    class DialogLine;
}

/**
 * @brief The DialogLine class dialog for ToolLine. Help create line and edit option.
 */
class DialogLine : public DialogTool
{
    Q_OBJECT
public:
    DialogLine(const VContainer *data, quint32 toolId, QWidget *parent = nullptr);
    virtual ~DialogLine() override;

    quint32        GetFirstPoint() const;
    void           SetFirstPoint(quint32 value);

    quint32        GetSecondPoint() const;
    void           SetSecondPoint(quint32 value);

    QString        GetTypeLine() const;
    void           SetTypeLine(const QString &value);

    QString        GetLineColor() const;
    void           SetLineColor(const QString &value);
public slots:
    virtual void   ChosenObject(quint32 id, const SceneObject &type) override;
    virtual void   PointNameChanged() override;
protected:
    virtual void   ShowVisualization() override;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void   SaveData() override;
    virtual bool   IsValid() const final;
private:
    Q_DISABLE_COPY(DialogLine)

    /** @brief ui keeps information about user interface */
    Ui::DialogLine *ui;

    bool flagError;
};

//---------------------------------------------------------------------------------------------------------------------
inline bool DialogLine::IsValid() const
{
    return flagError;
}

#endif // DIALOGLINE_H
