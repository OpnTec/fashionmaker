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
 ** You should have received a copy of the GNU General Public License
 ** along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
**********************************************************************/

#include "dl_writer.h"

#include <ctype.h>
#include <algorithm>

#include "dxflib/dl_attributes.h"
#include "dxflib/dl_codes.h"

/**
 * @param version DXF version. Defaults to DL_VERSION_2002.
 */
DL_Writer::DL_Writer(DL_Codes::version version)
    : m_handle(0x30), modelSpaceHandle(0), paperSpaceHandle(0), paperSpace0Handle(0), version(version)
{}

DL_Writer::~DL_Writer()
{}

/** Generic section for section 'name'.
 *
 * <pre>
 *   0
 *  SECTION
 *   2
 *  name
 * </pre>
 */
void DL_Writer::section(const char *name) const
{
    dxfString(0, "SECTION");
    dxfString(2, name);
}

/**
 * Section HEADER
 *
 * <pre>
 *   0
 *  SECTION
 *   2
 *  HEADER
 * </pre>
 */
void DL_Writer::sectionHeader() const
{
    section("HEADER");
}

/**
 * Section TABLES
 *
 * <pre>
 *   0
 *  SECTION
 *   2
 *  TABLES
 * </pre>
 */
void DL_Writer::sectionTables() const
{
    section("TABLES");
}

/**
 * Section BLOCKS
 *
 * <pre>
 *   0
 *  SECTION
 *   2
 *  BLOCKS
 * </pre>
 */
void DL_Writer::sectionBlocks() const
{
    section("BLOCKS");
}

/**
 * Section ENTITIES
 *
 * <pre>
 *   0
 *  SECTION
 *   2
 *  ENTITIES
 * </pre>
 */
void DL_Writer::sectionEntities() const
{
    section("ENTITIES");
}

/**
 * Section CLASSES
 *
 * <pre>
 *   0
 *  SECTION
 *   2
 *  CLASSES
 * </pre>
 */
void DL_Writer::sectionClasses() const
{
    section("CLASSES");
}

/**
 * Section OBJECTS
 *
 * <pre>
 *   0
 *  SECTION
 *   2
 *  OBJECTS
 * </pre>
 */
void DL_Writer::sectionObjects() const
{
    section("OBJECTS");
}

/**
 * End of a section.
 *
 * <pre>
 *   0
 *  ENDSEC
 * </pre>
 */
void DL_Writer::sectionEnd() const
{
    dxfString(0, "ENDSEC");
}

/**
 * Generic table for table 'name' with 'num' entries:
 *
 * <pre>
 *   0
 *  TABLE
 *   2
 *  name
 *  70
 *   num
 * </pre>
 */
void DL_Writer::table(const char *name, int num, int h) const
{
    dxfString(0, "TABLE");
    dxfString(2, name);
    if (version>=DL_VERSION_2000)
    {
        if (h==0)
        {
            handle();
        }
        else
        {
            dxfHex(5, h);
        }
        dxfString(100, "AcDbSymbolTable");
    }
    dxfInt(70, num);
}

/** Table for layers.
 *
 * @param num Number of layers in total.
 *
 * <pre>
 *   0
 *  TABLE
 *   2
 *  LAYER
 *   70
 *      num
 * </pre>
 */
void DL_Writer::tableLayers(int num) const
{
    table("LAYER", num, 2);
}

/** Table for line types.
 *
 * @param num Number of line types in total.
 *
 * <pre>
 *   0
 *  TABLE
 *   2
 *  LTYPE
 *   70
 *      num
 * </pre>
 */
void DL_Writer::tableLinetypes(int num) const
{
    //linetypeHandle = 5;
    table("LTYPE", num, 5);
}

/** Table for application id.
 *
 * @param num Number of registered applications in total.
 *
 * <pre>
 *   0
 *  TABLE
 *   2
 *  APPID
 *   70
 *      num
 * </pre>
 */
void DL_Writer::tableAppid(int num) const
{
    table("APPID", num, 9);
}

/** Table for text style.
 *
 * @param num Number of text styles.
 *
 * <pre>
 *   0
 *  TABLE
 *   2
 *  STYLE
 *   70
 *      num
 * </pre>
 */
