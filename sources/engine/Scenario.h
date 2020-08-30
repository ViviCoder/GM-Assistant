/*************************************************************************
* Copyright © 2011-2020 Vincent Prat & Simon Nicolas
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

#ifndef HEADER_SCENARIO
#define HEADER_SCENARIO

#include "Tree.h"
#include "CharacterList.h"
#include "PropertyList.h"
#include "IOConfig.h"
#include "Metadata.h"
#include "Note.h"

/*!
 * \brief Game scenario
 *
 * Contains everything necessary for a game
 */
class Scenario
{
    public:
        // types
        /*!
         * \brief Type of user interface used by the game
         */
        enum UserInterface {
            //! Full interface
            uiFull,
            //! Simple interface with the plot, musics and sounds
            uiSimple,
            //! Interface with only musics and sounds
            uiMusic,
            //! Design-phase interface with the plot, characters and notes
            uiDesign,
            //! Interface without musics and sounds
            uiNoMusic
        };
        /*
         * \brief Default constructor
         */
        Scenario();
        // loading and saving methods
        /*!
         * \brief Loads a scenario from a file
         * \param fileName File name of the scenario to load
         * \param checkFiles Indicates if the existence of files associated with FileItems is checked
         */
        void fromFile(const std::string &fileName, bool checkFiles = true);
        /*!
         * \brief Saves a game into a file
         * \param fileName Name of the file to be saved
         */
        void toFile(const std::string &fileName) const;
        /*!
         * \brief Getter for the main note of the scenario
         * \return Main note
         */
        inline Note& mainNote();
        // accessors
        /*!
         * \brief Getter for the plot tree
         * \return Plot tree
         */
        inline Tree& plot();
        /*!
         * \brief Getter for the history tree
         * \return History tree
         */
        inline Tree& history();
        /*!
         * \brief Getter for the music tree
         * \return Music tree
         */
        inline Tree& music();
        /*!
         * \brief Getter for the sound effect tree
         * \return Sound effect tree
         */
        inline Tree& effects();
        /*!
         * \brief Getter for the property list
         * \return Property list
         */
        inline PropertyList& properties();
        /*!
         * \brief Getter for the character list
         * \return Character list
         */
        inline CharacterList& characters();
        /*!
         * \brief Get the user interface
         * \return Current user interface
         */
        inline UserInterface userInterface() const;
        /*!
         * \brief Set the user interface
         * \param userInterface New value of the user interface
         */
        inline void setUserInterface(UserInterface userInterface);
        //! Clear the current scenario
        void clear();
        /*!
         * \brief Convertion method from UserInterface to string
         * \param userInterface User interface
         * \return String equivalent to the interface
         */
        static std::string interfaceToString(UserInterface userInterface);
        /*!
         * \brief Convertion method form string to UserInterface
         * \param userInterface String to convert
         * \return User interface equivalent to the string 
         */
        static UserInterface stringToInterface(const std::string& userInterface);
        /*!
         * \brief Getter for the IO configuration
         * \return IO configuration
         */
        inline IOConfig configuration() const;
        /*!
         * \brief Setter for the IO configuration version
         * \param version Version
         *
         * Sets the IO configuration to the default for the given version
         */
        inline void setVersion(const Version &version);
        /*!
         * \brief Getter for the metadata
         * \return Metadata of the game
         */
        inline Metadata& metadata();
        /*!
         * \brief Setter for the IO configuration
         * \param config IO configuration
         */
        inline void setConfig(const IOConfig &config);
    private:
        //! Plot tree
        Tree tPlot;
        //! History tree
        Tree tHistory;
        //! Music tree
        Tree tMusic;
        //! Sound effect tree
        Tree tEffects;
        //! Main note
        Note nMain;
        //! Character list
        CharacterList lCharacters;
        //! Property list
        PropertyList lProperties;
        //! User interface used by the game
        UserInterface uiInterface;
        //! IO configuration
        IOConfig ioConfig;
        //! Metadata
        Metadata mMetadata;
        //! Temporary directory (for the archive format)
        std::string sTempDir;
};

// inline methods

void Scenario::setConfig(const IOConfig &config)
{
    ioConfig = config;
}

Tree& Scenario::plot()
{
    return tPlot;
}

Tree& Scenario::history()
{
    return tHistory;
}

Tree& Scenario::music()
{
    return tMusic;
}

Tree& Scenario::effects()
{
    return tEffects;
}

CharacterList& Scenario::characters()
{
    return lCharacters;
}

PropertyList& Scenario::properties()
{
    return lProperties;
}

Scenario::UserInterface Scenario::userInterface() const
{
    return uiInterface;
}

void Scenario::setUserInterface(Scenario::UserInterface userInterface)
{
    uiInterface = userInterface;
}

IOConfig Scenario::configuration() const
{
    return ioConfig;
}

void Scenario::setVersion(const Version &version)
{
    ioConfig = IOConfig(version);
}

Metadata& Scenario::metadata()
{
    return mMetadata;
}

Note& Scenario::mainNote()
{
    return nMain;
}

#endif
