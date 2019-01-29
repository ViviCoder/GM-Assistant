/*************************************************************************
* Copyright © 2012-2019 Vincent Prat & Simon Nicolas
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

CharacterModification::CharacterModification(CharacterList *list, int index): Modification(Modification::aAddition), etEditType(etCharacter), iIndex(index), pCharacterList(list), pPropertyList(0), pCharacter(0)
{
}

CharacterModification::CharacterModification(CharacterList *list, Character *character, int index): Modification(Modification::aDeletion), etEditType(etCharacter), iIndex(index), pCharacterList(list), pPropertyList(0), pCharacter(character)
{
}

CharacterModification::CharacterModification(PropertyList *list, const string &property, CharacterList *characterList, int index): Modification(Modification::aAddition), etEditType(etProperty), iIndex(index), pCharacterList(characterList), pPropertyList(list), pCharacter(0), sProperty(property)
{
}

CharacterModification::CharacterModification(PropertyList *propertyList, const string &property, CharacterList *characterList, const vector<string> &values, int index): Modification(Modification::aDeletion), etEditType(etProperty), iIndex(index), pCharacterList(characterList), pPropertyList(propertyList), pCharacter(0), sProperty(property), vValues(values)
{
}

CharacterModification::CharacterModification(PropertyList *propertyList, const string &property, const string &newProperty, int index): Modification(Modification::aEdition), etEditType(etProperty), iIndex(index), pCharacterList(0), pPropertyList(propertyList), pCharacter(0), sProperty(property), sNewProperty(newProperty)
{
}

CharacterModification::CharacterModification(CharacterList *characterList, const string &name, const string &shortDescription, const string &newName, const string &newShortDescription, int index): Modification(Modification::aEdition), etEditType(etCharacter), iIndex(index), pCharacterList(characterList), pPropertyList(0), pCharacter(0), sProperty(shortDescription), sNewProperty(newShortDescription), sName(name), sNewName(newName)
{
}

CharacterModification::CharacterModification(CharacterList *characterList, const string &value, const string &newValue, int character, int property): Modification(Modification::aEdition), etEditType(etValue), iIndex(character), iNewIndex(property), pCharacterList(characterList), pPropertyList(0), pCharacter(0), sProperty(value), sNewProperty(newValue)
{
}

CharacterModification::CharacterModification(PropertyList *propertyList, CharacterList *characterList, int oldIndex, int newIndex): Modification(Modification::aMovement), etEditType(etProperty), iIndex(oldIndex), iNewIndex(newIndex), pCharacterList(characterList), pPropertyList(propertyList), pCharacter(0)
{
}

CharacterModification::CharacterModification(CharacterList *characterList, int oldIndex, int newIndex): Modification(Modification::aMovement), etEditType(etCharacter), iIndex(oldIndex), iNewIndex(newIndex), pCharacterList(characterList), pPropertyList(0), pCharacter(0)
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
                                    case aAddition: pCharacter = (*pCharacterList)[iIndex];
                                                    pCharacterList->remove(iIndex);
                                                    break;
                                    case aDeletion: pCharacterList->add(pCharacter, iIndex);
                                                    pCharacter = 0;
                                                    break;
                                    case aEdition:  {
                                                        Character *character = (*pCharacterList)[iIndex];
                                                        character->setName(sName);
                                                        character->setShortDescription(sProperty);
                                                        break;
                                                    }
                                    case aMovement: pCharacterList->move(iNewIndex, iIndex);
                                                    break;
                                    default:    break;
                                }
                            }
                            break;
        case etProperty:   switch (action())
                        {
                            case aAddition: if (pPropertyList)
                                            {
                                                pPropertyList->remove(iIndex);
                                            }
                                            if (pCharacterList)
                                            {
                                                for (CharacterList::iterator it = pCharacterList->begin(); it != pCharacterList->end(); it++)
                                                {
                                                    (*it)->removeProperty(iIndex);
                                                }
                                            }
                                            break;
                            case aDeletion: if (pPropertyList)
                                            {
                                                pPropertyList->add(sProperty, iIndex);
                                            }
                                            if (pCharacterList)
                                            {
                                                int i = 0;
                                                for (CharacterList::iterator it = pCharacterList->begin(); it != pCharacterList->end(); it++)
                                                {
                                                    (*it)->addProperty(vValues[i], iIndex);
                                                    i++;
                                                }
                                            }
                                            break;
                            case aEdition:  if (pPropertyList)
                                            {
                                                (*pPropertyList)[iIndex] = sProperty;
                                            }
                                            break;
                            case aMovement: if (pPropertyList)
                                            {
                                                pPropertyList->move(iNewIndex, iIndex);
                                            }
                                            if (pCharacterList)
                                            {
                                                for (CharacterList::iterator it = pCharacterList->begin(); it != pCharacterList->end(); it++)
                                                {
                                                    (*it)->moveProperty(iNewIndex, iIndex);
                                                }
                                            }
                                            break;
                            default:    break;
                        }
                        break;
        case etValue:   if (pCharacterList)
                        {
                            (*pCharacterList)[iIndex]->property(iNewIndex) = sProperty;
                        }
                        break;
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
                                    case aAddition: pCharacterList->add(pCharacter, iIndex);
                                                    pCharacter = 0;
                                                    break;
                                    case aDeletion: pCharacter = (*pCharacterList)[iIndex];
                                                    pCharacterList->remove(iIndex);
                                                    break;
                                    case aEdition:  {
                                                        Character *character = (*pCharacterList)[iIndex];
                                                        character->setName(sNewName);
                                                        character->setShortDescription(sNewProperty);
                                                        break;
                                                    }
                                    case aMovement: pCharacterList->move(iIndex, iNewIndex);
                                                    break;
                                    default:    break;
                                }
                            }
                            break;
        case etProperty:   switch (action())
                        {
                            case aAddition: if (pPropertyList)
                                            {
                                                pPropertyList->add(sProperty, iIndex);
                                            }
                                            if (pCharacterList)
                                            {
                                                for (CharacterList::iterator it = pCharacterList->begin(); it != pCharacterList->end(); it++)
                                                {
                                                    (*it)->addProperty("0", iIndex);
                                                }
                                            }
                                            break;
                            case aDeletion: if (pPropertyList)
                                            {
                                                pPropertyList->remove(iIndex);
                                            }
                                            if (pCharacterList)
                                            {
                                                for (CharacterList::iterator it = pCharacterList->begin(); it != pCharacterList->end(); it++)
                                                {
                                                    (*it)->removeProperty(iIndex);
                                                }
                                            }
                                            break;
                            case aEdition:  if (pPropertyList)
                                            {
                                                (*pPropertyList)[iIndex] = sNewProperty;
                                            }
                                            break;
                            case aMovement: if (pPropertyList)
                                            {
                                                pPropertyList->move(iIndex, iNewIndex);
                                            }
                                            if (pCharacterList)
                                            {
                                                for (CharacterList::iterator it = pCharacterList->begin(); it != pCharacterList->end(); it++)
                                                {
                                                    (*it)->moveProperty(iIndex, iNewIndex);
                                                }
                                            }
                                            break;
                            default:    break;
                        }
                        break;
        case etValue:   if (pCharacterList)
                        {
                            (*pCharacterList)[iIndex]->property(iNewIndex) = sNewProperty;
                        }
                        break;
        default:    break;
    }
}

int CharacterModification::index() const
{
    return iIndex;
}

int CharacterModification::newIndex() const
{
    return iNewIndex;
}

CharacterModification::EditionType CharacterModification::editionType() const
{
    return etEditType;
}
