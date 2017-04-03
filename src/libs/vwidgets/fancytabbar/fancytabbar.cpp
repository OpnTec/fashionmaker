/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "fancytabbar.h"
#include "stylehelper.h"

#include <QMouseEvent>
#include <QPainter>
#include <QColor>
#include <QStackedLayout>
#include <QToolTip>

const int FancyTabBar::m_rounding = 22;

//---------------------------------------------------------------------------------------------------------------------
FancyTabBar::FancyTabBar(const TabBarPosition position, QWidget *parent)
    : QWidget(parent),
      m_position(position),
      m_hoverRect(),
      m_hoverIndex(-1),
      m_currentIndex(-1),
      m_attachedTabs(),
      m_timerTriggerChangedSignal()
{
    if (m_position == FancyTabBar::Above || m_position == FancyTabBar::Below)
    {
        setMinimumHeight(qMax(2 * m_rounding, 40));
        setMaximumHeight(TabSizeHint(false).height());
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    }
    else
    {
        setMinimumWidth(qMax(2 * m_rounding, 40));
        setMaximumWidth(TabSizeHint(false).width());
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    }

    setAttribute(Qt::WA_Hover, true);
    setFocusPolicy(Qt::NoFocus);
    setMouseTracking(true); // Needed for hover events
    m_timerTriggerChangedSignal.setSingleShot(true);

    // We use a zerotimer to keep the sidebar responsive
    connect(&m_timerTriggerChangedSignal, &QTimer::timeout, this, &FancyTabBar::EmitCurrentIndex);
}

//---------------------------------------------------------------------------------------------------------------------
QSize FancyTabBar::TabSizeHint(bool minimum) const
{
    QFont boldFont(font());
    boldFont.setPointSizeF(StyleHelper::sidebarFontSize());
    boldFont.setBold(true);
    QFontMetrics fm(boldFont);
    int spacing = 8;
    int width = 60 + spacing + 2;
    int maxLabelwidth = 0;
    for (int tab=0 ; tab<Count() ;++tab)
    {
        int width = fm.width(TabText(tab));
        if (width > maxLabelwidth)
        {
            maxLabelwidth = width;
        }
    }
    int iconHeight = minimum ? 0 : 32;

    return QSize(qMax(width, maxLabelwidth + 4), iconHeight + spacing + fm.height());
}

//---------------------------------------------------------------------------------------------------------------------
QPoint FancyTabBar::GetCorner(const QRect& rect, const Corner corner) const
{
    switch(m_position)
    {
        case Above:
            switch(corner)
            {
                case OutsideBeginning:
                    return rect.topLeft();
                case OutsideEnd:
                    return rect.topRight();
                case InsideBeginning:
                    return rect.bottomLeft();
                case InsideEnd:
                    return rect.bottomRight();
                default:
                    break;
            }
            break;
        case Below:
            switch(corner)
            {
                case OutsideBeginning:
                    return rect.bottomLeft();
                case OutsideEnd:
                    return rect.bottomRight();
                case InsideBeginning:
                    return rect.topLeft();
                case InsideEnd:
                    return rect.topRight();
                default:
                    break;
            }
            break;
        case Left:
            switch(corner)
            {
                case OutsideBeginning:
                    return rect.topLeft();
                case OutsideEnd:
                    return rect.bottomLeft();
                case InsideBeginning:
                    return rect.topRight();
                case InsideEnd:
                    return rect.bottomRight();
                default:
                    break;
            }
            break;
        case Right:
            switch(corner)
            {
                case OutsideBeginning:
                    return rect.topRight();
                case OutsideEnd:
                    return rect.bottomRight();
                case InsideBeginning:
                    return rect.topLeft();
                case InsideEnd:
                    return rect.bottomLeft();
                default:
                    break;
            }
            break;
        default:
            break;
    }

    Q_ASSERT("that's impossible!");

    return QPoint();
}

