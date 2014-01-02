/************************************************************************
 **
 **  @file   vtooluniondetails.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   26 12, 2013
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

#ifndef VTOOLUNIONDETAILS_H
#define VTOOLUNIONDETAILS_H

#include "vabstracttool.h"
#include "../dialogs/dialoguniondetails.h"

class VToolUnionDetails : public VAbstractTool
{
    Q_OBJECT
public:
    VToolUnionDetails(VDomDocument *doc, VContainer *data, const qint64 &id, const VDetail &d1, const VDetail &d2,
                      const ptrdiff_t &indexD1, const ptrdiff_t &indexD2,
                      const Tool::Sources &typeCreation);
    virtual void setDialog() {}
    /**
     * @brief Create
     * @param dialog
     * @param doc dom document container
     * @param data
     */
    static void  Create(QSharedPointer<DialogUnionDetails> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                        VContainer *data);
    /**
     * @brief Create
     * @param _id
     * @param newDetail
     * @param scene
     * @param doc dom document container
     * @param data
     * @param parse
     * @param typeCreation
     */
    static void  Create(const qint64 _id, const VDetail &d1,  const VDetail &d2, const qint64 &d1id, const qint64 &d2id,
                        const ptrdiff_t &indexD1, const ptrdiff_t &indexD2, VMainGraphicsScene *scene,
                        VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                        const Tool::Sources &typeCreation);
    static QVector<VDetail> GetDetailFromFile(VDomDocument *doc, const QDomElement &domElement);
    /**
     * @brief TagName
     */
    static const QString TagName;
    static const QString ToolType;
    static const QString TagDetail;
    static const QString TagNode;
    static const QString AttrIndexD1;
    static const QString AttrIndexD2;
    static const QString AttrIdObject;
    static const QString AttrNodeType;
    static const QString NodeTypeContour;
    static const QString NodeTypeModeling;

    static void  AddToNewDetail(VDomDocument *doc, VContainer *data, VDetail &newDetail, const VDetail &det,
                                const ptrdiff_t &i, const qint64 &idTool, const qreal &dx = 0, const qreal &dy = 0,
                                const qint64 &pRotate = 0, const qreal &angle = 0);
    static void  UpdatePoints(const qint64 &idDetail, VContainer *data, const VDetail &det, const ptrdiff_t &i,
                              qint64 &idCount, const qreal &dx = 0, const qreal &dy = 0, const qint64 &pRotate = 0,
                              const qreal &angle = 0);
    static void  BiasRotatePoint(VPointF *point, const qreal &dx, const qreal &dy, const QPointF &pRotate,
                                 const qreal angle);
public slots:
    /**
     * @brief FullUpdateFromFile
     */
    virtual void FullUpdateFromFile (){}
protected:
    /**
     * @brief AddToFile
     */
    virtual void AddToFile();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void RefreshDataInFile();
private:
    Q_DISABLE_COPY(VToolUnionDetails)
    VDetail      d1;
    VDetail      d2;
    ptrdiff_t    indexD1;
    ptrdiff_t    indexD2;
    void         AddDetail(QDomElement &domElement, VDetail &d);
    void         AddNode(QDomElement &domElement, const VNodeDetail &node);
    QDomNode     UpdateDetail(const QDomNode &domNode, const VDetail &d);
    void         AddToModeling(const QDomElement &domElement);
};

#endif // VTOOLUNIONDETAILS_H
