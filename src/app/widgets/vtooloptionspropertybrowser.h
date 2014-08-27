/************************************************************************
 **
 **  @file   vtooloptionspropertybrowser.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef VTOOLOPTIONSPROPERTYBROWSER_H
#define VTOOLOPTIONSPROPERTYBROWSER_H

#include <QObject>
#include <QMap>

#include "../libs/vpropertyexplorer/vproperty.h"
#include "../libs/vpropertyexplorer/vpropertymodel.h"
#include "../libs/vpropertyexplorer/vpropertyformview.h"

class QDockWidget;
class QGraphicsItem;
class QScrollArea;

using namespace VPE;

class VToolOptionsPropertyBrowser : public QObject
{
    Q_OBJECT
public:
    VToolOptionsPropertyBrowser(QDockWidget *parent);
public slots:
    void itemClicked(QGraphicsItem *item);
    void userChangedData(VProperty* property);
    void UpdateOptions();
private:
    Q_DISABLE_COPY(VToolOptionsPropertyBrowser)

    VPropertyModel* PropertyModel;
    VPropertyFormView* TreeView;

    QGraphicsItem *currentItem;
    QMap<VProperty *, QString> propertyToId;
    QMap<QString, VProperty *> idToProperty;

    void AddProperty(VProperty *property, const QString &id);
    void ShowItemOptions(QGraphicsItem *item);
};

#endif // VTOOLOPTIONSPROPERTYBROWSER_H
