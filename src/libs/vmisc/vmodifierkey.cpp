/************************************************************************
 **
 **  @file   vmodifierkey.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 2, 2019
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
#include "vmodifierkey.h"

#include <QKeySequence>

//---------------------------------------------------------------------------------------------------------------------
VModifierKey::VModifierKey(int key)
 : m_nativeText(QKeySequence(static_cast<int>(key)).toString(QKeySequence::NativeText))
{
    if (m_nativeText.endsWith(QLatin1Char('+')))
    {
        m_nativeText.chop(1);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VModifierKey::VModifierKey(Qt::KeyboardModifiers keys)
 : VModifierKey(static_cast<int>(keys))
{
    // nothing else
}

//---------------------------------------------------------------------------------------------------------------------
VModifierKey::VModifierKey(Qt::Key key)
 : VModifierKey(static_cast<int>(key))
{
    // nothing else
}

//---------------------------------------------------------------------------------------------------------------------
const VModifierKey& VModifierKey::Alt()
{
    static const VModifierKey key(Qt::AltModifier);
    return key;
}

//---------------------------------------------------------------------------------------------------------------------
const VModifierKey& VModifierKey::Control()
{
    static const VModifierKey key(Qt::ControlModifier);
    return key;
}

//---------------------------------------------------------------------------------------------------------------------
const VModifierKey& VModifierKey::ControlShift()
{
    static const VModifierKey key(Qt::ControlModifier | Qt::ShiftModifier);
    return key;
}

//---------------------------------------------------------------------------------------------------------------------
const VModifierKey& VModifierKey::Meta()
{
    static const VModifierKey key(Qt::MetaModifier);
    return key;
}

//---------------------------------------------------------------------------------------------------------------------
const VModifierKey& VModifierKey::Shift()
{
    static const VModifierKey key(Qt::ShiftModifier);
    return key;
}

//---------------------------------------------------------------------------------------------------------------------
const VModifierKey& VModifierKey::Space()
{
    static const VModifierKey key(Qt::Key_Space);
    return key;
}

//---------------------------------------------------------------------------------------------------------------------
const VModifierKey& VModifierKey::ReturnKey()
{
    static const VModifierKey key(Qt::Key_Return);
    return key;
}

//---------------------------------------------------------------------------------------------------------------------
const VModifierKey &VModifierKey::EnterKey()
{
    static const VModifierKey key(Qt::Key_Enter);
    return key;
}

//---------------------------------------------------------------------------------------------------------------------
const VModifierKey& VModifierKey::Backspace()
{
    static const VModifierKey key(Qt::Key_Backspace);
    return key;
}

//---------------------------------------------------------------------------------------------------------------------
const VModifierKey& VModifierKey::Escape()
{
    static const VModifierKey key(Qt::Key_Escape);
    return key;
}
