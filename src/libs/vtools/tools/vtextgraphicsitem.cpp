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
#include <QDebug>

#include "vtextgraphicsitem.h"

#define RESIZE_SQUARE               30

//---------------------------------------------------------------------------------------------------------------------
VTextGraphicsItem::VTextGraphicsItem(QGraphicsItem* pParent)
    : QGraphicsTextItem(pParent)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_eMode = mNormal;
    m_iMinW = 100;
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
void VTextGraphicsItem::SetMinimalWidth(int iW)
{
    m_iMinW = iW;
    setTextWidth(m_iMinW);
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::Reset()
{
    m_eMode = mNormal;
    Update();
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::SetHTML(const QString& qsHTML)
{
    QGraphicsTextItem::setHtml(qsHTML);
    m_iMinFontSize = font().pixelSize();
    m_iMinH = boundingRect().height();
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
            m_iCurrentH = boundingRect().height();
        }
        else
        {
            m_eMode = mMove;
        }
        qDebug() << "MOUSEPRESS" << m_rectResize << pME->pos() << int(m_eMode);
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
        int iNewW = textWidth() + pME->pos().x() - m_ptStart.x();
        m_iCurrentH += pME->pos().y() - m_ptStart.y();
        if (iNewW >= m_iMinW)
        {
            setTextWidth(iNewW);
            m_ptStart.setX(pME->pos().x());
            Update();
        }

        if (m_iCurrentH >= m_iMinH)
        {
            int iFontSize = m_iMinFontSize*m_iCurrentH/m_iMinH;
            if (iFontSize > m_iMinFontSize*2)
            {
                // prevent too big letters
                return;
            }
            QFont fnt = font();
            fnt.setPixelSize(iFontSize);
            setFont(fnt);
            m_ptStart.setY(pME->pos().y());
            Update();
        }
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
void VTextGraphicsItem::setHtml(const QString &qsHtml)
{
    Q_UNUSED(qsHtml);
    // prevent using this method! Use SetHTML instead!
}

//---------------------------------------------------------------------------------------------------------------------

