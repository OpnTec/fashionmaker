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

#include "qmarginsf.h"

#include <QDebugStateSaver>
#if QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
#include <QDataStream>
#include <QtDebug>

/*!
    \class QMarginsF
    \inmodule QtCore
    \ingroup painting
    \since 5.3

    \brief The QMarginsF class defines the four margins of a rectangle.

    QMarginsF defines a set of four margins; left, top, right and bottom,
    that describe the size of the borders surrounding a rectangle.

    The isNull() function returns \c true only if all margins are set to zero.

    QMarginsF objects can be streamed as well as compared.
*/


/*****************************************************************************
  QMarginsF member functions
 *****************************************************************************/

/*!
    \fn QMarginsF::QMarginsF()

    Constructs a margins object with all margins set to 0.

    \sa isNull()
*/

/*!
    \fn QMarginsF::QMarginsF(qreal left, qreal top, qreal right, qreal bottom)

    Constructs margins with the given \a left, \a top, \a right, \a bottom

    \sa setLeft(), setRight(), setTop(), setBottom()
*/

/*!
    \fn QMarginsF::QMarginsF(const QMargins &margins)

    Constructs margins copied from the given \a margins
*/

/*!
    \fn bool QMarginsF::isNull() const

    Returns \c true if all margins are 0; otherwise returns
    false.
*/


/*!
    \fn qreal QMarginsF::left() const

    Returns the left margin.

    \sa setLeft()
*/

/*!
    \fn qreal QMarginsF::top() const

    Returns the top margin.

    \sa setTop()
*/

/*!
    \fn qreal QMarginsF::right() const

    Returns the right margin.
*/

/*!
    \fn qreal QMarginsF::bottom() const

    Returns the bottom margin.
*/


/*!
    \fn void QMarginsF::setLeft(qreal left)

    Sets the left margin to \a left.
*/

/*!
    \fn void QMarginsF::setTop(qreal Top)

    Sets the Top margin to \a Top.
*/

/*!
    \fn void QMarginsF::setRight(qreal right)

    Sets the right margin to \a right.
*/

/*!
    \fn void QMarginsF::setBottom(qreal bottom)

    Sets the bottom margin to \a bottom.
*/

/*!
    \fn bool operator==(const QMarginsF &lhs, const QMarginsF &rhs)
    \relates QMarginsF

    Returns \c true if \a lhs and \a rhs are equal; otherwise returns \c false.
*/

/*!
    \fn bool operator!=(const QMarginsF &lhs, const QMarginsF &rhs)
    \relates QMarginsF

    Returns \c true if \a lhs and \a rhs are different; otherwise returns \c false.
*/

/*!
    \fn const QMarginsF operator+(const QMarginsF &lhs, const QMarginsF &rhs)
    \relates QMarginsF

    Returns a QMarginsF object that is the sum of the given margins, \a lhs
    and \a rhs; each component is added separately.

    \sa QMarginsF::operator+=(), QMarginsF::operator-=()
*/

/*!
    \fn const QMarginsF operator-(const QMarginsF &lhs, const QMarginsF &rhs)
    \relates QMarginsF

    Returns a QMarginsF object that is formed by subtracting \a rhs from
    \a lhs; each component is subtracted separately.

    \sa QMarginsF::operator+=(), QMarginsF::operator-=()
*/

/*!
    \fn const QMarginsF operator+(const QMarginsF &lhs, qreal rhs)
    \relates QMarginsF

    Returns a QMarginsF object that is formed by adding \a rhs to
    \a lhs.

    \sa QMarginsF::operator+=(), QMarginsF::operator-=()
*/

/*!
    \fn const QMarginsF operator+(qreal lhs, const QMarginsF &rhs)
    \relates QMarginsF

    Returns a QMarginsF object that is formed by adding \a lhs to
    \a rhs.

    \sa QMarginsF::operator+=(), QMarginsF::operator-=()
*/

/*!
    \fn const QMarginsF operator-(const QMarginsF &lhs, qreal rhs)
    \relates QMarginsF

    Returns a QMarginsF object that is formed by subtracting \a rhs from
    \a lhs.

    \sa QMarginsF::operator+=(), QMarginsF::operator-=()
*/

