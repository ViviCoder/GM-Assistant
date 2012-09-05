/*************************************************************************
* Copyright © 2012 Vincent Prat & Simon Nicolas
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*************************************************************************/

#include "CharacterModification.h"

CharacterModification::CharacterModification(CharacterList &list, Character *character, int index, bool isAddition): Modification(isAddition?(Modification::aAddition):(Modification::aDeletion)), etEditType(etCharacter), iIndex(index), rList(list), pCharacter(character)
{
}

CharacterModification::~CharacterModification()
{
    if (pCharacter)
    {
        delete pCharacter;
    }
}

Modification::Type CharacterModification::type() const
{
    return tCharacter;
}

void CharacterModification::undo()
{
}

void CharacterModification::redo()
{
}
