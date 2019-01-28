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

#ifndef HEADER_CHARACTERMODIFICATION
#define HEADER_CHARACTERMODIFICATION

#include "Modification.h"
#include "CharacterList.h"
#include "PropertyList.h"

/*!
 * \brief Modification of the characters/properties
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
            //! Edition of a property
            etProperty,
            //! Edition of a value
            etValue
        };
        /*!
         * \brief Constructor for additions of characters
         * \param list List of characters
         * \param index Index of the character
         */
        CharacterModification(CharacterList *list, int index);
        /*!
         * \brief Constructor for deletions of characters
         * \param list List of characters
         * \param character Character added or deleted
         * \param index Index of the character
         * \param isAddition Indicates if the modification is an addition (true) or a deletion (false)
         */
        CharacterModification(CharacterList *list, Character *character, int index);
        /*!
         * \brief Constructor for additions of properties
         * \param list List of properties
         * \param property Property added
         * \param characterList List of characters
         * \param index Index of the property
         */
        CharacterModification(PropertyList *list, const std::string &property, CharacterList *characterList, int index);
        /*!
         * \brief Constructor for deletions of properties
         * \param propertyList List of properties
         * \param property Property deleted
         * \param characterList List of characters
         * \param values Values of the property
         * \param index Index of the property
         */
        CharacterModification(PropertyList *propertyList, const std::string &property, CharacterList *characterList, const std::vector<std::string> &values, int index);
        /*!
         * \brief Constructor for editions of properties
         * \param propertyList List of properties
         * \param property Modified property
         * \param newProperty New property
         * \param index Index of the property
         */
        CharacterModification(PropertyList *propertyList, const std::string &property, const std::string &newProperty, int index);
        /*!
         * \brief Constructor for editions of characters
         * \param characterList List of characters
         * \param name Modified name
         * \param shortDescription Modified player's name
         * \param newName New name
         * \param newShortDescription New player's name
         * \param index Index of the character
         */
        CharacterModification(CharacterList *characterList, const std::string &name, const std::string &shortDescription, const std::string &newName, const std::string &newShortDescription, int index);
        /*!
         * \brief Constructor for editions of values
         * \param characterList List of characters
         * \param value Modified value
         * \param newValue New value
         * \param character Index of the character
         * \param property Index of the property
         */
        CharacterModification(CharacterList *characterList, const std::string &value, const std::string &newValue, int character, int property);
        /*!
         * \brief Constructor for movements of properties
         * \param propertyList List of properties
         * \param characterList List of characters
         * \param oldIndex Old index of the character
         * \param newIndex New index of the character
         */
        CharacterModification(PropertyList *propertyList, CharacterList *characterList, int oldIndex, int newIndex);
        /*!
         * \brief Constructor for movements of characters
         * \param characterList List of characters
         * \param oldIndex Old index of the character
         * \param newIndex New index of the character
         */
        CharacterModification(CharacterList *characterList, int oldIndex, int newIndex);
        /*!
         * \brief Destructor
         */
        virtual ~CharacterModification();
        // inherited pure virtual getter
        Type type() const;
        // inherited pure virtual methods
        void undo();
        void redo();
        /*!
         * \brief Getter for the index
         * \return The index (or the row for cell editions) of the modification
         */
        int index() const;
        /*!
         * \brief Getter for the new index
         * \return The new index (or the column for cell editions) of the modification
         */
        int newIndex() const;
        /*!
         * \brief Getter for the edition type
         * \return The edition type of the modification
         */
        EditionType editionType() const;
    private:
        //! Type of edition
        EditionType etEditType;
        //! First (or unique) index of the modification
        int iIndex;
        //! Second or new index of the modification
        int iNewIndex;
        //! Character list modified
        CharacterList *pCharacterList;
        //! Property list modified
        PropertyList *pPropertyList;
        //! Copy of the character
        Character *pCharacter;
        //! Copy of the property (or player name or value)
        std::string sProperty;
        //! Copy of the new property (or player name or value)
        std::string sNewProperty;
        //! Copy of the name
        std::string sName;
        //! Copy of the new name
        std::string sNewName;
        //! Copy of the values of a property
        std::vector<std::string> vValues;
};

#endif
