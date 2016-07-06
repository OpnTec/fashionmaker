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
#include <QGraphicsView>
#include <QTransform>
#include <QDebug>

#include "../vmisc/def.h"
#include "vtextgraphicsitem.h"

#define RESIZE_SQUARE               30
#define ROTATE_CIRCLE               20
#define ROTATE_RECT                 60
#define ROTATE_ARC                  50
#define MIN_W                       120
#define MIN_H                       60
#define MIN_FONT_SIZE               12
#define MAX_FONT_SIZE               128
#define SPACING                     2
#define TOP_Z                       2

//---------------------------------------------------------------------------------------------------------------------
TextLine::TextLine()
    :m_qsText(), m_iFontSize(MIN_FONT_SIZE), m_eFontWeight(QFont::Normal), m_eStyle(QFont::StyleNormal),
      m_eAlign(Qt::AlignCenter), m_iHeight(0)
{}

//---------------------------------------------------------------------------------------------------------------------
VTextGraphicsItem::VTextGraphicsItem(QGraphicsItem* pParent)
    :QGraphicsObject(pParent), m_eMode(VTextGraphicsItem::mNormal), m_bReleased(false),
      m_ptStartPos(), m_ptStart(), m_ptRotCenter(), m_szStart(), m_dRotation(0), m_dAngle(0),
      m_rectResize(), m_iMinH(MIN_H), m_rectBoundingBox(), m_font(), m_liLines(), m_liOutput()

{
    m_rectBoundingBox.setTopLeft(QPointF(0, 0));
    SetSize(MIN_W, m_iMinH);
    setZValue(TOP_Z);
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
    painter->setRenderHints(QPainter::Antialiasing);

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
            painter->setPen(QPen(Qt::black, 3));
            painter->setBrush(Qt::NoBrush);
            int iTop = ROTATE_RECT - ROTATE_ARC;
            int iLeft = ROTATE_RECT - ROTATE_ARC;
            int iRight = m_rectBoundingBox.width() - ROTATE_RECT;
            int iBottom = m_rectBoundingBox.height() - ROTATE_RECT;
            painter->drawArc(iLeft, iTop, ROTATE_ARC, ROTATE_ARC, 180*16, -90*16);
            painter->drawArc(iRight, iTop, ROTATE_ARC, ROTATE_ARC, 90*16, -90*16);
            painter->drawArc(iLeft, iBottom, ROTATE_ARC, ROTATE_ARC, 270*16, -90*16);
            painter->drawArc(iRight, iBottom, ROTATE_ARC, ROTATE_ARC, 0*16, -90*16);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::Reset()
{
    m_eMode = mNormal;
    m_bReleased = false;
    Update();
    setZValue(TOP_Z);
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

    m_rectBoundingBox.setTopLeft(QPointF(0, 0));
    m_rectBoundingBox.setWidth(fW);
    m_rectBoundingBox.setHeight(fH);
    m_rectResize.setTopLeft(QPointF(fW - RESIZE_SQUARE, fH - RESIZE_SQUARE));
    m_rectResize.setWidth(RESIZE_SQUARE);
    m_rectResize.setHeight(RESIZE_SQUARE);
    setTransformOriginPoint(m_rectBoundingBox.center());
    prepareGeometryChange();
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::Update()
{
    UpdateFont();
    UpdateBox();
}

//---------------------------------------------------------------------------------------------------------------------
int VTextGraphicsItem::GetFontSize() const
{
    return m_font.pixelSize();
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
        m_ptStartPos = pos();
        m_ptStart = pME->scenePos();
        m_szStart = m_rectBoundingBox.size();
        m_ptRotCenter = mapToScene(m_rectBoundingBox.center());
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
                SetOverrideCursor(cursorArrowCloseHand, 1, 1);
            }
        }
        setZValue(TOP_Z + 1);
        UpdateBox();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* pME)
{
    QPointF ptDiff = pME->scenePos() - m_ptStart;
    if (m_eMode == mMove)
    {
        QPointF pt = m_ptStartPos + ptDiff;
        pt.setX(pt.x() + m_rectBoundingBox.width()/2);
        pt.setY(pt.y() + m_rectBoundingBox.height()/2);
        if (parentItem()->boundingRect().contains(pt) == true)
        {
            setPos(m_ptStartPos + ptDiff);
            UpdateBox();
        }
    }
    else if (m_eMode == mResize)
    {
        QPointF pt = m_ptStartPos;
        pt.setX(pt.x() + (m_szStart.width() + ptDiff.x())/2);
        pt.setY(pt.y() + (m_szStart.height() + ptDiff.y())/2);
        if (parentItem()->boundingRect().contains(pt) == true) {
            SetSize(m_szStart.width() + ptDiff.x(), m_szStart.height() + ptDiff.y());
            Update();
            emit SignalShrink();
        }
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
        //emit SignalRotated(rotation());
        Update();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VTextGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* pME)
{
    if (pME->button() == Qt::LeftButton)
    {
        RestoreOverrideCursor(cursorArrowCloseHand);
        double dDist = fabs(pME->scenePos().x() - m_ptStart.x()) + fabs(pME->scenePos().y() - m_ptStart.y());
        bool bShort = (dDist < 2);

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
                SetOverrideCursor(cursorArrowCloseHand, 1, 1);
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
    double dX = pt.x() - m_ptRotCenter.x();
    double dY = pt.y() - m_ptRotCenter.y();

    if (fabs(dX) < 1 && fabs(dY) < 1)
        return 0;
    else
        return atan2(dY, dX);
}
