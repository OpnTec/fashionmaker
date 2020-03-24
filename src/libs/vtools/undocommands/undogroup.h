/************************************************************************
 **
 **  @file   undogroup.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 3, 2020
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2020 Valentina project
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
#ifndef UNDOGROUP_H
#define UNDOGROUP_H


#include <qcompilerdetection.h>
#include <QDomElement>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "vundocommand.h"

class AddGroup : public VUndoCommand
{
    Q_OBJECT
public:
    AddGroup(const QDomElement &xml, VAbstractPattern *doc, QUndoCommand *parent = nullptr);
    virtual ~AddGroup()=default;
    virtual void undo() override;
    virtual void redo() override;
signals:
    void UpdateGroups();
private:
    Q_DISABLE_COPY(AddGroup)
    const QString nameActivDraw;
};

class RenameGroup : public VUndoCommand
{
    Q_OBJECT
public:
    RenameGroup(VAbstractPattern *doc, quint32 id, const QString &name, QUndoCommand *parent = nullptr);
    virtual ~RenameGroup()=default;
    virtual void undo() override;
    virtual void redo() override;
signals:
    void UpdateGroups();
private:
    Q_DISABLE_COPY(RenameGroup)
    QString newName;
    QString oldName{};
};

class ChangeGroupOptions : public VUndoCommand
{
    Q_OBJECT
public:
    ChangeGroupOptions(VAbstractPattern *doc, quint32 id, const QString &name, const QStringList &tags,
                       QUndoCommand *parent = nullptr);
    virtual ~ChangeGroupOptions()=default;
    virtual void undo() override;
    virtual void redo() override;
signals:
    void UpdateGroups();
private:
    Q_DISABLE_COPY(ChangeGroupOptions)
    QString newName;
    QString oldName{};
    QStringList newTags;
    QStringList oldTags{};
};

class AddItemToGroup : public VUndoCommand
{
    Q_OBJECT
public:
    AddItemToGroup(const QDomElement &xml, VAbstractPattern *doc, quint32 groupId, QUndoCommand *parent = nullptr);
    virtual ~AddItemToGroup()=default;
    virtual void undo() override;
    virtual void redo() override;
signals:
    void UpdateGroups();
protected:
    void performUndoRedo(bool isUndo);
private:
    Q_DISABLE_COPY(AddItemToGroup)
    const QString nameActivDraw;
};

class RemoveItemFromGroup : public VUndoCommand
{
    Q_OBJECT
public:
    RemoveItemFromGroup(const QDomElement &xml, VAbstractPattern *doc, quint32 groupId, QUndoCommand *parent = nullptr);
    virtual ~RemoveItemFromGroup()=default;
    virtual void undo() override;
    virtual void redo() override;
signals:
    void UpdateGroups();
protected:
    void performUndoRedo(bool isUndo);
private:
    Q_DISABLE_COPY(RemoveItemFromGroup)
    const QString nameActivDraw;
};

class ChangeGroupVisibility  : public VUndoCommand
{
    Q_OBJECT
public:
    ChangeGroupVisibility(VAbstractPattern *doc, vidtype id, bool visible, QUndoCommand *parent = nullptr);
    virtual ~ChangeGroupVisibility()=default;
    virtual void undo() override;
    virtual void redo() override;

signals:
    void UpdateGroup(vidtype id, bool visible);

private:
    Q_DISABLE_COPY(ChangeGroupVisibility)

    bool m_oldVisibility{true};
    bool m_newVisibility{true};
    const QString m_nameActivDraw{};

    void Do(bool visible);
};

class ChangeMultipleGroupsVisibility  : public VUndoCommand
{
    Q_OBJECT
public:
    ChangeMultipleGroupsVisibility(VAbstractPattern *doc, const QVector<vidtype> &groups, bool visible,
                                   QUndoCommand *parent = nullptr);
    virtual ~ChangeMultipleGroupsVisibility() =default;
    virtual void undo() override;
    virtual void redo() override;

signals:
    void UpdateMultipleGroups(const QMap<vidtype, bool> &groups);

private:
    Q_DISABLE_COPY(ChangeMultipleGroupsVisibility)

    QVector<vidtype> m_groups;
    bool m_newVisibility{true};
    QMap<vidtype, bool> m_oldVisibility{};
    const QString m_nameActivDraw{};
};

class DelGroup : public VUndoCommand
{
    Q_OBJECT
public:
    DelGroup(VAbstractPattern *doc, quint32 id, QUndoCommand *parent = nullptr);
    virtual ~DelGroup()=default;
    virtual void undo() override;
    virtual void redo() override;
signals:
    void UpdateGroups();
private:
    Q_DISABLE_COPY(DelGroup)
    const QString nameActivDraw;
};

#endif // UNDOGROUP_H
