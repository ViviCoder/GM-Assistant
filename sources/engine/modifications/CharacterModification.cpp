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

using namespace std;

CharacterModification::CharacterModification(CharacterList *list, Character *character, int index, bool isAddition): Modification(isAddition?(Modification::aAddition):(Modification::aDeletion)), etEditType(etCharacter), iIndex(index), pCharacterList(list), pSkillList(0), pCharacter(character)
{
}

CharacterModification::CharacterModification(SkillList *list, const string &skill, int index): Modification(Modification::aAddition), etEditType(etSkill), iIndex(index), pCharacterList(0), pSkillList(list), pCharacter(0), sSkill(skill)
{
}

CharacterModification::CharacterModification(SkillList *skillList, const string &skill, CharacterList *characterList, const vector<string> &values, int index): Modification(Modification::aDeletion), etEditType(etSkill), iIndex(index), pCharacterList(characterList), pSkillList(skillList), pCharacter(0), sSkill(skill), vValues(values)
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
    switch (etEditType)
    {
        case etCharacter:   if (pCharacterList)
                            {
                                switch (action())
                                {
                                    case aAddition: pCharacterList->remove(iIndex);
                                                    break;
                                    case aDeletion: pCharacterList->add(*pCharacter, iIndex);
                                                    break;
                                    default:    break;
                                }
                            }
                            break;
        case etSkill:   if (pSkillList)
                        {
                            switch (action())
                            {
                                case aAddition: pSkillList->remove(iIndex);
                                                break;
                                case aDeletion: if (pCharacterList)
                                                {
                                                    pSkillList->add(sSkill, iIndex);
                                                    int i = 0;
                                                    for (CharacterList::iterator it = pCharacterList->begin(); it != pCharacterList->end(); it++)
                                                    {
                                                        it->addSkill(vValues[i], iIndex);
                                                        i++;
                                                    }
                                                }
                                                break;
                                default:    break;
                            }
                            break;
                        }
        default:    break;
    }
}

void CharacterModification::redo()
{
    switch (etEditType)
    {
        case etCharacter:   if (pCharacterList)
                            {
                                switch (action())
                                {
                                    case aAddition: pCharacterList->add(*pCharacter, iIndex);
                                                    break;
                                    case aDeletion: pCharacterList->remove(iIndex);
                                                    break;
                                    default:    break;
                                }
                            }
                            break;
        case etSkill:   if (pSkillList)
                        {
                            switch (action())
                            {
                                case aAddition: pSkillList->add(sSkill, iIndex);
                                                break;
                                case aDeletion: if (pCharacterList)
                                                {
                                                    pSkillList->remove(iIndex);
                                                    for (CharacterList::iterator it = pCharacterList->begin(); it != pCharacterList->end(); it++)
                                                    {
                                                        it->removeSkill(iIndex);
                                                    }
                                                }
                                                break;
                                default:    break;
                            }
                            break;
                        }
        default:    break;
    }
}