//---------------------------------------------------------------------------------------------------------------------
// You can pass this method a QRect and tell it to move its edges to the outside (+)
// or inside (-) of the rect. For example, with a TabBar at the Above,
//
//      adjustRect(QRect(0,0,10,10), 1, 2, 3, -4)  // thats a 10 by 10 QRect, starting at 0/0
//
//  gives
//
//      QRect(-3, -1, 9, 13) // 9 by 13 rect, starting at -3/-1.
QRect FancyTabBar::AdjustRect(const QRect& rect, const qint8 offsetOutside, const qint8 offsetInside,
                              const qint8 offsetBeginning, const qint8 offsetEnd) const
{
    switch(m_position)
    {
        case Above:
            return rect.adjusted(-offsetBeginning, -offsetOutside, offsetEnd, offsetInside);
        case Below:
            return rect.adjusted(-offsetBeginning, -offsetInside, -offsetBeginning, offsetOutside);
        case Left:
            return rect.adjusted(-offsetOutside, -offsetBeginning, offsetInside, offsetEnd);
        case Right:
            return rect.adjusted(-offsetInside, -offsetBeginning, offsetOutside, offsetEnd);
        default:
            break;
    }

    Q_ASSERT("that's impossible!");
    return QRect();
}

//---------------------------------------------------------------------------------------------------------------------
// Same with a point: + means towards Outside/End, - means towards Inside/Beginning
QPoint FancyTabBar::AdjustPoint(const QPoint& point, const qint8 offsetInsideOutside,
                                const qint8 offsetBeginningEnd) const
{
    switch(m_position)
    {
        case Above:
            return point + QPoint(offsetBeginningEnd, -offsetInsideOutside);
        case Below:
            return point + QPoint(offsetBeginningEnd, offsetInsideOutside);
        case Left:
            return point + QPoint(-offsetInsideOutside, offsetBeginningEnd);
        case Right:
            return point + QPoint(offsetInsideOutside, offsetBeginningEnd);
        default:
            break;
    }

    Q_ASSERT("that's impossible!");
    return QPoint();
}

