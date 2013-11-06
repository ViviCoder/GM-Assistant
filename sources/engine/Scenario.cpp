/*************************************************************************
* Copyright © 2011-2013 Vincent Prat & Simon Nicolas
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

#include "Scenario.h"
#include "Version.h"
#include <Poco/Path.h>
#include <Poco/Zip/Decompress.h>
#include <fstream>
#include <Poco/TemporaryFile.h>

using namespace std;

// constructors

Scenario::Scenario(const FileDetector *detector): uiInterface(uiFull), ioConfig(Version()), pDetector(0)
{
    if (detector && detector->isValid())
    {
        pDetector = detector;
    }
}

// XML methods

void Scenario::fromFile(const std::string &fileName, bool checkFiles) throw(xmlpp::exception, invalid_argument, overflow_error)
{
    using namespace xmlpp;
    using namespace Poco;
    string xmlFile, fileType;
    bool isArchive = false;

    clear();
    if (pDetector)
    {
       fileType = pDetector->typeOfFile(fileName);
    }
    if (fileType == "application/xml")
    {
        xmlFile = fileName;
    }
    else if (!pDetector || fileType == "application/zip")
    {
        // attempt to unzip
        ifstream input(fileName.c_str());
        if (input.good())
        {
            // creating new temporary directory and extracting into it
            sTempDir = TemporaryFile::tempName();
            Zip::Decompress dec(input, sTempDir);
            TemporaryFile::registerForDeletion(sTempDir);
            // valid extraction
            isArchive = false;
            try
            {
                dec.decompressAllFiles();
                isArchive = true;
            }
            catch (Poco::Exception)
            {
                if (!pDetector)
                {
                    xmlFile = fileName;
                }
                else
                {
                    throw xmlpp::exception("Bad file format");
                }
            }
            if (isArchive)
            {
                Zip::Decompress::ZipMapping mapping = dec.mapping();
                try
                {
                    xmlFile = mapping.at("scenario.xml").getFileName();
                }
                catch(out_of_range &e)
                {
                    throw xmlpp::exception("No scenario in file " + fileName);
                }
            }
        }
        else
        {
            throw xmlpp::exception("Unable to open the file " + fileName);
        }
    }
    else
    {
        throw xmlpp::exception("Unreckognized file format");
    }
    ioConfig = IOConfig::detect(xmlFile, isArchive);
    DomParser parser(xmlFile);
    Document *document = parser.get_document();
    Element *root = document->get_root_node();
    if (root->get_name() != ioConfig.rootName())
    {
        throw xmlpp::exception("Bad document content type: " + ioConfig.rootName() + " expected");
    }
    // getting the user interface
    try
    {
        Attribute *attr = root->get_attribute("interface");
        if (attr)
        {
            uiInterface = stringToInterface(attr->get_value());
        }
        else
        {
            uiInterface = uiFull;
        }
    }
    catch (invalid_argument)
    {
        throw xmlpp::exception("Bad user interface");
    }
    // now loading the different parts of the game
    Node::NodeList node;
    if (ioConfig.hasMetadata())
    {
        node = root->get_children("metadata");
        if (!node.empty())
        {
            mMetadata.fromXML(*dynamic_cast<Element*>(node.front()));
        }
    }
    node = root->get_children(ioConfig.plotName());
    if (node.empty())
    {
        throw xmlpp::exception("Missing \"" + ioConfig.plotName() + "\" section");
    }
    else
    {
        tPlot.fromXML(ioConfig, *dynamic_cast<Element*>(node.front()), checkFiles);
    }
    node = root->get_children("notes");
    if (node.empty())
    {
        throw xmlpp::exception("Missing \"notes\" section");
    }
    else
    {
        Element *elem = dynamic_cast<Element*>(node.front());
        if (elem->has_child_text())
        {
            sNotes = elem->get_child_text()->get_content();
        }
        else
        {
            sNotes = "";
        }
    }
    node = root->get_children(ioConfig.propertiesName());
    if (node.empty())
    {
        throw xmlpp::exception("Missing \"" + ioConfig.propertiesName() + "\" section");
    }
    else
    {
        lProperties.fromXML(ioConfig, *dynamic_cast<Element*>(node.front()));
    }
    node = root->get_children("characters");
    if (node.empty())
    {
        throw xmlpp::exception("Missing \"characters\" section");
    }
    else
    {
        lCharacters.fromXML(ioConfig, *dynamic_cast<Element*>(node.front()));
    }
    node = root->get_children("history");
    if (node.empty())
    {
        throw xmlpp::exception("Missing \"history\" section");
    }
    else
    {
        tHistory.fromXML(ioConfig, *dynamic_cast<Element*>(node.front()), checkFiles);
    }
    node = root->get_children("music");
    if (node.empty())
    {
        throw xmlpp::exception("Missing \"music\" section");
    }
    else
    {
        tMusic.fromXML(ioConfig, *dynamic_cast<Element*>(node.front()), checkFiles);
    }
    node = root->get_children("effects");
    if (node.empty())
    {
        throw xmlpp::exception("Missing \"effects\" section");
    }
    else
    {
        tEffects.fromXML(ioConfig, *dynamic_cast<Element*>(node.front()), checkFiles, true);
    }
}

void Scenario::toFile(const string &fileName) const
{
    using namespace xmlpp;

    Document document;
    Element *root = document.create_root_node(ioConfig.rootName());
    root->set_attribute("version",ioConfig.version().shortVersion());
    root->set_attribute("interface",interfaceToString(uiInterface));
    Element *tmp;
    if (ioConfig.hasMetadata())
    {
       tmp = root->add_child("metadata");
       mMetadata.toXML(*tmp);
    }
    tmp = root->add_child(ioConfig.plotName());
    tPlot.toXML(ioConfig, *tmp);
    tmp = root->add_child("notes");
    tmp->add_child_text(sNotes);
    tmp = root->add_child(ioConfig.propertiesName());
    lProperties.toXML(ioConfig, *tmp);
    tmp = root->add_child("characters");
    lCharacters.toXML(ioConfig, *tmp);
    tmp = root->add_child("history");
    tHistory.toXML(ioConfig, *tmp);
    tmp = root->add_child("music");
    tMusic.toXML(ioConfig, *tmp);
    tmp = root->add_child("effects");
    tEffects.toXML(ioConfig, *tmp);
    document.write_to_file_formatted(fileName,"UTF-8");
}

// accessors

Tree& Scenario::plot()
{
    return tPlot;
}

string& Scenario::notes()
{
    return sNotes;
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

// methods

void Scenario::clear()
{
    tPlot.clear();
    sNotes = "";
    tHistory.clear();
    tMusic.clear();
    tEffects.clear();
    lProperties.clear();
    lCharacters.clear();
    uiInterface = uiFull;
    ioConfig = IOConfig(Version());
    mMetadata = Metadata();
    // removing previous temporary directory
    if (sTempDir != "")
    {
        Poco::File file(sTempDir);
        if (file.exists() && file.canWrite())
        {
            file.remove(true);
        }
        sTempDir = "";
    }
}

CharacterList& Scenario::characters()
{
    return lCharacters;
}

PropertyList& Scenario::properties()
{
    return lProperties;
}

string Scenario::interfaceToString(UserInterface interface)
{
    switch (interface)
    {
        case uiFull:    return "full";
        case uiSimple:  return "simple";
        case uiMusic:   return "music";
        case uiDesign:  return "design";
        default:        return "nomusic";
    }
}

Scenario::UserInterface Scenario::stringToInterface(const std::string& interface) throw(invalid_argument)
{
    if (interface=="full")
        return uiFull;
    else if (interface=="simple")
        return uiSimple;
    else if (interface=="music")
        return uiMusic;
    else if (interface=="design")
        return uiDesign;
    else if (interface=="nomusic")
        return uiNoMusic;
    else
        throw invalid_argument("Invalid user interface");
}

Scenario::UserInterface Scenario::userInterface() const
{
    return uiInterface;
}

void Scenario::setUserInterface(Scenario::UserInterface interface)
{
    uiInterface = interface;
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
