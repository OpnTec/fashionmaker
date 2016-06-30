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
#include <QTransform>
#include <QDebug>

#include "vtextgraphicsitem.h"

#define RESIZE_SQUARE               30
#define ROTATE_CIRCLE               20
#define MIN_W                       120
#define MIN_H                       60
#define MIN_FONT_SIZE               12
#define MAX_FONT_SIZE               48
#define SPACING                     2

//---------------------------------------------------------------------------------------------------------------------
VTextGraphicsItem::VTextGraphicsItem(QGraphicsItem* pParent)
    : QGraphicsObject(pParent)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    m_eMode = mNormal;
    m_bReleased = false;
    m_rectBoundingBox.setTopLeft(QPointF(0, 0));
    m_iMinH = MIN_H;
    SetSize(MIN_W, m_iMinH);
    setZValue(2);
}

//---------------------------------------------------------------------------------------------------------------------
VTextGraphicsItem::~VTextGraphicsItem()
{}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::SetFont(const QFont& fnt)
{
    m_font = fnt;
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->fillRect(option->rect, QColor(251, 251, 175));

    // draw text lines
    int iY = 0;
    int iH = 0;
    painter->setPen(Qt::black);
    QFont fnt = m_font;
    for (int i = 0; i < m_liOutput.count(); ++i)
    {
        const TextLine& tl = m_liOutput.at(i);
        iH = tl.m_iHeight;
        fnt.setPixelSize(m_font.pixelSize() + tl.m_iFontSize);
        fnt.setWeight(tl.m_eFontWeight);
        fnt.setStyle(tl.m_eStyle);
        painter->setFont(fnt);
        painter->drawText(0, iY, boundingRect().width(), iH, tl.m_eAlign, tl.m_qsText);
        iY += iH + SPACING;
    }


    if (m_eMode != mNormal)
    {
        painter->setPen(QPen(Qt::black, 2, Qt::DashLine));
        painter->drawRect(boundingRect().adjusted(1, 1, -1, -1));

        if (m_eMode != mRotate)
        {
            painter->setPen(Qt::black);
            painter->setBrush(Qt::black);
            painter->drawRect(m_rectResize);

            if (m_eMode == mResize)
            {
                painter->drawLine(0, 0, m_rectBoundingBox.width(), m_rectBoundingBox.height());
                painter->drawLine(0, m_rectBoundingBox.height(), m_rectBoundingBox.width(), 0);
            }
        }
        else
        {
            painter->setPen(Qt::black);
            painter->setBrush(Qt::black);
            painter->drawEllipse(
                        m_rectBoundingBox.width()/2,
                        m_rectBoundingBox.height()/2,
                        ROTATE_CIRCLE,
                        ROTATE_CIRCLE
                        );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::Reset()
{
    return;
    m_eMode = mNormal;
    m_bReleased = false;
    Update();
    setZValue(2);
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::AddLine(const TextLine& tl)
{
    m_liLines << tl;
    while (IsBigEnough(MIN_W, m_iMinH, MIN_FONT_SIZE) == false)
    {
        m_iMinH += 5;
    }
    if (m_rectBoundingBox.height() < m_iMinH)
    {
        SetSize(m_rectBoundingBox.width(), m_iMinH);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::Clear()
{
    m_liLines.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::SetSize(qreal fW, qreal fH)
{
    // don't allow resize under specific size
    if (fW < MIN_W || fH < m_iMinH)
        return;

    m_rectBoundingBox.setWidth(fW);
    m_rectBoundingBox.setHeight(fH);
    m_rectResize.setTopLeft(QPointF(fW - RESIZE_SQUARE, fH - RESIZE_SQUARE));
    m_rectResize.setWidth(RESIZE_SQUARE);
    m_rectResize.setHeight(RESIZE_SQUARE);
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::Update()
{
    UpdateFont();
    UpdateBox();
}

//---------------------------------------------------------------------------------------------------------------------
QRectF VTextGraphicsItem::boundingRect() const
{
    return m_rectBoundingBox;
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *pME)
{
    if (pME->button() == Qt::LeftButton)
    {
        qDebug() << "PRESS" << m_eMode;
        m_ptStartPos = pos();
        m_ptStart = pME->scenePos();
        m_szStart = m_rectBoundingBox.size();
        m_dAngle = GetAngle(pME->scenePos());
        m_dRotation = rotation();
        if (m_eMode != mRotate)
        {
            if (m_rectResize.contains(pME->pos()) == true)
            {
                m_eMode = mResize;
            }
            else
            {
                m_eMode = mMove;
            }
        }
        setZValue(3);
        UpdateBox();
        qDebug() << "PRESS finished" << m_eMode;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* pME)
{
    QPointF ptDiff = pME->scenePos() - m_ptStart;
    if (m_eMode == mMove)
    {
        //moveBy(ptDiff.x(), ptDiff.y());
        setPos(m_ptStartPos + ptDiff);
        UpdateBox();
    }
    else if (m_eMode == mResize)
    {
        SetSize(m_szStart.width() + ptDiff.x(), m_szStart.height() + ptDiff.y());
        Update();
        emit SignalShrink();
    }
    else if (m_eMode == mRotate)
    {
        if (fabs(m_dAngle) < 0.01)
        {
            m_dAngle = GetAngle(pME->scenePos());
            return;
        }
        double dAng = 180*(GetAngle(pME->scenePos()) - m_dAngle)/M_PI;
        setRotation(m_dRotation + dAng);
        Update();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* pME)
{
    if (pME->button() == Qt::LeftButton)
    {
        double dDist = fabs(pME->scenePos().x() - m_ptStart.x()) + fabs(pME->scenePos().y() - m_ptStart.y());
        bool bShort = (dDist < 2);

        qDebug() << "RELEASE" << m_eMode << dDist;

        if (m_eMode == mMove || m_eMode == mResize)
        {   // when released in mMove or mResize mode
            if (bShort == true)
            {
                if (m_bReleased == true)
                {
                    m_eMode = mRotate;
                    UpdateBox();
                }
            }
            else if (m_eMode == mMove)
            {
                emit SignalMoved(pos());
                UpdateBox();
            }
            else
            {
                emit SignalResized(m_rectBoundingBox.width(), m_font.pixelSize());
                Update();
            }
        }
        else
        {   // when released in mRotate mode
            if (bShort == true)
            {
                m_eMode = mMove;
                UpdateBox();
            }
            else
            {
                emit SignalRotated(rotation());
                UpdateBox();
            }
        }
        m_bReleased = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::UpdateBox()
{
    update(m_rectBoundingBox);
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::UpdateFont()
{
    int iFS = m_font.pixelSize();

    // increase the font size until the bounding rect is not big enough
    while (iFS < MAX_FONT_SIZE && IsBigEnough(m_rectBoundingBox.width(), m_rectBoundingBox.height(), iFS) == true)
    {
        ++iFS;
    }
    // decrease the font size until the bounding rect is big enough
    while (iFS >= MIN_FONT_SIZE && IsBigEnough(m_rectBoundingBox.width(), m_rectBoundingBox.height(), iFS) == false)
    {
        --iFS;
    }
    m_font.setPixelSize(iFS);
    UpdateBox();
}

//---------------------------------------------------------------------------------------------------------------------
bool VTextGraphicsItem::IsBigEnough(qreal fW, qreal fH, int iFontSize)
{
    m_liOutput.clear();
    QFont fnt = m_font;
    int iY = 0;
    for (int i = 0; i < m_liLines.count(); ++i)
    {
        const TextLine& tl = m_liLines.at(i);
        TextLine tlOut = tl;
        fnt.setPixelSize(iFontSize + tl.m_iFontSize);
        QFontMetrics fm(fnt);
        tlOut.m_iHeight = fm.height();
        QStringList qslLines = SplitString(tlOut.m_qsText, fW, fm);
        for (int iL = 0; iL < qslLines.count(); ++iL)
        {
            tlOut.m_qsText = qslLines[iL];
            m_liOutput << tlOut;
            iY += tlOut.m_iHeight + SPACING;
        }
    }
    return iY < fH;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VTextGraphicsItem::SplitString(const QString &qs, qreal fW, const QFontMetrics &fm)
{
    QRegExp reg("\\s+");
    QStringList qslWords = qs.split(reg);
    QStringList qslLines;
    QString qsCurrent;
    for (int i = 0; i < qslWords.count(); ++i)
    {
        if (qsCurrent.length() > 0)
        {
            qsCurrent += " ";
        }
        if (fm.width(qsCurrent + qslWords[i]) > fW)
        {
            qslLines << qsCurrent;
            qsCurrent = qslWords[i];
        }
        else
        {
            qsCurrent += qslWords[i];
        }
    }
    qslLines << qsCurrent;
    return qslLines;
}

//---------------------------------------------------------------------------------------------------------------------
double VTextGraphicsItem::GetAngle(QPointF pt) const
{
    double dX = pt.x() - m_ptStart.x();
    double dY = pt.y() - m_ptStart.y();

    if (fabs(dX) < 1 && fabs(dY) < 1)
        return 0;
    else
        return atan2(dY, dX);
}
