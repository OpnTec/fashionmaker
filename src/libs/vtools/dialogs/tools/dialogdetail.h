/************************************************************************
 **
 **  @file   dialogdetail.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef DIALOGDETAIL_H
#define DIALOGDETAIL_H

#include <qcompilerdetection.h>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QtGlobal>

#include "../vmisc/def.h"
#include "../vpatterndb/vdetail.h"
#include "../vpatterndb/vpatterninfogeometry.h"
#include "../vpatterndb/vpatternpiecedata.h"
#include "dialogtool.h"
#include "ui_dialogdetail.h"

class QWidget;
class VContainer;

/**
 * @brief The DialogDetail class dialog for ToolDetai. Help create detail and edit option.
 */
class DialogDetail : public DialogTool
{
    Q_OBJECT
public:
    DialogDetail(const VContainer *data, const quint32 &toolId, QWidget *parent = nullptr);

    VDetail          getDetail() const;
    void             setDetail(const VDetail &value);
public slots:
    virtual void     ChosenObject(quint32 id, const SceneObject &type) Q_DECL_OVERRIDE;
    void             BiasXChanged(qreal d);
    void             BiasYChanged(qreal d);
    void             AlowenceChanged(qreal d);
    void             ClickedSeams(bool checked);
    void             ClickedClosed(bool checked);
    void             ClickedReverse(bool checked);
    void             ObjectChanged(int row);
    void             DeleteItem();
    void             ScrollUp();
    void             ScrollDown();
protected:
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void     SaveData() Q_DECL_OVERRIDE;
    virtual void     CheckState() Q_DECL_OVERRIDE;

protected slots:
    void             UpdateList();
    void             AddUpdate();
    void             Cancel();
    void             Remove();

private slots:
    void             NameDetailChanged();
    void             MaterialChanged();
private:

    /** @brief ui keeps information about user interface */
    Ui::DialogDetail ui;

    /** @brief detail detail */
    VDetail          detail;

    /** @brief supplement keep option supplement of seams */
    bool             supplement;

    /** @brief closed keep option about equdistant (closed or not) */
    bool             closed;
    bool             flagWidth;
    bool             m_bAddMode;

    QStringList      m_qslMaterials;
    QStringList      m_qslPlacements;
    // temporary container for Material/Cut/Placement 3-tuples
    MCPContainer     m_conMCP;
    VPatternPieceData       m_oldData;
    VPatternInfoGeometry    m_oldGeom;


    bool             DetailIsValid() const;
    bool             FirstPointEqualLast() const;
    bool             DetailIsClockwise() const;

    void             NewItem(quint32 id, const Tool &typeTool, const NodeDetail &typeNode,
                             qreal mx = 0, qreal my = 0, bool reverse = false);
    VDetail          CreateDetail() const;
    void             ValidObjects(bool value);
    void             EnableObjectGUI(bool value);

    void             ClearFields();

    quint32          RowId(int i) const;

private slots:
    void             SetAddMode();
    void             SetEditMode();
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getDetails return detail
 * @return detail
 */
inline VDetail DialogDetail::getDetail() const
{
    return detail;
}

#endif // DIALOGDETAIL_H
