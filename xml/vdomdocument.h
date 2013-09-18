/****************************************************************************
 **
 **  Copyright (C) 2013 Valentina project All Rights Reserved.
 **
 **  This file is part of Valentina.
 **
 **  Tox is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Tox is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 ****************************************************************************/

#ifndef VDOMDOCUMENT_H
#define VDOMDOCUMENT_H

#include <QDomDocument>
#include <QMap>
#include <QObject>
#include <QComboBox>
#include "container/vcontainer.h"
#include "widgets/vmaingraphicsscene.h"
#include "tools/vdatatool.h"
#include "vtoolrecord.h"

namespace Document{
    enum Enum
    {
        LiteParse,
        FullParse
    };
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class VDomDocument : public QObject, public QDomDocument
{
    Q_OBJECT
public:
                VDomDocument(VContainer *data,QComboBox *comboBoxDraws, Draw::Mode *mode);
                VDomDocument(const QString& name, VContainer *data, QComboBox *comboBoxDraws,
                             Draw::Mode *mode);
                VDomDocument(const QDomDocumentType& doctype, VContainer *data, QComboBox *comboBoxDraws,
                             Draw::Mode *mode);
                ~VDomDocument();
    QDomElement elementById(const QString& id);
    void        CreateEmptyFile();
    void        ChangeActivDraw(const QString& name, Document::Enum parse = Document::FullParse);
    QString     GetNameActivDraw() const;
    bool        GetActivDrawElement(QDomElement &element);
    bool        GetActivCalculationElement(QDomElement &element);
    bool        GetActivModelingElement(QDomElement &element);
    bool        GetActivDetailsElement(QDomElement &element);
    bool        appendDraw(const QString& name);
    void        SetNameDraw(const QString& name);
    void        Parse(Document::Enum parse, VMainGraphicsScene *sceneDraw, VMainGraphicsScene *sceneDetail);
    QMap<qint64, VDataTool*>* getTools();
    QVector<VToolRecord> *getHistory();
    qint64      getCursor() const;
    void        setCursor(const qint64 &value);
    void        setCurrentData();
    void        GarbageCollector();
    void        AddTool(const qint64 &id, VDataTool *tool);
    void        UpdateToolData(const qint64 &id, VContainer *data);
signals:
    void        ChangedActivDraw(const QString newName);
    void        ChangedNameDraw(const QString oldName, const QString newName);
    void        FullUpdateFromFile();
    void        haveChange();
    void        ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    void        ChangedCursor(qint64 id);
public slots:
    void        FullUpdateTree();
    void        haveLiteChange();
    void        ShowHistoryTool(qint64 id, Qt::GlobalColor color, bool enable);
private:
    QMap<QString, QDomElement> map;
    QString     nameActivDraw;
    VContainer  *data;
    QMap<qint64, VDataTool*> tools;
    QVector<VToolRecord> history;
    qint64 cursor;
    QComboBox *comboBoxDraws;
    Draw::Mode          *mode;
                VDomDocument(const VDomDocument & doc);
    const VDomDocument &operator=(const VDomDocument& doc);
    bool        find(QDomElement node, const QString& id);
    bool        CheckNameDraw(const QString& name) const;
    void        SetActivDraw(const QString& name);
    bool        GetActivNodeElement(const QString& name, QDomElement& element);
    void        ParseDrawElement(VMainGraphicsScene  *sceneDraw, VMainGraphicsScene *sceneDetail,
                                 const QDomNode& node, Document::Enum parse);
    void        ParseDrawMode(VMainGraphicsScene  *sceneDraw, VMainGraphicsScene  *sceneDetail,
                              const QDomNode& node, Document::Enum parse, Draw::Mode mode);
    void        ParseDetailElement(VMainGraphicsScene  *sceneDetail, const QDomElement &domElement,
                                   Document::Enum parse);
    void        ParseDetails(VMainGraphicsScene  *sceneDetail, const QDomElement &domElement,
                             Document::Enum parse);
    void        ParsePointElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                  Document::Enum parse, const QString &type, Draw::Mode mode);
    void        ParseLineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                 Document::Enum parse, Draw::Mode mode);
    void        ParseSplineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                   Document::Enum parse, const QString& type, Draw::Mode mode);
    void        ParseArcElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                Document::Enum parse, const QString& type, Draw::Mode mode);
    void        ParseIncrementsElement(const QDomNode& node);
    qint64      GetParametrId(const QDomElement& domElement) const;
    qint64      GetParametrLongLong(const QDomElement& domElement, const QString &name) const;
    QString     GetParametrString(const QDomElement& domElement, const QString &name) const;
    qreal       GetParametrDouble(const QDomElement& domElement, const QString &name) const;
};

#pragma GCC diagnostic pop

#endif // VDOMDOCUMENT_H
