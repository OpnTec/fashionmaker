/***************************************************************************************************
 **
 **  Copyright (c) 2012 Linas Valiukas and others.
 **
 **  Permission is hereby granted, free of charge, to any person obtaining a copy of this
 **  software and associated documentation files (the "Software"), to deal in the Software
 **  without restriction, including without limitation the rights to use, copy, modify,
 **  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 **  permit persons to whom the Software is furnished to do so, subject to the following conditions:
 **
 **  The above copyright notice and this permission notice shall be included in all copies or
 **  substantial portions of the Software.
 **
 **  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 **  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 **  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 **  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 **  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **
 ******************************************************************************************************/

#include "fvavailableupdate.h"

//---------------------------------------------------------------------------------------------------------------------
FvAvailableUpdate::FvAvailableUpdate(QObject *parent)
    : QObject(parent),
      m_enclosureUrl(),
      m_enclosureVersion(),
      m_enclosurePlatform()
{
    // noop
}

//---------------------------------------------------------------------------------------------------------------------
QUrl FvAvailableUpdate::GetEnclosureUrl() const
{
    return m_enclosureUrl;
}

//---------------------------------------------------------------------------------------------------------------------
void FvAvailableUpdate::SetEnclosureUrl(const QUrl &enclosureUrl)
{
    m_enclosureUrl = enclosureUrl;
}

//---------------------------------------------------------------------------------------------------------------------
void FvAvailableUpdate::SetEnclosureUrl(const QString &enclosureUrl)
{
    SetEnclosureUrl(QUrl(enclosureUrl));
}

//---------------------------------------------------------------------------------------------------------------------
QString FvAvailableUpdate::GetEnclosureVersion() const
{
    return m_enclosureVersion;
}

//---------------------------------------------------------------------------------------------------------------------
void FvAvailableUpdate::SetEnclosureVersion(const QString &enclosureVersion)
{
    m_enclosureVersion = enclosureVersion;
}

//---------------------------------------------------------------------------------------------------------------------
QString FvAvailableUpdate::GetEnclosurePlatform() const
{
    return m_enclosurePlatform;
}

//---------------------------------------------------------------------------------------------------------------------
void FvAvailableUpdate::SetEnclosurePlatform(const QString &enclosurePlatform)
{
    m_enclosurePlatform = enclosurePlatform;
}