void DL_Writer::tableStyle(int num) const
{
    table("STYLE", num, 3);
}

/**
 * End of a table.
 *
 * <pre>
 *   0
 *  ENDTAB
 * </pre>
 */
void DL_Writer::tableEnd() const
{
    dxfString(0, "ENDTAB");
}

/**
 * End of the DXF file.
 *
 * <pre>
 *   0
 *  EOF
 * </pre>
 */
void DL_Writer::dxfEOF() const
{
    dxfString(0, "EOF");
}

/**
 * Comment.
 *
 * <pre>
 *  999
 *  text
 * </pre>
 */
void DL_Writer::comment(const char *text) const
{
    dxfString(999, text);
}

/**
 * Entity.
 *
 * <pre>
 *   0
 *  entTypeName
 * </pre>
 *
 */
void DL_Writer::entity(const char *entTypeName) const
{
    dxfString(0, entTypeName);
    if (version>=DL_VERSION_2000)
    {
        handle();
    }
}

/**
 * Attributes of an entity.
 *
 * <pre>
 *   8
 *  layer
 *  62
 *  color
 *  39
 *  width
 *   6
 *  linetype
 * </pre>
 */
void DL_Writer::entityAttributes(const DL_Attributes &attrib) const
{

    // layer name:
    dxfString(8, attrib.getLayer());

    // R12 doesn't accept BYLAYER values. The value has to be missing
    //   in that case.
    if (version>=DL_VERSION_2000 || attrib.getColor()!=256)
    {
        dxfInt(62, attrib.getColor());
    }
    if (version>=DL_VERSION_2000 && attrib.getColor24()!=-1)
    {
        dxfInt(420, attrib.getColor24());
    }
    if (version>=DL_VERSION_2000)
    {
        dxfInt(370, attrib.getWidth());
    }
    if (version>=DL_VERSION_2000) //-V581
    {
        dxfReal(48, attrib.getLinetypeScale());
    }
    std::string linetype = attrib.getLinetype();
    std::transform(linetype.begin(), linetype.end(), linetype.begin(), ::toupper);
    if (version>=DL_VERSION_2000 || linetype=="BYLAYER")
    {
        dxfString(6, attrib.getLinetype());
    }
}

/**
 * Subclass.
 */
void DL_Writer::subClass(const char *sub) const
{
    dxfString(100, sub);
}

/**
 * Layer (must be in the TABLES section LAYER).
 *
 * <pre>
 *   0
 *  LAYER
 * </pre>
 */
void DL_Writer::tableLayerEntry(unsigned long h) const
{
    dxfString(0, "LAYER");
    if (version>=DL_VERSION_2000)
    {
        if (h==0)
        {
            handle();
        }
        else
        {
            dxfHex(5, static_cast<int>(h));
        }
        dxfString(100, "AcDbSymbolTableRecord");
        dxfString(100, "AcDbLayerTableRecord");
    }
}

/**
 * Line type (must be in the TABLES section LTYPE).
 *
 * <pre>
 *   0
 *  LTYPE
 * </pre>
 */
void DL_Writer::tableLinetypeEntry(unsigned long h) const
{
    dxfString(0, "LTYPE");
    if (version>=DL_VERSION_2000)
    {
        if (h==0)
        {
            handle();
        }
        else
        {
            dxfHex(5, static_cast<int>(h));
        }
        //dxfHex(330, 0x5);
        dxfString(100, "AcDbSymbolTableRecord");
        dxfString(100, "AcDbLinetypeTableRecord");
    }
}

/**
 * Appid (must be in the TABLES section APPID).
 *
 * <pre>
 *   0
 *  APPID
 * </pre>
 */
void DL_Writer::tableAppidEntry(unsigned long h) const
{
    dxfString(0, "APPID");
    if (version>=DL_VERSION_2000)
    {
        if (h==0)
        {
            handle();
        }
        else
        {
            dxfHex(5, static_cast<int>(h));
        }
        //dxfHex(330, 0x9);
        dxfString(100, "AcDbSymbolTableRecord");
        dxfString(100, "AcDbRegAppTableRecord");
    }
}

