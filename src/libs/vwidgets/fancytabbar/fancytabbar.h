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

#ifndef FANCYTABWIDGET_H
#define FANCYTABWIDGET_H

#include <QIcon>
#include <QWidget>
#include <QTimer>
#include <QPropertyAnimation>

class QPainter;

#include "fancytab.h"

class FancyTabBar : public QWidget
{
    Q_OBJECT

public:
    enum TabBarPosition
    {
        Above,
        Below,
        Left,
        Right
    };

    FancyTabBar(const TabBarPosition position, QWidget *parent = nullptr);
    virtual ~FancyTabBar();

    virtual QSize sizeHint() const Q_DECL_OVERRIDE;
    virtual QSize minimumSizeHint() const Q_DECL_OVERRIDE;

    void SetOrientation(const TabBarPosition p);

    void SetTabEnabled(int index, bool enable);
    bool IsTabEnabled(int index) const;

    void InsertTab(int index, const QIcon &icon, const QString &label);
    void RemoveTab(int index);

    void SetCurrentIndex(int index);
    int  CurrentIndex() const;

    void    SetTabToolTip(int index, QString toolTip);
    QString TabToolTip(int index) const;

    QIcon TabIcon(int index) const;

    QString TabText(int index) const;
    void    SetTabText(int index, QString text);

    int   Count() const;
    QRect TabRect(int index) const;

signals:
    void CurrentChanged(int);

protected:
    virtual bool event(QEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    virtual void enterEvent(QEvent *) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *) Q_DECL_OVERRIDE;

private slots:
    void EmitCurrentIndex();

private:
    Q_DISABLE_COPY(FancyTabBar)

    enum Corner { OutsideBeginning, OutsideEnd, InsideBeginning, InsideEnd };

    static const int m_rounding;

    TabBarPosition   m_position;
    QRect            m_hoverRect;
    int              m_hoverIndex;
    int              m_currentIndex;
    QList<FancyTab*> m_attachedTabs;
    QTimer           m_timerTriggerChangedSignal;

    QPoint GetCorner(const QRect& rect, const Corner corner) const;

    QRect AdjustRect(const QRect& rect, const qint8 offsetOutside, const qint8 offsetInside, const qint8 offsetStart,
                     const qint8 offsetEnd) const;

    // Same with a point. + means towards Outside/End, - means towards Inside/Beginning
    QPoint AdjustPoint(const QPoint& point, const qint8 offsetInsideOutside, const qint8 offsetBeginningEnd) const;

    QSize TabSizeHint(bool minimum = false) const;
    void  PaintTab(QPainter *painter, int tabIndex) const;
    bool  ValidIndex(int index) const;
};

#endif // FANCYTABWIDGET_H
