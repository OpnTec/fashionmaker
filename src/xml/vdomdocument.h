/************************************************************************
 **
 **  @file   vdomdocument.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
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

#ifndef VDOMDOCUMENT_H
#define VDOMDOCUMENT_H

#include "../container/vcontainer.h"
#include "../widgets/vmaingraphicsscene.h"
#include "../tools/vdatatool.h"
#include "vtoolrecord.h"

#include <QComboBox>
#include <QDomDocument>
#include <QDebug>

namespace Document
{
    /**
     * @brief The Document enum
     */
    enum Document { LiteParse, FullParse};
    Q_DECLARE_FLAGS(Documents, Document)
}
Q_DECLARE_OPERATORS_FOR_FLAGS(Document::Documents)

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

/**
 * @brief The VDomDocument class
 */
class VDomDocument : public QObject, public QDomDocument
{
    Q_OBJECT
public:
                   /**
                    * @brief VDomDocument
                    * @param data
                    * @param comboBoxDraws
                    * @param mode
                    */
                   VDomDocument(VContainer *data, QComboBox *comboBoxDraws, Draw::Draws *mode);
                   /**
                    * @brief VDomDocument
                    * @param name
                    * @param data
                    * @param comboBoxDraws
                    * @param mode
                    */
                   VDomDocument(const QString& name, VContainer *data, QComboBox *comboBoxDraws, Draw::Draws *mode);
                   /**
                    * @brief VDomDocument
                    * @param doc dom document containertype
                    * @param data
                    * @param comboBoxDraws
                    * @param mode
                    */
                   VDomDocument(const QDomDocumentType& doctype, VContainer *data, QComboBox *comboBoxDraws,
                                Draw::Draws *mode);
                   ~VDomDocument(){}
    /**
     * @brief elementById
     * @param id
     * @return
     */
    QDomElement    elementById(const QString& id);
    /**
     * @brief CreateEmptyFile
     */
    void           CreateEmptyFile();
    /**
     * @brief ChangeActivDraw
     * @param name
     * @param parse
     */
    void           ChangeActivDraw(const QString& name, const Document::Documents &parse = Document::FullParse);
    /**
     * @brief GetNameActivDraw
     * @return
     */
    inline QString GetNameActivDraw() const {return nameActivDraw;}
    /**
     * @brief GetActivDrawElement
     * @param element
     * @return
     */
    bool           GetActivDrawElement(QDomElement &element);
    /**
     * @brief GetActivCalculationElement
     * @param element
     * @return
     */
    bool           GetActivCalculationElement(QDomElement &element);
    /**
     * @brief GetActivModelingElement
     * @param element
     * @return
     */
    bool           GetActivModelingElement(QDomElement &element);
    /**
     * @brief GetActivDetailsElement
     * @param element
     * @return
     */
    bool           GetActivDetailsElement(QDomElement &element);
    /**
     * @brief appendDraw
     * @param name
     * @return
     */
    bool           appendDraw(const QString& name);
    /**
     * @brief SetNameDraw
     * @param name
     * @return
     */
    bool           SetNameDraw(const QString& name);
    /**
     * @brief Parse
     * @param parse
     * @param sceneDraw
     * @param sceneDetail
     */
    void           Parse(const Document::Documents &parse, VMainGraphicsScene *sceneDraw,
                         VMainGraphicsScene *sceneDetail);
    /**
     * @brief getTools
     * @return
     */
    inline QHash<qint64, VDataTool*>* getTools() {return &tools;}
    /**
     * @brief getHistory
     * @return
     */
    inline QVector<VToolRecord> *getHistory() {return &history;}
    /**
     * @brief getCursor
     * @return
     */
    inline qint64  getCursor() const {return cursor;}
    /**
     * @brief setCursor
     * @param value
     */
    void           setCursor(const qint64 &value);
    /**
     * @brief setCurrentData
     */
    void           setCurrentData();
    /**
     * @brief AddTool
     * @param id
     * @param tool
     */
    void           AddTool(const qint64 &id, VDataTool *tool);
    /**
     * @brief UpdateToolData
     * @param id
     * @param data
     */
    void           UpdateToolData(const qint64 &id, VContainer *data);
    /**
     * @brief IncrementReferens
     * @param id
     */
    void           IncrementReferens(qint64 id) const;
    /**
     * @brief DecrementReferens
     * @param id
     */
    void           DecrementReferens(qint64 id) const;
    /**
     * @brief TestUniqueId
     */
    void           TestUniqueId() const;
signals:
    /**
     * @brief ChangedActivDraw
     * @param newName
     */
    void           ChangedActivDraw(const QString &newName);
    /**
     * @brief ChangedNameDraw
     * @param oldName
     * @param newName
     */
    void           ChangedNameDraw(const QString oldName, const QString newName);
    /**
     * @brief FullUpdateFromFile
     */
    void           FullUpdateFromFile();
    /**
     * @brief haveChange
     */
    void           haveChange();
    /**
     * @brief ShowTool
     * @param id
     * @param color
     * @param enable
     */
    void           ShowTool(qint64 id, Qt::GlobalColor color, bool enable);
    /**
     * @brief ChangedCursor
     * @param id
     */
    void           ChangedCursor(qint64 id);
public slots:
    /**
     * @brief FullUpdateTree
     */
    void           FullUpdateTree();
    /**
     * @brief haveLiteChange
     */
    void           haveLiteChange();
    /**
     * @brief ShowHistoryTool
     * @param id
     * @param color
     * @param enable
     */
    void           ShowHistoryTool(qint64 id, Qt::GlobalColor color, bool enable);
private:
    Q_DISABLE_COPY(VDomDocument)
    /**
     * @brief map
     */
    QHash<QString, QDomElement> map;
    /**
     * @brief nameActivDraw
     */
    QString        nameActivDraw;
    /**
     * @brief data container with data
     */
    VContainer     *data;
    /**
     * @brief tools
     */
    QHash<qint64, VDataTool*> tools;
    /**
     * @brief history
     */
    QVector<VToolRecord> history;
    /**
     * @brief cursor
     */
    qint64         cursor;
    /**
     * @brief comboBoxDraws
     */
    QComboBox      *comboBoxDraws;
    /**
     * @brief mode
     */
    Draw::Draws    *mode;
    /**
     * @brief find
     * @param node
     * @param id
     * @return
     */
    bool           find(const QDomElement &node, const QString& id);
    /**
     * @brief CheckNameDraw
     * @param name
     * @return
     */
    bool           CheckNameDraw(const QString& name) const;
    /**
     * @brief SetActivDraw
     * @param name
     */
    void           SetActivDraw(const QString& name);
    /**
     * @brief GetActivNodeElement
     * @param name
     * @param element
     * @return
     */
    bool           GetActivNodeElement(const QString& name, QDomElement& element);
    /**
     * @brief ParseDrawElement
     * @param sceneDraw
     * @param sceneDetail
     * @param node
     * @param parse
     */
    void           ParseDrawElement(VMainGraphicsScene  *sceneDraw, VMainGraphicsScene *sceneDetail,
                                    const QDomNode& node, const Document::Documents &parse);
    /**
     * @brief ParseDrawMode
     * @param sceneDraw
     * @param sceneDetail
     * @param node
     * @param parse
     * @param mode
     */
    void           ParseDrawMode(VMainGraphicsScene  *sceneDraw, VMainGraphicsScene  *sceneDetail,
                                 const QDomNode& node, const Document::Documents &parse, const Draw::Draws &mode);
    /**
     * @brief ParseDetailElement
     * @param sceneDetail
     * @param domElement
     * @param parse
     */
    void           ParseDetailElement(VMainGraphicsScene  *sceneDetail, const QDomElement &domElement,
                                      const Document::Documents &parse);
    /**
     * @brief ParseDetails
     * @param sceneDetail
     * @param domElement
     * @param parse
     */
    void           ParseDetails(VMainGraphicsScene  *sceneDetail, const QDomElement &domElement,
                                const Document::Documents &parse);
    /**
     * @brief ParsePointElement
     * @param scene
     * @param domElement
     * @param parse
     * @param type
     * @param mode
     */
    void           ParsePointElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                     const Document::Documents &parse, const QString &type);
    /**
     * @brief ParseLineElement
     * @param scene
     * @param domElement
     * @param parse
     * @param mode
     */
    void           ParseLineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                    const Document::Documents &parse);
    /**
     * @brief ParseSplineElement
     * @param scene
     * @param domElement
     * @param parse
     * @param type
     * @param mode
     */
    void           ParseSplineElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                      const Document::Documents &parse, const QString& type);
    /**
     * @brief ParseArcElement
     * @param scene
     * @param domElement
     * @param parse
     * @param type
     * @param mode
     */
    void           ParseArcElement(VMainGraphicsScene *scene, const QDomElement& domElement,
                                   const Document::Documents &parse, const QString& type);
    /**
     * @brief ParseIncrementsElement
     * @param node
     */
    void           ParseIncrementsElement(const QDomNode& node);
    /**
     * @brief GetParametrId
     * @param domElement
     * @return
     */
    qint64         GetParametrId(const QDomElement& domElement) const;
    /**
     * @brief GetParametrLongLong
     * @param domElement
     * @param name
     * @return
     */
    qint64         GetParametrLongLong(const QDomElement& domElement, const QString &name,
                                       const QString &defValue) const;
    /**
     * @brief GetParametrString
     * @param domElement
     * @param name
     * @return
     */
    QString        GetParametrString(const QDomElement& domElement, const QString &name, const QString &defValue) const;
    /**
     * @brief GetParametrDouble
     * @param domElement
     * @param name
     * @return
     */
    qreal          GetParametrDouble(const QDomElement& domElement, const QString &name, const QString &defValue) const;
    /**
     * @brief CollectId
     * @param node
     * @param vector
     */
    void           CollectId(const QDomElement &node, QVector<qint64> &vector)const;
};

#ifdef Q_CC_GNU
    #pragma GCC diagnostic pop
#endif

#endif // VDOMDOCUMENT_H
