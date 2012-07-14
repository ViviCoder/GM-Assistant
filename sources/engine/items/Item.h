/*************************************************************************
* Copyright © 2011-2012 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_ITEM
#define HEADER_ITEM

#include <string>
#include <stdexcept>
#include <libxml++/libxml++.h>

/*!
 * \brief Base item class
 */
class Item
{
    public:
        // Different possible states for the item   
        enum State {sNone,sProgress,sSuccess,sFailure};
        /*!
         * \brief Type of item
         */
        enum Type
        {
            //! Basic item
            tBasic,
            //! Item related to a file
            tFile,
            //! Item related to a sound file
            tSound,
            //! Item related to an image file
            tImage
        };
        // constructor
        Item(const std::string &content="", State state=sNone);
        /*!
         * \brief Default destructor
         */
        virtual ~Item();
        // accessors
        std::string content() const;
        void setContent(const std::string &content);
        State state() const;
        void setState(State state);
        virtual Type type() const;
        // XML-related methods
        /*!
         * \brief XML loader
         * \param root Root of the tree
         * \param checkFile Indicates if the existence of potential files is checked
         */
        virtual void fromXML(const xmlpp::Element &root, bool checkFiles);
        virtual void toXML(xmlpp::Element &root);
        // static methods to get a link between strings and state or type
        static std::string stateToStr(State state);
        /*!
         * \brief Conversion method from Type to string
         * \param type Type
         * \return String equivalent to the type
         */
        static std::string typeToStr(Type type);
        static State strToState(const std::string &name) throw(std::invalid_argument);
        /*!
         * \brief Conversion method from string to Type
         * \param name Name of the type
         * \return Type equivalent to the string
         */
        static Type strToType(const std::string &name) throw(std::invalid_argument);
        /*!
         * \brief Subclass hierarchy test method
         * \param type Type to test
         * \param isType Mother class type to test
         * \return True if type is a subclass of isType, false otherwise
         */
        static bool is(Type type, Type isType);
        /*!
         * \brief Method for getting the parent type of a given type
         * \param type Son type
         * \return Parent type
         */
        static Type parentType(Type type);
    private:
        State sState;
        std::string sContent;
};

#endif
