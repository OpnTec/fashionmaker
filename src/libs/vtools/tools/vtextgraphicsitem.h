/************************************************************************
 **
 **  @file   vtextgraphicsitem.h
 **  @author Bojan Kverh
 **  @date   June 16, 2016
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

#ifndef VTEXTGRAPHICSITEM_H
#define VTEXTGRAPHICSITEM_H

#include <QGraphicsObject>
#include <QFont>
#include <QList>

struct TextLine
{
    QString             m_qsText;
    int                 m_iFontSize;  // 0 means default
    QFont::Weight       m_eFontWeight;
    QFont::Style        m_eStyle;
    Qt::Alignment       m_eAlign;
    int                 m_iHeight;
};

class VTextGraphicsItem : public QGraphicsObject
{
    Q_OBJECT

    enum Mode {
        mNormal,
        mActivated,
        mMove,
        mResize
    };

public:
    VTextGraphicsItem(QGraphicsItem* pParent = 0);
    ~VTextGraphicsItem();

    void                SetFont(const QFont& fnt);
    void                paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void                Reset();

    QRectF              boundingRect() const;
    void                AddLine(const TextLine& tl);
    void                Clear();
    void                SetWidth(qreal fW);

protected:
    void                Resize(qreal fW, qreal fH);
    void                mousePressEvent(QGraphicsSceneMouseEvent* pME);
    void                mouseMoveEvent(QGraphicsSceneMouseEvent* pME);
    void                mouseReleaseEvent(QGraphicsSceneMouseEvent* pME);
    void                Update();
    void                UpdateFont();
    bool                IsBigEnough(qreal fW, qreal fH, int iFontSize);
    QStringList         SplitString(const QString& qs, qreal fW, const QFontMetrics& fm);

signals:
    void                SignalMoved(QPointF ptPos);
    void                SignalResized(qreal iTW, int iFontSize);
    void                SignalShrink();

private:
    Mode                m_eMode;
    QPointF             m_ptStart;
    QSizeF              m_szStart;
    QRectF              m_rectResize;
    int                 m_iMinH;
    QRectF              m_rectBoundingBox;
    QFont               m_font;
    QList<TextLine>     m_liLines;
    QList<TextLine>     m_liOutput;
};

#endif // VTEXTGRAPHICSITEM_H
