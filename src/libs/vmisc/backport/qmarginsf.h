/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QMARGINSF_H
#define QMARGINSF_H

#include <qcompilerdetection.h>
#include <QTypeInfo>
#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
#include <QMargins>

/*****************************************************************************
  QMarginsF class
 *****************************************************************************/

class QMarginsF
{
public:
    Q_DECL_CONSTEXPR QMarginsF() Q_DECL_NOTHROW;
    Q_DECL_CONSTEXPR QMarginsF(qreal left, qreal top, qreal right, qreal bottom) Q_DECL_NOTHROW;
    Q_DECL_CONSTEXPR QMarginsF(const QMargins &margins) Q_DECL_NOTHROW;

    Q_DECL_CONSTEXPR bool isNull() const Q_DECL_NOTHROW;

    Q_DECL_CONSTEXPR qreal left() const Q_DECL_NOTHROW;
    Q_DECL_CONSTEXPR qreal top() const Q_DECL_NOTHROW;
    Q_DECL_CONSTEXPR qreal right() const Q_DECL_NOTHROW;
    Q_DECL_CONSTEXPR qreal bottom() const Q_DECL_NOTHROW;

    void setLeft(qreal left) Q_DECL_NOTHROW;
    void setTop(qreal top) Q_DECL_NOTHROW;
    void setRight(qreal right) Q_DECL_NOTHROW;
    void setBottom(qreal bottom) Q_DECL_NOTHROW;

    QMarginsF &operator+=(const QMarginsF &margins) Q_DECL_NOTHROW;
    QMarginsF &operator-=(const QMarginsF &margins) Q_DECL_NOTHROW;
    QMarginsF &operator+=(qreal addend) Q_DECL_NOTHROW;
    QMarginsF &operator-=(qreal subtrahend) Q_DECL_NOTHROW;
    QMarginsF &operator*=(qreal factor) Q_DECL_NOTHROW;
    QMarginsF &operator/=(qreal divisor);

    Q_DECL_CONSTEXPR inline QMargins toMargins() const Q_DECL_NOTHROW;

private:
    qreal m_left;
    qreal m_top;
    qreal m_right;
    qreal m_bottom;
};

Q_DECLARE_TYPEINFO(QMarginsF, Q_MOVABLE_TYPE);

/*****************************************************************************
  QMarginsF stream functions
 *****************************************************************************/

#ifndef QT_NO_DATASTREAM
Q_CORE_EXPORT QDataStream &operator<<(QDataStream &, const QMarginsF &);
Q_CORE_EXPORT QDataStream &operator>>(QDataStream &, QMarginsF &);
#endif

/*****************************************************************************
  QMarginsF inline functions
 *****************************************************************************/

Q_DECL_CONSTEXPR inline QMarginsF::QMarginsF() Q_DECL_NOTHROW
    : m_left(0), m_top(0), m_right(0), m_bottom(0) {}

Q_DECL_CONSTEXPR inline QMarginsF::QMarginsF(qreal aleft, qreal atop, qreal aright, qreal abottom) Q_DECL_NOTHROW
    : m_left(aleft), m_top(atop), m_right(aright), m_bottom(abottom) {}

Q_DECL_CONSTEXPR inline QMarginsF::QMarginsF(const QMargins &margins) Q_DECL_NOTHROW
    : m_left(margins.left()), m_top(margins.top()), m_right(margins.right()), m_bottom(margins.bottom()) {}

Q_DECL_CONSTEXPR inline bool QMarginsF::isNull() const Q_DECL_NOTHROW
{ return qFuzzyIsNull(m_left) && qFuzzyIsNull(m_top) && qFuzzyIsNull(m_right) && qFuzzyIsNull(m_bottom); }

Q_DECL_CONSTEXPR inline qreal QMarginsF::left() const Q_DECL_NOTHROW
{ return m_left; }

Q_DECL_CONSTEXPR inline qreal QMarginsF::top() const Q_DECL_NOTHROW
{ return m_top; }

Q_DECL_CONSTEXPR inline qreal QMarginsF::right() const Q_DECL_NOTHROW
{ return m_right; }

Q_DECL_CONSTEXPR inline qreal QMarginsF::bottom() const Q_DECL_NOTHROW
{ return m_bottom; }

inline void QMarginsF::setLeft(qreal aleft) Q_DECL_NOTHROW
{ m_left = aleft; }

inline void QMarginsF::setTop(qreal atop) Q_DECL_NOTHROW
{ m_top = atop; }

inline void QMarginsF::setRight(qreal aright) Q_DECL_NOTHROW
{ m_right = aright; }

inline void QMarginsF::setBottom(qreal abottom) Q_DECL_NOTHROW
{ m_bottom = abottom; }

