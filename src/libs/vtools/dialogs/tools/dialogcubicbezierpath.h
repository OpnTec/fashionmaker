/************************************************************************
 **
 **  @file   dialogcubicbezierpath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 3, 2016
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

#ifndef DIALOGCUBICBEZIERPATH_H
#define DIALOGCUBICBEZIERPATH_H

#include "dialogtool.h"
#include "../vgeometry/vcubicbezierpath.h"

namespace Ui
{
    class DialogCubicBezierPath;
}

class DialogCubicBezierPath : public DialogTool
{
    Q_OBJECT

public:
    explicit DialogCubicBezierPath(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);
    virtual ~DialogCubicBezierPath();

    VCubicBezierPath GetPath() const;
    void             SetPath(const VCubicBezierPath &value);

    QString GetColor() const;
    void    SetColor(const QString &value);
public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    virtual void ShowDialog(bool click) Q_DECL_OVERRIDE;
protected:
    virtual void ShowVisualization() Q_DECL_OVERRIDE;
    virtual void SaveData() Q_DECL_OVERRIDE;
private slots:
    void PointChanged(int row);
    void currentPointChanged(int index);

private:
    Q_DISABLE_COPY(DialogCubicBezierPath)
    Ui::DialogCubicBezierPath *ui;

    /** @brief path cubic bezier path */
    VCubicBezierPath path;

    qint32 newDuplicate;

    void             NewItem(const VPointF &point);
    void             DataPoint(const VPointF &p);
    void             SavePath();
    QSet<quint32>    AllPathBackboneIds() const;
    bool             IsPathValid() const;
    VCubicBezierPath ExtractPath() const;
};

#endif // DIALOGCUBICBEZIERPATH_H
