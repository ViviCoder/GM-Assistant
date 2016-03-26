/*************************************************************************
* Copyright © 2011-2016 Vincent Prat & Simon Nicolas
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

#include "FileMapping.h"
#include <libxml++/libxml++.h>
#include "IOConfig.h"
#include "FileMapping.h"

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
            tImage,
            //! Item associted with a note
            tNote
        };
        /*!
         * \brief Constructor
         * \param content Content of the item
         * \param state State of the item
         * \param expanded Expanded/collapsed state of the item
         */
        Item(const std::string &content="", State state=sNone, bool expanded=false);
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
        /*!
         * \brief Getter for the expanded property
         * \return True if the item is expanded, false otherwise
         */
        bool expanded() const;
        /*!
         * \brief Setter for the expanded property
         * \param expanded True to expand the item, false otherwise
         */
        void setExpanded(bool expanded);
        // XML-related methods
        /*!
         * \brief XML loader
         * \param config IO configuration
         * \param root Root of the tree
         * \param checkFiles Indicates if the existence of potential files is checked
         */
        virtual void fromXML(const IOConfig &config, const xmlpp::Element &root, bool checkFiles);
        /*!
         * \brief XML saver
         * \param config IO configuration
         * \param root Root of the XML tree
         * \param fileMapping Mapping of files associated with items (for archives)
         */
        virtual void toXML(const IOConfig &config, xmlpp::Element &root, FileMapping &fileMapping);
        // static methods to get a link between strings and state or type
        static std::string stateToStr(State state);
        /*!
         * \brief Conversion method from Type to string
         * \param type Type
         * \param config IO configuration
         * \return String equivalent to the type
         * \throw xmlpp::exception Thrown when the given string does not correspond to any state
         */
        static std::string typeToStr(Type type, const IOConfig &config);
        static State strToState(const std::string &name) throw(xmlpp::exception);
        /*!
         * \brief Conversion method from string to Type
         * \param name Name of the type
         * \param config IO configuration
         * \return Type equivalent to the string
         * \throw xmlpp::exception Thrown when the given string does not correspond to any type
         */
        static Type strToType(const std::string &name, const IOConfig &config) throw(xmlpp::exception);
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
        /*!
         * \brief Conversion method from boolean to string
         * \param value Boolean to convert
         * \return String equivalent to the boolean
         */
        static std::string boolToStr(bool value);
        /*!
         * \brief Conversion method from string to boolean
         * \param name String to convert
         * \return Boolean equivalent to the string
         * \throw xmlpp::exception Thrown when the given string does not correspond to any boolean value
         */
        static bool strToBool(const std::string &name) throw(xmlpp::exception);
    private:
        State sState;
        std::string sContent;
        bool bExpanded;
};

#endif
