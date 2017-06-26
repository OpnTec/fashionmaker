/************************************************************************
 **
 **  @file   vdxfpaintdevice.h
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   12 812, 2015
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

#ifndef VDXFPAINTDEVICE_H
#define VDXFPAINTDEVICE_H

#include <qcompilerdetection.h>
#include <QPaintDevice>
#include <QSize>
#include <QString>
#include <QtGlobal>

#include "dxfdef.h"
#include "libdxfrw/drw_base.h"

class VDxfEngine;

class VDxfPaintDevice : public QPaintDevice
{
public:
    VDxfPaintDevice();
    virtual ~VDxfPaintDevice() Q_DECL_OVERRIDE;
    virtual QPaintEngine *paintEngine() const Q_DECL_OVERRIDE;

    QString getFileName() const;
    void setFileName(const QString &value);

    QSize getSize();
    void setSize(const QSize &size);

    double getResolution() const;
    void   setResolution(double dpi);

    DRW::Version GetVersion() const;
    void         SetVersion(DRW::Version version);

    void SetBinaryFormat(bool binary);
    bool IsBinaryFromat() const;

    void setMeasurement(const VarMeasurement &var);
    void setInsunits(const VarInsunits &var);

protected:
    virtual int metric(PaintDeviceMetric metric) const Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VDxfPaintDevice)
    VDxfEngine *engine;
    QString     fileName;
};

#endif // VDXFPAINTDEVICE_H
