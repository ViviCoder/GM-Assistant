/*************************************************************************
* Copyright © 2013 Vincent Prat & Simon Nicolas
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
#include <libxml++/libxml++.h>

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
                int day() const;
                /*!
                 * \brief Getter for the month
                 * \return Month of the year
                 */
                int month() const;
                /*!
                 * \brief Getter for the year
                 * \return Year
                 */
                int year() const;
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
        std::string title() const;
        /*!
         * \brief Setter for the title
         * \param title New title
         */
        void setTitle(const std::string &title);
        /*!
         * \brief Getter for the author
         * \return Author of the game
         */
        std::string author() const;
        /*!
         * \brief Setter for the author
         * \param author New author
         */
        void setAuthor(const std::string &author);
        /*!
         * \brief Getter for the creation date
         * \return Creation date of the game
         */
        Date creationDate() const;
        /*!
         * \brief Setter for the creation date
         * \param creationDate New creation date
         */
        void setCreationDate(const Date &creationDate);
        /*!
         * \brief Getter for the description
         * \return Description of the game
         */
        std::string description() const;
        /*!
         * \brief Setter for the description
         * \param description New description
         */
        void setDescription(const std::string &description);
        /*!
         * \brief Getter for the players
         * \return Players
         */
        std::string players() const;
        /*!
         * \brief Setter for the players
         * \param players New players
         */
        void setPlayers(const std::string &players);
        /*!
         * \brief Getter for the game date
         * \return Game date
         */
        Date gameDate() const;
        /*!
         * \brief Setter for the game date
         * \param New game date
         */
        void setGameDate(const Date &gameDate);
        /*!
         * \brief XML loader
         * \param root Root of the XML tree
         */
        void fromXML(const xmlpp::Element &root);
        /*!
         * \brief XML saver
         * \param root Root of the XML tree
         */
        void toXML(xmlpp::Element &root) const;
    private:
        //! Title of the game
        std::string sTitle;
        //! Author of the game
        std::string sAuthor;
        //! Creation date
        Date dCreation;
        //! Description
        std::string sDescription;
        //! Players
        std::string sPlayers;
        //! Game date
        Date dGame;
};

#endif