//---------------------------------------------------------------------------------------------------------------------
void FancyTabBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    // paint background
    QRect rectangle = AdjustRect(rect(), 0, -1, 0, 0);
    QLinearGradient lg;

    lg.setStart(GetCorner(rectangle, OutsideBeginning));
    lg.setFinalStop(GetCorner(rectangle, InsideBeginning));
    lg.setColorAt(0.0, QColor(64, 64, 64, 255));
    lg.setColorAt(1.0, QColor(130, 130, 130, 255));
    painter.fillRect(rectangle, lg);

    // draw dark widget bordert on inner inside (e.g. bottom if the widget position is top)
    painter.setPen(StyleHelper::borderColor());
    painter.drawLine(AdjustPoint(GetCorner(rectangle, InsideBeginning), -1, 0),
                     AdjustPoint(GetCorner(rectangle, InsideEnd), -1, 0));

    // draw bright widget border on outer inside (e.g. bottom if the widget position is top)
    painter.setPen(StyleHelper::sidebarHighlight());
    painter.drawLine(GetCorner(rectangle, InsideBeginning), GetCorner(rectangle, InsideEnd));

    // paint inactive tabs
    for (int i = 0; i < Count(); ++i)
    {
        if (i != CurrentIndex())
        {
            PaintTab(&painter, i);
        }
    }

    // paint active tab last, since it overlaps the neighbors
    if (CurrentIndex() != -1)
    {
        PaintTab(&painter, CurrentIndex());
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Handle hover events for mouse fade ins
void FancyTabBar::mouseMoveEvent(QMouseEvent *e)
{
    int newHover = -1;
    for (int i = 0; i < Count(); ++i)
    {
        QRect area = TabRect(i);
        if (area.contains(e->pos()))
        {
            newHover = i;
            break;
        }
    }
    if (newHover == m_hoverIndex)
    {
        return;
    }

    if (ValidIndex(m_hoverIndex))
    {
        m_attachedTabs[m_hoverIndex]->fadeOut();
    }

    m_hoverIndex = newHover;

    if (ValidIndex(m_hoverIndex))
    {
        m_attachedTabs[m_hoverIndex]->fadeIn();
        m_hoverRect = TabRect(m_hoverIndex);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool FancyTabBar::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip)
    {
        if (ValidIndex(m_hoverIndex))
        {
            QString tt = TabToolTip(m_hoverIndex);
            if (!tt.isEmpty())
            {
                QToolTip::showText(static_cast<QHelpEvent*>(event)->globalPos(), tt, this);
                return true;
            }
        }
    }
    return QWidget::event(event);
}

//---------------------------------------------------------------------------------------------------------------------
// Resets hover animation on mouse enter
void FancyTabBar::enterEvent(QEvent *e)
{
    Q_UNUSED(e)
    m_hoverRect = QRect();
    m_hoverIndex = -1;
}

//---------------------------------------------------------------------------------------------------------------------
// Resets hover animation on mouse leave
void FancyTabBar::leaveEvent(QEvent *e)
{
    Q_UNUSED(e)
    m_hoverIndex = -1;
    m_hoverRect = QRect();
    for (int i = 0 ; i < m_attachedTabs.count() ; ++i)
    {
        m_attachedTabs[i]->fadeOut();
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool FancyTabBar::ValidIndex(int index) const
{
    return index >= 0 && index < m_attachedTabs.count();
}

//---------------------------------------------------------------------------------------------------------------------
void FancyTabBar::SetOrientation(const FancyTabBar::TabBarPosition p)
{
    m_position = p;
}

//---------------------------------------------------------------------------------------------------------------------
QSize FancyTabBar::sizeHint() const
{
    QSize sh = TabSizeHint();
    //    return QSize(sh.width(), sh.height() * mAttachedTabs.count());

    if (m_position == Above || m_position == Below)
    {
        return QSize(sh.width() * m_attachedTabs.count(), sh.height());
    }
    else
    {
        return QSize(sh.width(), sh.height() * m_attachedTabs.count());
    }
}

//---------------------------------------------------------------------------------------------------------------------
QSize FancyTabBar::minimumSizeHint() const
{
    const QSize sh = TabSizeHint(true);
//    return QSize(sh.width(), sh.height() * mAttachedTabs.count());

    if(m_position == Above || m_position == Below)
    {
        return QSize(sh.width() * m_attachedTabs.count(), sh.height());
    }
    else
    {
        return QSize(sh.width(), sh.height() * m_attachedTabs.count());
    }
}

//---------------------------------------------------------------------------------------------------------------------
QRect FancyTabBar::TabRect(int index) const
{
    QSize sh = TabSizeHint();

    if(m_position == Above || m_position == Below)
    {
        if (sh.width() * m_attachedTabs.count() > width())
        {
            sh.setWidth(width() / m_attachedTabs.count());
        }

        return QRect(index * sh.width(), 0, sh.width(), sh.height());
    }
    else
    {
        if (sh.height() * m_attachedTabs.count() > height())
        {
            sh.setHeight(height() / m_attachedTabs.count());
        }

        return QRect(0, index * sh.height(), sh.width(), sh.height());
    }

}

//---------------------------------------------------------------------------------------------------------------------
// This keeps the sidebar responsive since
// we get a repaint before loading the
// mode itself
void FancyTabBar::EmitCurrentIndex()
{
    emit CurrentChanged(m_currentIndex);
}

//---------------------------------------------------------------------------------------------------------------------
void FancyTabBar::mousePressEvent(QMouseEvent *e)
{
    e->accept();
    for (int index = 0; index < m_attachedTabs.count(); ++index)
    {
        if (TabRect(index).contains(e->pos()))
        {
            if (IsTabEnabled(index))
            {
                m_currentIndex = index;
                update();
                m_timerTriggerChangedSignal.start(0);
            }
            break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void FancyTabBar::PaintTab(QPainter *painter, int tabIndex) const
{
    if (!ValidIndex(tabIndex))
    {
        qWarning("invalid index");
        return;
    }
    painter->save();

    QRect rect = TabRect(tabIndex);
    bool selected = (tabIndex == m_currentIndex);
    bool enabled = IsTabEnabled(tabIndex);

    if(selected)
    {
        // background
        painter->save();
        QLinearGradient grad(GetCorner(rect, OutsideBeginning), GetCorner(rect, InsideBeginning));
        grad.setColorAt(0, QColor(255, 255, 255, 140));
        grad.setColorAt(1, QColor(255, 255, 255, 210));
        painter->fillRect(AdjustRect(rect, 0, 0, 0, -1), grad);
        painter->restore();

        // shadows (the black lines immediately before/after (active && selected)-backgrounds)
        painter->setPen(QColor(0, 0, 0, 110));
        painter->drawLine(AdjustPoint(GetCorner(rect, OutsideBeginning), 0, -1),
                          AdjustPoint(GetCorner(rect, InsideBeginning), 0, -1));
        painter->drawLine(GetCorner(rect, OutsideEnd), GetCorner(rect, InsideEnd));

        // thin shadow on the outside of active tab
        painter->setPen(QColor(0, 0, 0, 40));
        painter->drawLine(GetCorner(rect, OutsideBeginning), GetCorner(rect, OutsideEnd));

        // highlights
        painter->setPen(QColor(255, 255, 255, 50));
        painter->drawLine(AdjustPoint(GetCorner(rect, OutsideBeginning), 0, -2),
                          AdjustPoint(GetCorner(rect, InsideBeginning), 0, -2));
        painter->drawLine(AdjustPoint(GetCorner(rect, OutsideEnd), 0, 1),
                          AdjustPoint(GetCorner(rect, InsideEnd), 0, 1));

        painter->setPen(QColor(255, 255, 255, 40));
        // thin white line towards beginning
        painter->drawLine(AdjustPoint(GetCorner(rect, OutsideBeginning), 0, 0),
                          AdjustPoint(GetCorner(rect, InsideBeginning), 0, 0));
        // thin white line on inside border
        painter->drawLine(AdjustPoint(GetCorner(rect, InsideBeginning), 0, 1),
                          AdjustPoint(GetCorner(rect, InsideEnd), 0, -1));
        // thin white line towards end
        painter->drawLine(AdjustPoint(GetCorner(rect, OutsideEnd), 0, -1),
                          AdjustPoint(GetCorner(rect, InsideEnd), 0, -1));
    }

    QString tabText(this->TabText(tabIndex));
    QRect tabTextRect(rect);
    const bool drawIcon = rect.height() > 36;
    QRect tabIconRect(tabTextRect);
    tabTextRect.translate(0, drawIcon ? -2 : 1);
    QFont boldFont(painter->font());
    boldFont.setPointSizeF(StyleHelper::sidebarFontSize());
    boldFont.setBold(true);
    painter->setFont(boldFont);
    painter->setPen(selected ? QColor(255, 255, 255, 160) : QColor(0, 0, 0, 110));
    const int textFlags = Qt::AlignCenter | (drawIcon ? Qt::AlignBottom : Qt::AlignVCenter) | Qt::TextWordWrap;
    if (enabled)
    {
        painter->drawText(tabTextRect, textFlags, tabText);
        painter->setPen(selected ? QColor(60, 60, 60) : StyleHelper::panelTextColor());
    }
    else
    {
        painter->setPen(selected ? StyleHelper::panelTextColor() : QColor(255, 255, 255, 120));
    }

#ifndef Q_OS_MAC
    // hover
    if(!selected && enabled)
    {
        painter->save();
        int fader = int(m_attachedTabs[tabIndex]->fader());
        QLinearGradient grad(GetCorner(rect, OutsideBeginning), GetCorner(rect, InsideBeginning));

        grad.setColorAt(0, Qt::transparent);
        grad.setColorAt(0.5, QColor(255, 255, 255, fader));
        grad.setColorAt(1, Qt::transparent);
        painter->fillRect(rect, grad);
        painter->setPen(QPen(grad, 1.0));

        if(m_position == Above || m_position == Below)
        {
            painter->drawLine(rect.topLeft(), rect.bottomLeft());
            painter->drawLine(rect.topRight(), rect.bottomRight());
        }
        else
        {
            painter->drawLine(rect.topLeft(), rect.topRight());
            painter->drawLine(rect.bottomLeft(), rect.bottomRight());
        }

        painter->restore();
    }
#endif //#ifndef Q_OS_MAC

    if (!enabled)
    {
        painter->setOpacity(0.7);
    }

    if (drawIcon)
    {
        int textHeight = painter->fontMetrics().boundingRect(QRect(0, 0, width(), height()), Qt::TextWordWrap,
                                                             tabText).height();
        tabIconRect.adjust(0, 0, 0, -textHeight);
        StyleHelper::drawIconWithShadow(TabIcon(tabIndex), tabIconRect, painter,
                                        enabled ? QIcon::Normal : QIcon::Disabled);
    }

    painter->translate(0, -1);
    painter->drawText(tabTextRect, textFlags, tabText);
    painter->restore();
}

//---------------------------------------------------------------------------------------------------------------------
void FancyTabBar::SetCurrentIndex(int index)
{
    if (IsTabEnabled(index))
    {
        m_currentIndex = index;
        update();
        emit CurrentChanged(m_currentIndex);
    }
}

//---------------------------------------------------------------------------------------------------------------------
int FancyTabBar::CurrentIndex() const
{
    return m_currentIndex;
}

//---------------------------------------------------------------------------------------------------------------------
void FancyTabBar::SetTabToolTip(int index, QString toolTip)
{
    m_attachedTabs[index]->m_toolTip = toolTip;
}

//---------------------------------------------------------------------------------------------------------------------
QString FancyTabBar::TabToolTip(int index) const
{
    return m_attachedTabs.at(index)->m_toolTip;
}

//---------------------------------------------------------------------------------------------------------------------
QIcon FancyTabBar::TabIcon(int index) const
{
    return m_attachedTabs.at(index)->m_icon;
}

//---------------------------------------------------------------------------------------------------------------------
QString FancyTabBar::TabText(int index) const
{
    return m_attachedTabs.at(index)->m_text;
}

//---------------------------------------------------------------------------------------------------------------------
void FancyTabBar::SetTabText(int index, QString text)
{
    m_attachedTabs.at(index)->m_text=text;
    update();
}

//---------------------------------------------------------------------------------------------------------------------
int FancyTabBar::Count() const
{
    return m_attachedTabs.count();
}

//---------------------------------------------------------------------------------------------------------------------
void FancyTabBar::SetTabEnabled(int index, bool enable)
{
    Q_ASSERT(index < m_attachedTabs.size());
    Q_ASSERT(index >= 0);

    if (index < m_attachedTabs.size() && index >= 0)
    {
        m_attachedTabs[index]->m_enabled = enable;
        update(TabRect(index));
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool FancyTabBar::IsTabEnabled(int index) const
{
    Q_ASSERT(index < m_attachedTabs.size());
    Q_ASSERT(index >= 0);

    if (index < m_attachedTabs.size() && index >= 0)
    {
        return m_attachedTabs[index]->m_enabled;
    }

    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void FancyTabBar::InsertTab(int index, const QIcon &icon, const QString &label)
{
    FancyTab *tab = new FancyTab(this);
    tab->m_icon = icon;
    tab->m_text = label;
    m_attachedTabs.insert(index, tab);
}

//---------------------------------------------------------------------------------------------------------------------
void FancyTabBar::RemoveTab(int index)
{
    FancyTab *tab = m_attachedTabs.takeAt(index);
    delete tab;
}
