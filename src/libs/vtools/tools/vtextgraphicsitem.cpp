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

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QDebug>

#include "vtextgraphicsitem.h"

#define RESIZE_SQUARE               30
#define MIN_W                       120
#define MIN_H                       60
#define MIN_FONT_SIZE               12
#define MAX_FONT_SIZE               36

//---------------------------------------------------------------------------------------------------------------------
VTextGraphicsItem::VTextGraphicsItem(QGraphicsItem* pParent)
    : QGraphicsTextItem(pParent)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_eMode = mNormal;
    m_rectBoundingBox.setTopLeft(QPointF(0, 0));
    setTextWidth(MIN_W);
    m_rectBoundingBox.setWidth(MIN_W);
    m_iMinH = MIN_H;
    m_rectBoundingBox.setHeight(m_iMinH);
    setZValue(2);
}

//---------------------------------------------------------------------------------------------------------------------
VTextGraphicsItem::~VTextGraphicsItem()
{}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(option->rect, QColor(251, 251, 175));
    QGraphicsTextItem::paint(painter, option, widget);

    if (m_eMode != mNormal)
    {
        painter->setPen(QPen(Qt::black, 2, Qt::DashLine));
        painter->drawRect(boundingRect().adjusted(1, 1, -1, -1));

        painter->setPen(Qt::black);
        painter->setBrush(Qt::black);
        SetResizeArea();
        painter->drawRect(m_rectResize);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::Reset()
{
    m_eMode = mNormal;
    Update();
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VTextGraphicsItem::boundingRect() const
{   return m_rectBoundingBox; }

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::SetHTML(const QString& qsHtml)
{
    QGraphicsTextItem::setHtml(qsHtml);
    m_rectBoundingBox.setHeight(document()->size().height());

    // calculate the minimal height
    QScopedPointer<QTextDocument> pDoc(document()->clone());
    QFont fnt = font();
    fnt.setPixelSize(MIN_FONT_SIZE);
    pDoc->setDefaultFont(fnt);
    m_iMinH = pDoc->size().height();
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *pME)
{
    if ((pME->button() & Qt::LeftButton) > 0)
    {
        SetResizeArea();
        if (m_rectResize.contains(pME->pos()) == true)
        {
            m_eMode = mResize;
        }
        else
        {
            m_eMode = mMove;
        }
        Update();
        m_ptStart = pME->pos();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *pME)
{
    if (m_eMode == mMove)
    {
        m_eMode = mMove;
        QPointF ptRel = pME->pos() - m_ptStart;
        moveBy(ptRel.x(), ptRel.y());
    }
    else if (m_eMode == mResize)
    {
        prepareGeometryChange();
        qreal fW = m_rectBoundingBox.width() + pME->pos().x() - m_ptStart.x();
        qreal fH = m_rectBoundingBox.height() + pME->pos().y() - m_ptStart.y();

        if (fW >= MIN_W)
        {
            m_rectBoundingBox.setWidth(fW);
            setTextWidth(fW);
        }
        if (fH >= m_iMinH)
        {
            m_rectBoundingBox.setHeight(fH);
        }
        m_ptStart = pME->pos();

        UpdateFont();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *pME)
{
    if ((pME->button() & Qt::LeftButton) > 0)
    {
        if (m_eMode == mMove)
        {
            m_eMode = mActivated;
            emit SignalMoved(pos());
        }
        else if (m_eMode == mResize)
        {
            m_eMode = mActivated;
            emit SignalResized(textWidth(), font().pixelSize());
        }
        Update();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::Update()
{
    update(boundingRect());
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::SetResizeArea()
{
    m_rectResize.setLeft(boundingRect().right() - RESIZE_SQUARE);
    m_rectResize.setTop(boundingRect().bottom() - RESIZE_SQUARE);
    m_rectResize.setWidth(RESIZE_SQUARE);
    m_rectResize.setHeight(RESIZE_SQUARE);
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::UpdateFont()
{
    QScopedPointer<QTextDocument> pDoc(document()->clone());
    pDoc->setTextWidth(m_rectBoundingBox.width());

    QFont fnt = font();
    int iFS = fnt.pixelSize();
    QSizeF sz = pDoc->size();
    while (sz.width() < m_rectBoundingBox.width() && sz.height() < m_rectBoundingBox.height() && iFS < MAX_FONT_SIZE)
    {
        ++iFS;
        fnt.setPixelSize(iFS);
        pDoc->setDefaultFont(fnt);
        sz = pDoc->size();
    }

    while ((sz.width() >= m_rectBoundingBox.width() || sz.height() >= m_rectBoundingBox.height()) && iFS > MIN_FONT_SIZE)
    {
        --iFS;
        fnt.setPixelSize(iFS);
        pDoc->setDefaultFont(fnt);
        sz = pDoc->size();

    }

    fnt.setPixelSize(iFS);
    setFont(fnt);
    Update();
}

//---------------------------------------------------------------------------------------------------------------------