/*!
    \fn const QMarginsF operator*(const QMarginsF &lhs, qreal rhs)
    \relates QMarginsF
    \overload

    Returns a QMarginsF object that is formed by multiplying each component
    of the given \a lhs margins by \a rhs factor.

    \sa QMarginsF::operator*=(), QMarginsF::operator/=()
*/

/*!
    \fn const QMarginsF operator*(qreal lhs, const QMarginsF &rhs)
    \relates QMarginsF
    \overload

    Returns a QMarginsF object that is formed by multiplying each component
    of the given \a lhs margins by \a rhs factor.

    \sa QMarginsF::operator*=(), QMarginsF::operator/=()
*/

/*!
    \fn const QMarginsF operator/(const QMarginsF &lhs, qreal rhs)
    \relates QMarginsF
    \overload

    Returns a QMarginsF object that is formed by dividing the components of
    the given \a lhs margins by the given \a rhs divisor.

    \sa QMarginsF::operator*=(), QMarginsF::operator/=()
*/

/*!
    \fn QMarginsF operator+(const QMarginsF &margins)
    \relates QMarginsF

    Returns a QMargin object that is formed from all components of \a margins.
*/

/*!
    \fn QMarginsF operator-(const QMarginsF &margins)
    \relates QMarginsF

    Returns a QMargin object that is formed by negating all components of \a margins.
*/

/*!
    \fn QMarginsF &QMarginsF::operator+=(const QMarginsF &margins)

    Add each component of \a margins to the respective component of this object
    and returns a reference to it.

    \sa operator-=()
*/

/*!
    \fn QMarginsF &QMarginsF::operator-=(const QMarginsF &margins)

    Subtract each component of \a margins from the respective component of this object
    and returns a reference to it.

    \sa operator+=()
*/

/*!
    \fn QMarginsF &QMarginsF::operator+=(qreal addend)
    \overload

    Adds the \a addend to each component of this object
    and returns a reference to it.

    \sa operator-=()
*/

/*!
    \fn QMarginsF &QMarginsF::operator-=(qreal subtrahend)
    \overload

    Subtracts the \a subtrahend from each component of this object
    and returns a reference to it.

    \sa operator+=()
*/

/*!
    \fn QMarginsF &QMarginsF::operator*=(qreal factor)

    Multiplies each component of this object by \a factor
    and returns a reference to it.

    \sa operator/=()
*/

/*!
    \fn QMarginsF &QMarginsF::operator/=(qreal divisor)

    Divides each component of this object by \a divisor
    and returns a reference to it.

    \sa operator*=()
*/

/*!
    \fn QMargins QMarginsF::toMargins() const

    Returns an integer based copy of this margins object.

    Note that the components in the returned margins will be rounded to
    the nearest integer.

    \sa QMarginsF()
*/

/*****************************************************************************
  QMarginsF stream functions
 *****************************************************************************/
#ifndef QT_NO_DATASTREAM
/*!
    \fn QDataStream &operator<<(QDataStream &stream, const QMarginsF &m)
    \relates QMarginsF

    Writes margin \a m to the given \a stream and returns a
    reference to the stream.

    \sa {Serializing Qt Data Types}
*/

QDataStream &operator<<(QDataStream &s, const QMarginsF &m)
{
    s << double(m.left()) << double(m.top()) << double(m.right()) << double(m.bottom());
    return s;
}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, QMarginsF &m)
    \relates QMarginsF

    Reads a margin from the given \a stream into margin \a m
    and returns a reference to the stream.

    \sa {Serializing Qt Data Types}
*/

QDataStream &operator>>(QDataStream &s, QMarginsF &m)
{
    double left, top, right, bottom;
    s >> left;
    s >> top;
    s >> right;
    s >> bottom;
    m = QMarginsF(qreal(left), qreal(top), qreal(right), qreal(bottom));
    return s;
}
#endif // QT_NO_DATASTREAM

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QMarginsF &m)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
    QDebugStateSaver saver(dbg);
#endif // QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
    dbg.nospace();
    dbg << "QMarginsF" << '(';
    dbg << m.left() << ", " << m.top() << ", " << m.right() << ", " << m.bottom();
    dbg << ')';
    return dbg;
}
#endif
#endif // QT_VERSION < QT_VERSION_CHECK(5, 3, 0)
