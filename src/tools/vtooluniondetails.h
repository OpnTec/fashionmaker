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
                      const qint64 &d1P1, const qint64 &d1P2, const qint64 &d2P1, const qint64 &d2P2,
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
                        const qint64 &d1P1, const qint64 &d1P2, const qint64 &d2P1, const qint64 &d2P2,
                        VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                        const Document::Documents &parse, const Tool::Sources &typeCreation);
    static QVector<VDetail> GetDetailFromFile(VDomDocument *doc, const QDomElement &domElement);
    /**
     * @brief TagName
     */
    static const QString TagName;
    static const QString ToolType;
    static const QString TagDetail;
    static const QString TagNode;
    static const QString AttrD1P1;
    static const QString AttrD1P2;
    static const QString AttrD2P1;
    static const QString AttrD2P2;
    static const QString AttrIdObject;
    static const QString AttrNodeType;
    static const QString NodeTypeContour;
    static const QString NodeTypeModeling;

    inline qint64 getD1P1() const{return d1P1;}
    inline qint64 getD1P2() const{return d1P2;}
    inline qint64 getD2P1() const{return d2P1;}
    inline qint64 getD2P2() const{return d2P2;}
    void         AddToNewDetail(VContainer *data, VDetail *newDetail, const VDetail &det, const ptrdiff_t &a,
                                const ptrdiff_t &b, const qreal &dx = 0, const qreal &dy = 0, const qint64 &pRotate = 0,
                                const qreal &angle = 0) const;
    void         UpdatePoints(const qint64 &idDetail, VContainer *data, const VDetail &det, const ptrdiff_t &a,
                              const ptrdiff_t &b, const qreal &dx = 0, const qreal &dy = 0, const qint64 &pRotate = 0,
                              const qreal &angle = 0) const;
    void         BiasRotatePoint(VPointF *point, const qreal &dx, const qreal &dy, const QPointF &pRotate,
                                 const qreal angle)const;
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
    qint64       d1P1;
    qint64       d1P2;
    qint64       d2P1;
    qint64       d2P2;
    void         AddDetail(QDomElement &domElement, VDetail &d);
    void         AddNode(QDomElement &domElement, VNodeDetail &node);
    QDomNode     UpdateDetail(QDomNode &domNode, VDetail &d);
    void         UpdateNode(QDomNode &domNode, VNodeDetail &node);
    void         AddToModeling(const QDomElement &domElement);
    void         CorectPoints(const VDetail &detail, qint64 &p1, qint64 &p2);

};

#endif // VTOOLUNIONDETAILS_H
