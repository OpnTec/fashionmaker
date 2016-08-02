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
#include <QCursor>
#include <QtMath>
#include <QDebug>

#include "../vmisc/def.h"
#include "vtextgraphicsitem.h"

#define RESIZE_SQUARE               30
#define ROTATE_CIRCLE               20
#define ROTATE_RECT                 60
#define ROTATE_ARC                  50
#define MIN_W                       120
#define MIN_H                       60
#define TOP_Z                       2

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::VTextGraphicsItem constructor
 * @param pParent pointer to the parent item
 */
VTextGraphicsItem::VTextGraphicsItem(QGraphicsItem* pParent)
    :QGraphicsObject(pParent), m_eMode(VTextGraphicsItem::mNormal), m_bReleased(false),
      m_ptStartPos(), m_ptStart(), m_ptRotCenter(), m_szStart(), m_dRotation(0), m_dAngle(0),
      m_rectResize(), m_iMinH(MIN_H), m_rectBoundingBox(), m_tm()
{
    m_rectBoundingBox.setTopLeft(QPointF(0, 0));
    SetSize(MIN_W, m_iMinH);
    setZValue(TOP_Z);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::~VTextGraphicsItem destructor
 */
VTextGraphicsItem::~VTextGraphicsItem()
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::SetFont sets the item font
 * @param fnt font to be used in item
 */
void VTextGraphicsItem::SetFont(const QFont& fnt)
{
    m_tm.SetFont(fnt);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::paint redraws the item content
 * @param painter pointer to the QPainter in use
 * @param option pointer to the object containing the actual label rectangle
 * @param widget not used
 */
void VTextGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->fillRect(option->rect, QColor(251, 251, 175));
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter->setPen(Qt::black);
    QFont fnt = m_tm.GetFont();
    // draw text lines
    int iY = 0;
    for (int i = 0; i < m_tm.GetCount(); ++i)
    {
        const TextLine& tl = m_tm.GetLine(i);
        fnt.setPixelSize(m_tm.GetFont().pixelSize() + tl.m_iFontSize);
        fnt.setWeight(tl.m_eFontWeight);
        fnt.setStyle(tl.m_eStyle);
        painter->setFont(fnt);
        painter->drawText(0, iY, qRound(boundingRect().width()), tl.m_iHeight, tl.m_eAlign, tl.m_qsText);
        iY += tl.m_iHeight + m_tm.GetSpacing();
    }

    // now draw the features specific to non-normal modes
    if (m_eMode != mNormal)
    {
        // outline the rectangle
        painter->setPen(QPen(Qt::black, 2, Qt::DashLine));
        painter->drawRect(boundingRect().adjusted(1, 1, -1, -1));

        if (m_eMode != mRotate)
        {
            // draw the resize square
            painter->setPen(Qt::black);
            painter->setBrush(Qt::black);
            painter->drawRect(m_rectResize);

            if (m_eMode == mResize)
            {
                // draw the resize diagonal lines
                painter->drawLine(0, 0, qRound(m_rectBoundingBox.width()), qRound(m_rectBoundingBox.height()));
                painter->drawLine(0, qRound(m_rectBoundingBox.height()), qRound(m_rectBoundingBox.width()), 0);
            }
        }
        else
        {
            // in rotate mode, draw the circle in the middle
            painter->setPen(Qt::black);
            painter->setBrush(Qt::black);
            painter->drawEllipse(
                        QPointF(m_rectBoundingBox.width()/2, m_rectBoundingBox.height()/2),
                        ROTATE_CIRCLE,
                        ROTATE_CIRCLE
                        );
            painter->setPen(QPen(Qt::black, 3));
            painter->setBrush(Qt::NoBrush);
            // and then draw the arc in each of the corners
            int iTop = ROTATE_RECT - ROTATE_ARC;
            int iLeft = ROTATE_RECT - ROTATE_ARC;
            int iRight = qRound(m_rectBoundingBox.width()) - ROTATE_RECT;
            int iBottom = qRound(m_rectBoundingBox.height()) - ROTATE_RECT;
            painter->drawArc(iLeft, iTop, ROTATE_ARC, ROTATE_ARC, 180*16, -90*16);
            painter->drawArc(iRight, iTop, ROTATE_ARC, ROTATE_ARC, 90*16, -90*16);
            painter->drawArc(iLeft, iBottom, ROTATE_ARC, ROTATE_ARC, 270*16, -90*16);
            painter->drawArc(iRight, iBottom, ROTATE_ARC, ROTATE_ARC, 0*16, -90*16);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::Reset resets the item, putting the mode and z coordinate to normal and redraws it
 */
void VTextGraphicsItem::Reset()
{
    m_eMode = mNormal;
    m_bReleased = false;
    Update();
    setZValue(TOP_Z);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::AddLine adds a line of text to the label list. If necessary, it also resizes the
 * label bounding box until it is big enough to contain all the text
 * @param tl line of text to add
 */
void VTextGraphicsItem::AddLine(const TextLine& tl)
{
    m_tm.AddLine(tl);
    while (m_tm.IsBigEnough(MIN_W, m_iMinH, MIN_FONT_SIZE) == false)
    {
        m_iMinH += 5;
    }
    if (m_rectBoundingBox.height() < m_iMinH)
    {
        SetSize(m_rectBoundingBox.width(), m_iMinH);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::Clear deletes all the label texts
 */
void VTextGraphicsItem::Clear()
{
    m_tm.Clear();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::SetSize Tries to set the label size to (fW, fH). If any of those is too small, the label
 * size does not change.
 * @param fW label width
 * @param fH label height
 */
void VTextGraphicsItem::SetSize(qreal fW, qreal fH)
{
    // don't allow resize under specific size
    if (fW < MIN_W || fH < m_iMinH)
    {
        return;
    }

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
/**
 * @brief VTextGraphicsItem::Update sets the correct font size and redraws the label
 */
void VTextGraphicsItem::Update()
{
    UpdateFont();
    UpdateBox();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::IsContained checks if the bounding box around rotated rectBB is contained in
 * the parent. If that is not the case, it calculates the amount of movement needed to put it inside the parent
 * and write it into dX, dY
 * @param rectBB bounding box in question
 * @param dRot bounding box rotation in degrees
 * @param dX horizontal translation needed to put the box inside parent item
 * @param dY vertical translation needed to put the box inside parent item
 * @return true, if rectBB is contained in parent item and false otherwise
 */
bool VTextGraphicsItem::IsContained(QRectF rectBB, qreal dRot, qreal &dX, qreal &dY) const
{
    QRectF rectParent = parentItem()->boundingRect();
    rectBB = GetBoundingRect(rectBB, dRot);
    dX = 0;
    dY = 0;

    if (rectParent.contains(rectBB) == false)
    {
        if (rectParent.left() - rectBB.left() > fabs(dX))
        {
            dX = rectParent.left() - rectBB.left();
        }
        else if (rectBB.right() - rectParent.right() > fabs(dX))
        {
            dX = rectParent.right() - rectBB.right();
        }

        if (rectParent.top() - rectBB.top() > fabs(dY))
        {
            dY = rectParent.top() - rectBB.top();
        }
        else if (rectBB.bottom() - rectParent.bottom() > fabs(dY))
        {
            dY = rectParent.bottom() - rectBB.bottom();
        }

        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::UpdateData Updates the detail label
 * @param qsName name of detail
 * @param data reference to VPatternPieceData
 */
void VTextGraphicsItem::UpdateData(const QString &qsName, const VPatternPieceData &data)
{
    m_tm.Update(qsName, data);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::UpdateData Updates the pattern label
 * @param pDoc pointer to the pattern object
 */
void VTextGraphicsItem::UpdateData(const VAbstractPattern* pDoc)
{
    m_tm.Update(pDoc);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::GetTextLines returns the number of lines of text to show
 * @return number of lines of text
 */
int VTextGraphicsItem::GetTextLines() const
{
    return m_tm.GetCount();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::GetFontSize returns the currently used text base font size
 * @return current text base font size
 */
int VTextGraphicsItem::GetFontSize() const
{
    return m_tm.GetFont().pixelSize();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::boundingRect returns the label bounding box
 * @return label bounding box
 */
QRectF VTextGraphicsItem::boundingRect() const
{
    return m_rectBoundingBox;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::mousePressEvent handles left button mouse press events
 * @param pME pointer to QGraphicsSceneMouseEvent object
 */
void VTextGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *pME)
{
    if (pME->button() == Qt::LeftButton)
    {
        // record the parameters of the mouse press. Specially record the position
        // of the press as the origin for the following operations
        m_ptStartPos = pos();
        m_ptStart = pME->scenePos();
        m_szStart = m_rectBoundingBox.size();
        m_ptRotCenter = mapToScene(m_rectBoundingBox.center());
        m_dAngle = GetAngle(pME->scenePos());
        m_dRotation = rotation();
        // in rotation mode, do not do any changes here, because user might want to
        // rotate the label more.
        if (m_eMode != mRotate)
        {
            // if user pressed the button inside the resize square, switch to resize mode
            if (m_rectResize.contains(pME->pos()) == true)
            {
                m_eMode = mResize;
                SetOverrideCursor(Qt::SizeFDiagCursor);
            }
            else
            {
                // if user pressed the button outside the resize square, switch to move mode
                m_eMode = mMove;
                SetOverrideCursor(cursorArrowCloseHand, 1, 1);
            }
        }
        // raise the label and redraw it
        setZValue(TOP_Z + 1);
        UpdateBox();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::mouseMoveEvent handles mouse move events
 * @param pME pointer to QGraphicsSceneMouseEvent object
 */
void VTextGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* pME)
{
    qreal dX;
    qreal dY;
    QRectF rectBB;
    QPointF ptDiff = pME->scenePos() - m_ptStart;
    if (m_eMode == mMove)
    {
        // in move mode move the label along the mouse move from the origin
        QPointF pt = m_ptStartPos + ptDiff;
        rectBB.setTopLeft(pt);
        rectBB.setWidth(m_rectBoundingBox.width());
        rectBB.setHeight(m_rectBoundingBox.height());
        // before moving label to a new position, check if it will still be inside the parent item
        if (IsContained(rectBB, rotation(), dX, dY) == false)
        {
            pt.setX(pt.x() + dX);
            pt.setY(pt.y() + dY);
        }
        setPos(pt);
        UpdateBox();
    }
    else if (m_eMode == mResize)
    {
        // in resize mode, resize the label along the mouse move from the origin
        QPointF pt = m_ptStartPos;
        rectBB.setTopLeft(pt);
        QSizeF sz(m_szStart.width() + ptDiff.x(), m_szStart.height() + ptDiff.y());
        rectBB.setSize(sz);
        // before resizing the label to a new size, check if it will still be inside the parent item
        if (IsContained(rectBB, rotation(), dX, dY) == true)
        {
            SetSize(sz.width(), sz.height());
            Update();
            emit SignalShrink();
        }
    }
    else if (m_eMode == mRotate)
    {
        // if the angle from the original position is small (0.5 degrees), just remeber the new angle
        // new angle will be the starting angle for rotation
        if (fabs(m_dAngle) < 0.01)
        {
            m_dAngle = GetAngle(pME->scenePos());
            return;
        }
        // calculate the angle difference from the starting angle
        double dAng =  qRadiansToDegrees(GetAngle(pME->scenePos()) - m_dAngle);
        rectBB.setTopLeft(m_ptStartPos);
        rectBB.setWidth(m_rectBoundingBox.width());
        rectBB.setHeight(m_rectBoundingBox.height());
        // check if the rotated label will be inside the parent item and then rotate it
        if (IsContained(rectBB, m_dRotation + dAng, dX, dY) == true)
        {
            setRotation(m_dRotation + dAng);
            Update();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::mouseReleaseEvent handles left button mouse release events
 * @param pME pointer to QGraphicsSceneMouseEvent object
 */
void VTextGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* pME)
{
    if (pME->button() == Qt::LeftButton)
    {
        // restore the cursor
        if (m_eMode == mMove)
        {
            RestoreOverrideCursor(cursorArrowCloseHand);
        }
        else if (m_eMode == mResize)
        {
            RestoreOverrideCursor(Qt::SizeFDiagCursor);
        }
        double dDist = fabs(pME->scenePos().x() - m_ptStart.x()) + fabs(pME->scenePos().y() - m_ptStart.y());
        // determine if this was just press/release (bShort == true) or user did some operation between press and release
        bool bShort = (dDist < 2);

        if (m_eMode == mMove || m_eMode == mResize)
        {   // if user just pressed and released the button, we must switch the mode to rotate
            // but if user did some operation (move/resize), emit the proper signal and update the label
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
                emit SignalResized(m_rectBoundingBox.width(), m_tm.GetFont().pixelSize());
                Update();
            }
        }
        else
        {   // in rotate mode, if user did just press/release, switch to move mode
            if (bShort == true)
            {
                m_eMode = mMove;
                UpdateBox();
            }
            else
            {
                // if user rotated the item, emit proper signal and update the label
                emit SignalRotated(rotation());
                UpdateBox();
            }
        }
        m_bReleased = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::UpdateBox redraws the label content
 */
void VTextGraphicsItem::UpdateBox()
{
    update(m_rectBoundingBox);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::UpdateFont sets the text font size, so that the entire text will
 *  just fit into the label bounding box
 */
void VTextGraphicsItem::UpdateFont()
{
    int iFS = m_tm.GetFont().pixelSize();

    // increase the font size until the bounding rect is not big enough
    while (iFS < MAX_FONT_SIZE && m_tm.IsBigEnough(m_rectBoundingBox.width(), m_rectBoundingBox.height(), iFS) == true)
    {
        ++iFS;
    }
    // decrease the font size until the bounding rect is big enough
    while (iFS >= MIN_FONT_SIZE && m_tm.IsBigEnough(m_rectBoundingBox.width(), m_rectBoundingBox.height(), iFS) == false)
    {
        --iFS;
    }
    m_tm.SetFontSize(iFS);
    UpdateBox();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::GetAngle calculates the angle between the line, which goes from
 * rotation center to pt and x axis
 * @param pt point of interest
 * @return the angle between line from rotation center and point of interest and x axis
 */
double VTextGraphicsItem::GetAngle(QPointF pt) const
{
    double dX = pt.x() - m_ptRotCenter.x();
    double dY = pt.y() - m_ptRotCenter.y();

    if (fabs(dX) < 1 && fabs(dY) < 1)
    {
        return 0;
    }
    else
    {
        return qAtan2(dY, dX);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VTextGraphicsItem::GetBoundingRect calculates the bounding box
 *  around rectBB rectangle, rotated around its center by dRot degrees
 * @param rectBB rectangle of interest
 * @param dRot rectangle rotation
 * @return bounding box around rectBB rotated by dRot
 */
QRectF VTextGraphicsItem::GetBoundingRect(QRectF rectBB, qreal dRot) const
{
    QPointF apt[4] = { rectBB.topLeft(), rectBB.topRight(), rectBB.bottomLeft(), rectBB.bottomRight() };
    QPointF ptCenter = rectBB.center();

    qreal dX1 = 0;
    qreal dX2 = 0;
    qreal dY1 = 0;
    qreal dY2 = 0;

     double dAng = qDegreesToRadians(dRot);
    for (int i = 0; i < 4; ++i)
    {
        QPointF pt = apt[i] - ptCenter;
        qreal dX = pt.x()*cos(dAng) + pt.y()*sin(dAng);
        qreal dY = -pt.x()*sin(dAng) + pt.y()*cos(dAng);

        if (i == 0)
        {
            dX1 = dX2 = dX;
            dY1 = dY2 = dY;
        }
        else
        {
            if (dX < dX1)
            {
                dX1 = dX;
            }
            else if (dX > dX2)
            {
                dX2 = dX;
            }
            if (dY < dY1)
            {
                dY1 = dY;
            }
            else if (dY > dY2)
            {
                dY2 = dY;
            }
        }
    }
    QRectF rect;
    rect.setTopLeft(ptCenter + QPointF(dX1, dY1));
    rect.setWidth(dX2 - dX1);
    rect.setHeight(dY2 - dY1);
    return rect;
}