/**
 * Block (must be in the section BLOCKS).
 *
 * <pre>
 *   0
 *  BLOCK
 * </pre>
 */
void DL_Writer::sectionBlockEntry(unsigned long h) const
{
    dxfString(0, "BLOCK");
    if (version>=DL_VERSION_2000)
    {
        if (h==0)
        {
            handle();
        }
        else
        {
            dxfHex(5, static_cast<int>(h));
        }
        //dxfHex(330, blockHandle);
        dxfString(100, "AcDbEntity");
        if (h==0x1C)
        {
            dxfInt(67, 1);
        }
        dxfString(8, "0");                 // TODO: Layer for block
        dxfString(100, "AcDbBlockBegin");
    }
}

/**
 * End of Block (must be in the section BLOCKS).
 *
 * <pre>
 *   0
 *  ENDBLK
 * </pre>
 */
void DL_Writer::sectionBlockEntryEnd(unsigned long h) const
{
    dxfString(0, "ENDBLK");
    if (version>=DL_VERSION_2000)
    {
        if (h==0)
        {
            handle();
        }
        else
        {
            dxfHex(5, static_cast<int>(h));
        }
        //dxfHex(330, blockHandle);
        dxfString(100, "AcDbEntity");
        if (h==0x1D)
        {
            dxfInt(67, 1);
        }
        dxfString(8, "0");                 // TODO: Layer for block
        dxfString(100, "AcDbBlockEnd");
    }
}

void DL_Writer::color(int col) const
{
    dxfInt(62, col);
}

void DL_Writer::linetype(const char *lt) const
{
    dxfString(6, lt);
}

void DL_Writer::linetypeScale(double scale) const
{
    dxfReal(48, scale);
}

void DL_Writer::lineWeight(int lw) const
{
    dxfInt(370, lw);
}

void DL_Writer::coord(int gc, double x, double y, double z) const
{
    dxfReal(gc, x);
    dxfReal(gc+10, y);
    dxfReal(gc+20, z);
}

void DL_Writer::coordTriplet(int gc, const double *value) const
{
    if (value)
    {
        dxfReal(gc, *value++);
        dxfReal(gc+10, *value++);
        dxfReal(gc+20, *value++);
    }
}

void DL_Writer::resetHandle() const
{
    m_handle = 1;
}

/**
 * Writes a unique handle and returns it.
 */
unsigned long DL_Writer::handle(int gc) const
{
    // handle has to be hex
    dxfHex(gc, static_cast<int>(m_handle));
    return m_handle++;
}

/**
 * @return Next handle that will be written.
 */
unsigned long DL_Writer::getNextHandle() const
{
    return m_handle;
}

/**
 * Increases handle, so that the handle returned remains available.
 */
unsigned long DL_Writer::incHandle() const
{
    return m_handle++;
}

/**
 * Sets the handle of the model space. Entities refer to
 * this handle.
 */
void DL_Writer::setModelSpaceHandle(unsigned long h) const
{
    modelSpaceHandle = h;
}

unsigned long DL_Writer::getModelSpaceHandle() const
{
    return modelSpaceHandle;
}

/**
 * Sets the handle of the paper space. Some special blocks refer to
 * this handle.
 */
void DL_Writer::setPaperSpaceHandle(unsigned long h) const
{
    paperSpaceHandle = h;
}

unsigned long DL_Writer::getPaperSpaceHandle() const
{
    return paperSpaceHandle;
}

/**
 * Sets the handle of the paper space 0. Some special blocks refer to
 * this handle.
 */
void DL_Writer::setPaperSpace0Handle(unsigned long h) const
{
    paperSpace0Handle = h;
}

/**
 * Sets the handle of the paper space 0. Some special blocks refer to
 * this handle.
 */
unsigned long DL_Writer::getPaperSpace0Handle() const
{
    return paperSpace0Handle;
}

/**
 * Can be overwritten by the implementing class to write a
 * bool value to the file.
 *
 * @param gc Group code.
 * @param value The bool value.
 */
void DL_Writer::dxfBool(int gc, bool value) const
{
    dxfInt(gc, static_cast<int>(value));
}
