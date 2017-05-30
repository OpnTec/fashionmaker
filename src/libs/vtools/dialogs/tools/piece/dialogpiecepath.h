/************************************************************************
 **
 **  @file   dialogpiecepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 11, 2016
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

#ifndef DIALOGPIECEPATH_H
#define DIALOGPIECEPATH_H

#include "../dialogtool.h"

namespace Ui
{
    class DialogPiecePath;
}

class DialogPiecePath : public DialogTool
{
    Q_OBJECT
public:
    explicit DialogPiecePath(const VContainer *data, quint32 toolId, QWidget *parent = nullptr);
    virtual ~DialogPiecePath();

    void EnbleShowMode(bool disable);

    VPiecePath GetPiecePath() const;
    void       SetPiecePath(const VPiecePath &path);

    quint32 GetPieceId() const;
    void    SetPieceId(quint32 id);

    QString GetFormulaSAWidth() const;
    void    SetFormulaSAWidth(const QString &formula);

    virtual void SetPiecesList(const QVector<quint32> &list) Q_DECL_OVERRIDE;

public slots:
    virtual void ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    virtual void ShowDialog(bool click) Q_DECL_OVERRIDE;

protected:
    virtual void CheckState() Q_DECL_FINAL;
    virtual void ShowVisualization() Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void ShowContextMenu(const QPoint &pos);
    void ListChanged();
    void NameChanged();
    void NodeChanged(int index);
    void PassmarkChanged(int index);
    void ReturnDefBefore();
    void ReturnDefAfter();
    void PassmarkLineTypeChanged(int id);
    void PassmarkAngleTypeChanged(int id);

    void EvalWidth();
    void EvalWidthBefore();
    void EvalWidthAfter();

    void FXWidth();
    void FXWidthBefore();
    void FXWidthAfter();

    void WidthChanged();
    void WidthBeforeChanged();
    void WidthAfterChanged();

    void DeployWidthFormulaTextEdit();
    void DeployWidthBeforeFormulaTextEdit();
    void DeployWidthAfterFormulaTextEdit();

private:
    Q_DISABLE_COPY(DialogPiecePath)
    Ui::DialogPiecePath *ui;
    bool  m_showMode;
    qreal m_saWidth;

    QTimer *m_timerWidth;
    QTimer *m_timerWidthBefore;
    QTimer *m_timerWidthAfter;

    int m_formulaBaseWidth;
    int m_formulaBaseWidthBefore;
    int m_formulaBaseWidthAfter;

    void InitPathTab();
    void InitSeamAllowanceTab();
    void InitPassmarksTab();
    void InitPathTypes();
    void InitNodesList();
    void InitPassmarksList();
    void NodeAngleChanged(int index);

    VPiecePath CreatePath() const;

    bool PathIsValid() const;
    void ValidObjects(bool value);
    void NewItem(const VPieceNode &node);

    PiecePathType GetType() const;
    void          SetType(PiecePathType type);

    Qt::PenStyle GetPenType() const;
    void         SetPenType(const Qt::PenStyle &type);

    QListWidgetItem *GetItemById(quint32 id);

    quint32 GetLastId() const;

    void SetCurrentSABefore(const QString &formula);
    void SetCurrentSAAfter(const QString &formula);

    void UpdateNodeSABefore(const QString &formula);
    void UpdateNodeSAAfter(const QString &formula);

    QString GetFormulaSAWidthBefore() const;
    QString GetFormulaSAWidthAfter() const;
};

#endif // DIALOGPIECEPATH_H
