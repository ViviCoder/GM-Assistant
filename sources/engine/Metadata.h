/*************************************************************************
* Copyright © 2013-2020 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_METADATA
#define HEADER_METADATA

#include <string>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>

/*!
 * \brief Class encapsulating metadata for games
 */
class Metadata
{
    public:
        class Date
        {
            public:
                /*!
                 * \brief Default constructor
                 *
                 * Initialize the date to today
                 */
                Date();
                /*!
                 * \brief Full constructor
                 * \param day Day of the month
                 * \param month Month of the year
                 * \param year Year
                 */
                Date(int day, int month, int year);
                /*!
                 * \brief Constructor from a string
                 * \param date Date of the form dd/mm/yyyy
                 */
                Date(const std::string &date);
                /*!
                 * \brief Getter for the day
                 * \return Day of the month
                 */
                inline int day() const;
                /*!
                 * \brief Getter for the month
                 * \return Month of the year
                 */
                inline int month() const;
                /*!
                 * \brief Getter for the year
                 * \return Year
                 */
                inline int year() const;
                /*!
                 * \brief Comparison operator
                 * \param date Date to compare with
                 * \return True if both are different, false otherwise
                 */
                bool operator!=(const Date &date) const;
            private:
                //! Day
                int iDay;
                //! Month
                int iMonth;
                //! Year
                int iYear;
        };
        //! Default constructor
        Metadata();
        /*!
         * \brief Getter for the title
         * \return Title of the game
         */
        inline std::string title() const;
        /*!
         * \brief Setter for the title
         * \param title New title
         */
        inline void setTitle(const std::string &title);
        /*!
         * \brief Getter for the author
         * \return Author of the game
         */
        inline std::string author() const;
        /*!
         * \brief Setter for the author
         * \param author New author
         */
        inline void setAuthor(const std::string &author);
        /*!
         * \brief Getter for the creation date
         * \return Creation date of the game
         */
        inline Date creationDate() const;
        /*!
         * \brief Setter for the creation date
         * \param creationDate New creation date
         */
        inline void setCreationDate(const Date &creationDate);
        /*!
         * \brief Getter for the description
         * \return Description of the game
         */
        inline std::string description() const;
        /*!
         * \brief Setter for the description
         * \param description New description
         */
        inline void setDescription(const std::string &description);
        /*!
         * \brief Getter for the role-playing game
         * \return Role-playing game
         */
        inline std::string rpg() const;
        /*!
         * \brief Setter for the role-playing game
         * \param rpg New role-playing game
         */
        inline void setRpg(const std::string &rpg); 
        /*!
         * \brief Getter for the players
         * \return Players
         */
        inline std::string players() const;
        /*!
         * \brief Setter for the players
         * \param players New players
         */
        inline void setPlayers(const std::string &players);
        /*!
         * \brief Getter for the game date
         * \return Game date
         */
        inline Date gameDate() const;
        /*!
         * \brief Setter for the game date
         * \param gameDate New game date
         */
        inline void setGameDate(const Date &gameDate);
        /*!
         * \brief XML loader
         * \param root Root of the XML subtree
         */
        void fromXML(const Poco::XML::Element *root);
        /*!
         * \brief XML saver
         * \param root Root of the XML subtree
         */
        void toXML(Poco::XML::Element *root) const;
        /*!
         * \brief Comparison operator
         * \param metadata Metadata to compare with
         * \return True if both are different, false otherwise
         */
        bool operator!=(const Metadata &metadata) const;
    private:
        //! Title of the game
        std::string sTitle;
        //! Author of the game
        std::string sAuthor;
        //! Creation date
        Date dCreation;
        //! Description
        std::string sDescription;
        //! Role-playing game
        std::string sRpg;
        //! Players
        std::string sPlayers;
        //! Game date
        Date dGame;
};

// inline Metadata::Date methods

int Metadata::Date::day() const
{
    return iDay;
}

int Metadata::Date::month() const
{
    return iMonth;
}

int Metadata::Date::year() const
{
    return iYear;
}

// inline Metadata methods

std::string Metadata::title() const
{
    return sTitle;
}

void Metadata::setTitle(const std::string &title)
{
    sTitle = title;
}

std::string Metadata::author() const
{
    return sAuthor;
}

void Metadata::setAuthor(const std::string &author)
{
    sAuthor = author;
}

Metadata::Date Metadata::creationDate() const
{
    return dCreation;
}

void Metadata::setCreationDate(const Date &creationDate)
{
    dCreation = creationDate;
}

std::string Metadata::description() const
{
    return sDescription;
}

void Metadata::setDescription(const std::string &description)
{
    sDescription = description;
}

std::string Metadata::rpg() const
{
    return sRpg;
}

void Metadata::setRpg(const std::string &rpg)
{
    sRpg = rpg;
}

std::string Metadata::players() const
{
    return sPlayers;
}

void Metadata::setPlayers(const std::string &players)
{
    sPlayers = players;
}

Metadata::Date Metadata::gameDate() const
{
    return dGame;
}

void Metadata::setGameDate(const Date &gameDate)
{
    dGame = gameDate;
}

#endif
