﻿/************************************************************************
 **
 **  @file   dialogplacelabel.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 10, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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
#ifndef DIALOGPLACELABEL_H
#define DIALOGPLACELABEL_H

#include "dialogtool.h"

namespace Ui
{
    class DialogPlaceLabel;
}

class VPlaceLabelItem;

class DialogPlaceLabel : public DialogTool
{
    Q_OBJECT

public:
    explicit DialogPlaceLabel(const VContainer *data, quint32 toolId, QWidget *parent = nullptr);
    virtual ~DialogPlaceLabel();

    void EnbleShowMode(bool disable);

    quint32 GetCenterPoint() const;
    void    SetCenterPoint(quint32 id);

    PlaceLabelType GetLabelType() const;
    void           SetLabelType(PlaceLabelType type);

    QString GetWidth() const;
    void    SetWidth(const QString &value);

    QString GetHeight() const;
    void    SetHeight(const QString &value);

    QString GetAngle() const;
    void    SetAngle(const QString &value);

    quint32 GetPieceId() const;
    void    SetPieceId(quint32 id);

    virtual void SetPiecesList(const QVector<quint32> &list) Q_DECL_OVERRIDE;

public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;

protected:
    virtual void CheckState() Q_DECL_FINAL;
    virtual void ShowVisualization() Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void DeployFormulaWidthEdit();
    void DeployFormulaHeightEdit();
    void DeployFormulaAngleEdit();

    void FormulaWidthChanged();
    void FormulaHeightChanged();
    void FormulaAngleChanged();

    void EvalWidth();
    void EvalHeight();
    void EvalAngle();

    void FXWidth();
    void FXHeight();
    void FXAngle();

private:
    Q_DISABLE_COPY(DialogPlaceLabel)
    Ui::DialogPlaceLabel *ui;
    bool m_showMode;
    bool m_flagPoint;
    bool m_flagWidth;
    bool m_flagHeight;
    bool m_flagAngle;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int m_formulaBaseHeightWidth;
    int m_formulaBaseHeightHeight;
    int m_formulaBaseHeightAngle;

    QTimer *timerAngle;
    QTimer *timerWidth;
    QTimer *timerHeight;

    void FillPlaceLabelTypes();

    void CheckPieces();
    void CheckPoint();
};

#endif // DIALOGPLACELABEL_H
