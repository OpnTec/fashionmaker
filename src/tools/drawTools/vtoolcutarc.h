/************************************************************************
 **
 **  @file   vtoolcutarc.h
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   7 1, 2014
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

#ifndef VTOOLCUTARC_H
#define VTOOLCUTARC_H

#include "vtoolpoint.h"
#include "../../dialogs/dialogcutarc.h"
#include "../../widgets/vsimplearc.h"

class VToolCutArc : public VToolPoint
{
    Q_OBJECT
public:
    VToolCutArc(VDomDocument *doc, VContainer *data, const qint64 &id, const QString &formula,
                const qint64 &arcId, const qint64 &arc1id, const qint64 &arc2id,
                const Tool::Sources &typeCreation, QGraphicsItem * parent = 0);
    /**
     * @brief setDialog
     */
    virtual void setDialog();
    static void  Create(QSharedPointer<DialogCutArc> &dialog, VMainGraphicsScene *scene, VDomDocument *doc,
                        VContainer *data);
    static void  Create(const qint64 _id, const QString &pointName, const QString &formula, const qint64 &arcId,
                        const qreal &mx, const qreal &my, VMainGraphicsScene *scene, VDomDocument *doc,
                        VContainer *data, const Document::Documents &parse, const Tool::Sources &typeCreation);
    /**
     * @brief ToolType
     */
    static const QString ToolType;
    static const QString AttrArc;
public slots:
    /**
     * @brief FullUpdateFromFile
     */
    virtual void FullUpdateFromFile();
    /**
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void FullUpdateFromGui(int result);
    /**
     * @brief ArcChoosed
     * @param id
     */
    void ArcChoosed(qint64 id);
    /**
     * @brief ChangedActivDraw
     * @param newName
     */
    virtual void      ChangedActivDraw(const QString &newName);
    virtual void      ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    /**
     * @brief contextMenuEvent
     * @param event
     */
    virtual void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile
     */
    virtual void AddToFile();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void RefreshDataInFile();
    /**
     * @brief RefreshGeometry
     */
    void      RefreshGeometry();
private:
    Q_DISABLE_COPY(VToolCutArc)
    /**
     * @brief formula keep formula of length
     */
    QString           formula;
    /**
     * @brief arcId keep id of arc
     */
    qint64            arcId;
    /**
     * @brief DialogCutSpline pointer to the tool's dialog
     */
    QSharedPointer<DialogCutArc> dialogCutArc;
    /**
     * @brief firstArc
     */
    VSimpleArc *firstArc;
    /**
     * @brief secondArc
     */
    VSimpleArc *secondArc;
    const qint64 arc1id;
    const qint64 arc2id;
    void RefreshArc(VSimpleArc *sArc, qint64 arcid, SimpleArc::Translation tr);
};

#endif // VTOOLCUTARC_H