Q_DECL_CONSTEXPR inline bool operator==(const QMarginsF &lhs, const QMarginsF &rhs) Q_DECL_NOTHROW
{
    return qFuzzyCompare(lhs.left(), rhs.left())
           && qFuzzyCompare(lhs.top(), rhs.top())
           && qFuzzyCompare(lhs.right(), rhs.right())
           && qFuzzyCompare(lhs.bottom(), rhs.bottom());
}

Q_DECL_CONSTEXPR inline bool operator!=(const QMarginsF &lhs, const QMarginsF &rhs) Q_DECL_NOTHROW
{
    return !operator==(lhs, rhs);
}

Q_DECL_CONSTEXPR inline QMarginsF operator+(const QMarginsF &lhs, const QMarginsF &rhs) Q_DECL_NOTHROW
{
    return QMarginsF(lhs.left() + rhs.left(), lhs.top() + rhs.top(),
                     lhs.right() + rhs.right(), lhs.bottom() + rhs.bottom());
}

Q_DECL_CONSTEXPR inline QMarginsF operator-(const QMarginsF &lhs, const QMarginsF &rhs) Q_DECL_NOTHROW
{
    return QMarginsF(lhs.left() - rhs.left(), lhs.top() - rhs.top(),
                     lhs.right() - rhs.right(), lhs.bottom() - rhs.bottom());
}

Q_DECL_CONSTEXPR inline QMarginsF operator+(const QMarginsF &lhs, qreal rhs) Q_DECL_NOTHROW
{
    return QMarginsF(lhs.left() + rhs, lhs.top() + rhs,
                     lhs.right() + rhs, lhs.bottom() + rhs);
}

Q_DECL_CONSTEXPR inline QMarginsF operator+(qreal lhs, const QMarginsF &rhs) Q_DECL_NOTHROW
{
    return QMarginsF(rhs.left() + lhs, rhs.top() + lhs,
                     rhs.right() + lhs, rhs.bottom() + lhs);
}

Q_DECL_CONSTEXPR inline QMarginsF operator-(const QMarginsF &lhs, qreal rhs) Q_DECL_NOTHROW
{
    return QMarginsF(lhs.left() - rhs, lhs.top() - rhs,
                     lhs.right() - rhs, lhs.bottom() - rhs);
}

Q_DECL_CONSTEXPR inline QMarginsF operator*(const QMarginsF &lhs, qreal rhs) Q_DECL_NOTHROW
{
    return QMarginsF(lhs.left() * rhs, lhs.top() * rhs,
                     lhs.right() * rhs, lhs.bottom() * rhs);
}

Q_DECL_CONSTEXPR inline QMarginsF operator*(qreal lhs, const QMarginsF &rhs) Q_DECL_NOTHROW
{
    return QMarginsF(rhs.left() * lhs, rhs.top() * lhs,
                     rhs.right() * lhs, rhs.bottom() * lhs);
}

Q_DECL_CONSTEXPR inline QMarginsF operator/(const QMarginsF &lhs, qreal divisor)
{
    return QMarginsF(lhs.left() / divisor, lhs.top() / divisor,
                     lhs.right() / divisor, lhs.bottom() / divisor);
}

inline QMarginsF &QMarginsF::operator+=(const QMarginsF &margins) Q_DECL_NOTHROW
{
    return *this = *this + margins;
}

inline QMarginsF &QMarginsF::operator-=(const QMarginsF &margins) Q_DECL_NOTHROW
{
    return *this = *this - margins;
}

inline QMarginsF &QMarginsF::operator+=(qreal addend) Q_DECL_NOTHROW
{
    m_left += addend;
    m_top += addend;
    m_right += addend;
    m_bottom += addend;
    return *this;
}

inline QMarginsF &QMarginsF::operator-=(qreal subtrahend) Q_DECL_NOTHROW
{
    m_left -= subtrahend;
    m_top -= subtrahend;
    m_right -= subtrahend;
    m_bottom -= subtrahend;
    return *this;
}

inline QMarginsF &QMarginsF::operator*=(qreal factor) Q_DECL_NOTHROW
{
    return *this = *this * factor;
}

inline QMarginsF &QMarginsF::operator/=(qreal divisor)
{
    return *this = *this / divisor;
}

Q_DECL_CONSTEXPR inline QMarginsF operator+(const QMarginsF &margins) Q_DECL_NOTHROW
{
    return margins;
}

Q_DECL_CONSTEXPR inline QMarginsF operator-(const QMarginsF &margins) Q_DECL_NOTHROW
{
    return QMarginsF(-margins.left(), -margins.top(), -margins.right(), -margins.bottom());
}

Q_DECL_CONSTEXPR inline QMargins QMarginsF::toMargins() const Q_DECL_NOTHROW
{
    return QMargins(qRound(m_left), qRound(m_top), qRound(m_right), qRound(m_bottom));
}

#ifndef QT_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug, const QMarginsF &);
#endif

#endif // QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
#endif // QMARGINSF_H
