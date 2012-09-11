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

#ifndef HEADER_CHARACTERMODIFICATION
#define HEADER_CHARACTERMODIFICATION

#include "Modification.h"
#include "CharacterList.h"
#include "SkillList.h"

/*!
 * \brief Modification of the characters/skills list
 */
class CharacterModification: public Modification
{
    public:
        /*! 
         * \brief Type of edition
         */
        enum EditionType
        {
            //! Edition of a character 
            etCharacter,
            //! Edition of a skill
            etSkill,
            //! Edition of a value
            etValue
        };
        /*!
         *  \brief Constructor for additions/deletions of a character
         *  \param list List of characters
         *  \param character Character added or deleted
         *  \param index Index of the character
         *  \param isAddition Indicates if the modification is an addition (true) or a deletion (false)
         */
        CharacterModification(CharacterList *list, Character *character, int index, bool isAddition);
        /*!
         *  \brief Constructor for additions/deletions of a skill
         *  \param list List of skills
         *  \param skill Skill added or deleted
         *  \param index Index of the skill
         *  \param isAddition Indicates if the modification is an addition (true) or a deletion (false)
         */
        CharacterModification(SkillList *list, const std::string &skill, int index, bool isAddition);
        /*!
         * \brief Destructor
         */
        virtual ~CharacterModification();
        // inherited pure virtual getter
        Type type() const;
        // inherited pure virtual methods
        void undo();
        void redo();
    private:
        //! Type of edition
        EditionType etEditType;
        //! First (or unique) index of the modification
        int iIndex;
        //! Character list modified
        CharacterList *pCharacterList;
        //! Skill list modified
        SkillList *pSkillList;
        //! Copy of the character
        Character *pCharacter;
        //! Copy of the skill
        std::string sSkill;
};

#endif
