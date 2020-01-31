/************************************************************************
 **
 **  @file   vwatermark.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 12, 2019
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2019 Valentina project
 **  <https://gitlab.com/smart-pattern/valentina> All Rights Reserved.
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
#ifndef VWATERMARK_H
#define VWATERMARK_H

#include "../ifc/xml/vdomdocument.h"


class VWatermark : public VDomDocument
{
    Q_DECLARE_TR_FUNCTIONS(VWatermark)
public:
    VWatermark();
    virtual ~VWatermark() Q_DECL_EQ_DEFAULT;

    void CreateEmptyWatermark();

    virtual bool SaveDocument(const QString &fileName, QString &error) override;

    VWatermarkData GetWatermark() const;
    void           SetWatermark(const VWatermarkData &data);

    static const QString TagWatermark;
    static const QString TagText;
    static const QString TagImage;

    static const QString AttrOpacity;
    static const QString AttrRotation;
    static const QString AttrFont;
    static const QString AttrPath;
    static const QString AttrGrayscale;
    static const QString AttrShow;

private:
    Q_DISABLE_COPY(VWatermark)
};

#endif // VWATERMARK_H
