/************************************************************************
 **
 **  @file   testpath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 8, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
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
#ifndef TESTPATH_H
#define TESTPATH_H

#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QTemporaryFile>
#include <QTextStream>

class QPointF;
class QJsonObject;
template <class T> class QVector;
class VSAPoint;

#if !defined(V_NO_ASSERT)
void VectorToJson(const QVector<QPointF> &points, QJsonObject &json);
void VectorToJson(const QVector<VSAPoint> &points, QJsonObject &json);

//---------------------------------------------------------------------------------------------------------------------
template <class T>
void DumpVector(const QVector<T> &points, const QString &templateName=QString())
{
    QTemporaryFile temp; // Go to tmp folder to find dump
    temp.setAutoRemove(false); // Remove dump manually

    if (not templateName.isEmpty())
    {
        temp.setFileTemplate(QDir::tempPath() + QDir::separator() + templateName);
    }

    if (temp.open())
    {
        QJsonObject vectorObject;
        VectorToJson(points, vectorObject);
        QJsonDocument vector(vectorObject);

        QTextStream out(&temp);
        out << vector.toJson();
        out.flush();
    }
}
#endif // !defined(V_NO_ASSERT)

#endif // TESTPATH_H
