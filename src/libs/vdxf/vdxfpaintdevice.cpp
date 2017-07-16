/************************************************************************
 **
 **  @file   vdxfpaintdevice.cpp
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

#include "vdxfpaintdevice.h"

#include <QMessageLogger>
#include <QtDebug>

#include "vdxfengine.h"

 //---------------------------------------------------------------------------------------------------------------------
VDxfPaintDevice::VDxfPaintDevice()
    :QPaintDevice(), engine(new VDxfEngine()), fileName()
{
}

 //---------------------------------------------------------------------------------------------------------------------
VDxfPaintDevice::~VDxfPaintDevice()
{
    delete engine;
}

 //---------------------------------------------------------------------------------------------------------------------
 // cppcheck-suppress unusedFunction
QPaintEngine *VDxfPaintDevice::paintEngine() const
{
    return engine;
}

 //---------------------------------------------------------------------------------------------------------------------
 // cppcheck-suppress unusedFunction
QString VDxfPaintDevice::getFileName() const
{
    return fileName;
}

 //---------------------------------------------------------------------------------------------------------------------
void VDxfPaintDevice::setFileName(const QString &value)
{
    if (engine->isActive())
    {
        qWarning("VDxfPaintDevice::setFileName(), cannot set file name while Dxf is being generated");
        return;
    }

    fileName = value;
    engine->setFileName(fileName);
}

 //---------------------------------------------------------------------------------------------------------------------
QSize VDxfPaintDevice::getSize()
{
    return engine->getSize();
}

 //---------------------------------------------------------------------------------------------------------------------
void VDxfPaintDevice::setSize(const QSize &size)
{
    if (engine->isActive())
    {
        qWarning("VDxfPaintDevice::setSize(), cannot set size while Dxf is being generated");
        return;
    }
    engine->setSize(size);
}

 //---------------------------------------------------------------------------------------------------------------------
double VDxfPaintDevice::getResolution() const
{
    return engine->getResolution();
}

 //---------------------------------------------------------------------------------------------------------------------
void VDxfPaintDevice::setResolution(double dpi)
{
    if (engine->isActive())
    {
        qWarning("VDxfPaintDevice::setResolution(), cannot set dpi while Dxf is being generated");
        return;
    }
    engine->setResolution(dpi);
}

//---------------------------------------------------------------------------------------------------------------------
DRW::Version VDxfPaintDevice::GetVersion() const
{
    return engine->GetVersion();
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfPaintDevice::SetVersion(DRW::Version version)
{
    if (engine->isActive())
    {
        qWarning("VDxfPaintDevice::SetVersion(), cannot set version while Dxf is being generated");
        return;
    }
    engine->SetVersion(version);
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfPaintDevice::SetBinaryFormat(bool binary)
{
    if (engine->isActive())
    {
        qWarning("VDxfPaintDevice::SetBinaryFormat(), cannot set binary format while Dxf is being generated");
        return;
    }
    engine->SetBinaryFormat(binary);
}

//---------------------------------------------------------------------------------------------------------------------
bool VDxfPaintDevice::IsBinaryFromat() const
{
    return engine->IsBinaryFormat();
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfPaintDevice::setMeasurement(const VarMeasurement &var)
{
    if (engine->isActive())
    {
        qWarning("VDxfPaintDevice::setMeasurement(), cannot set measurements while Dxf is being generated");
        return;
    }
    engine->setMeasurement(var);
}

//---------------------------------------------------------------------------------------------------------------------
void VDxfPaintDevice::setInsunits(const VarInsunits &var)
{
    if (engine->isActive())
    {
        qWarning("VDxfPaintDevice::setInsunits(), cannot set units while Dxf is being generated");
        return;
    }
    engine->setInsunits(var);
}

//---------------------------------------------------------------------------------------------------------------------
bool VDxfPaintDevice::ExportToAAMA(const QVector<VLayoutPiece> &details) const
{
    engine->setActive(true);
    const bool res = engine->ExportToAAMA(details);
    engine->setActive(false);
    return res;
}

 //---------------------------------------------------------------------------------------------------------------------
int VDxfPaintDevice::metric(QPaintDevice::PaintDeviceMetric metric) const
{
    switch (metric)
    {
        case QPaintDevice::PdmDepth:
            return 32;
        case QPaintDevice::PdmWidth:
            return engine->getSize().width();
        case QPaintDevice::PdmHeight:
            return engine->getSize().height();
        case QPaintDevice::PdmHeightMM:
            return qRound(engine->getSize().height() * 25.4 / engine->getResolution());
        case QPaintDevice::PdmWidthMM:
            return qRound(engine->getSize().width() * 25.4 / engine->getResolution());
        case QPaintDevice::PdmNumColors:
            return static_cast<int>(0xffffffff);
        case QPaintDevice::PdmPhysicalDpiX:
        case QPaintDevice::PdmPhysicalDpiY:
        case QPaintDevice::PdmDpiX:
        case QPaintDevice::PdmDpiY:
            return static_cast<int>(engine->getResolution());
        case QPaintDevice::PdmDevicePixelRatio:
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
        case QPaintDevice::PdmDevicePixelRatioScaled:
#endif
            return 1;
        default:
            qWarning("VDxfPaintDevice::metric(), unhandled metric %d\n", metric);
            break;
    }
    return 0;
}
