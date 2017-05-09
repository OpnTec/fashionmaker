/****************************************************************************
** Copyright (C) 2015 Roman Telezhynskyi <dismine(at)gmail.com>
** Copyright (C) 2001-2013 RibbonSoft, GmbH. All rights reserved.
** Copyright (C) 2001 Robert J. Campbell Jr.
**
** This file is part of the dxflib project.
**
** This file is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** Licensees holding valid dxflib Professional Edition licenses may use
** this file in accordance with the dxflib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef DL_WRITER_H
#define DL_WRITER_H

#include "dl_global.h"

#ifndef _WIN32
#include <strings.h>
#endif

#if defined(Q_CC_MSVC)
    #if (_MSC_VER > 1000)
    #pragma once
    #endif // _MSC_VER > 1000
#endif // Q_CC_MSVC

#include <algorithm>
#include <iostream>
#include <string>

#include "dl_attributes.h"
#include "dl_codes.h"

/**
 * Defines interface for writing low level DXF constructs to
 * a file. Implementation is defined in derived classes that write
 * to binary or ASCII files.
 *
 * Implements functions that write higher level constructs in terms of
 * the low level ones.
 *
 * @todo Add error checking for string/entry length.
 */
class DXFLIB_EXPORT DL_Writer
{
public:
    explicit DL_Writer(DL_Codes::version version);
    virtual ~DL_Writer();

    void section(const char* name) const;
    void sectionHeader() const;
    void sectionTables() const;
    void sectionBlocks() const;
    void sectionEntities() const;
    void sectionClasses() const;
    void sectionObjects() const;
    void sectionEnd() const;
    void table(const char* name, int num, int h=0) const;
    void tableLayers(int num) const;
    void tableLinetypes(int num) const;
    void tableAppid(int num) const;
    void tableStyle(int num) const;
    void tableEnd() const;
    void dxfEOF() const;
    void comment(const char* text) const;
    void entity(const char* entTypeName) const;
    void entityAttributes(const DL_Attributes& attrib) const;
    void subClass(const char* sub) const;
    void tableLayerEntry(unsigned long int h=0)  const;
    void tableLinetypeEntry(unsigned long int h=0)  const;
    void tableAppidEntry(unsigned long int h=0)  const;
    void sectionBlockEntry(unsigned long int h=0)  const;
    void sectionBlockEntryEnd(unsigned long int h=0)  const;
    void color(int col=256) const;
    void linetype(const char *lt) const;
    void linetypeScale(double scale) const;
    void lineWeight(int lw) const;
    void coord(int gc, double x, double y, double z=0) const;
    void coordTriplet(int gc, const double* value) const;
    void resetHandle() const;

    unsigned long handle(int gc=5) const;
    unsigned long getNextHandle() const;
    unsigned long incHandle() const;

    void setModelSpaceHandle(unsigned long h) const;
    unsigned long getModelSpaceHandle() const;

    void setPaperSpaceHandle(unsigned long h) const;
    unsigned long getPaperSpaceHandle() const;

    void setPaperSpace0Handle(unsigned long h) const;
    unsigned long getPaperSpace0Handle() const;

    /**
     * Must be overwritten by the implementing class to write a
     * real value to the file.
     *
     * @param gc Group code.
     * @param value The real value.
     */
    virtual void dxfReal(int gc, double value) const = 0;

    /**
     * Must be overwritten by the implementing class to write an
     * int value to the file.
     *
     * @param gc Group code.
     * @param value The int value.
     */
    virtual void dxfInt(int gc, int value) const = 0;


    virtual void dxfBool(int gc, bool value) const;

    /**
     * Must be overwritten by the implementing class to write an
     * int value (hex) to the file.
     *
     * @param gc Group code.
     * @param value The int value.
     */
    virtual void dxfHex(int gc, int value) const = 0;

    /**
     * Must be overwritten by the implementing class to write a
     * string to the file.
     *
     * @param gc Group code.
     * @param value The string.
     */
    virtual void dxfString(int gc, const char* value) const = 0;

    /**
     * Must be overwritten by the implementing class to write a
     * string to the file.
     *
     * @param gc Group code.
     * @param value The string.
     */
    virtual void dxfString(int gc, const std::string& value) const = 0;

protected:
    mutable unsigned long m_handle;
    mutable unsigned long modelSpaceHandle;
    mutable unsigned long paperSpaceHandle;
    mutable unsigned long paperSpace0Handle;

    /**
     * DXF version to be created.
     */
    DL_Codes::version version;
};

#endif
