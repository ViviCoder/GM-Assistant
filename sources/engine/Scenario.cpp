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

#include "Scenario.h"
#include "Version.h"
#include <Poco/Path.h>
#include <Poco/Zip/Decompress.h>
#include <Poco/FileStream.h>
#include <Poco/TemporaryFile.h>
#include <Poco/Zip/Compress.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/Text.h>
#include <Poco/DOM/DOMWriter.h>
#include <Poco/UTF8Encoding.h>
#include <Poco/XML/XMLWriter.h>
#include <fstream>

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

void Scenario::fromFile(const std::string &fileName, bool checkFiles)
{
    using namespace Poco;
    using namespace Poco::XML;
    string xmlFile, fileType;
    bool isArchive = false;

    clear();
    if (pDetector)
    {
       fileType = pDetector->typeOfFile(fileName);
    }
    if (fileType == "text/xml" || fileType == "application/xml")
    {
        xmlFile = fileName;
    }
    else if (!pDetector || fileType == "application/zip")
    {
        // attempt to unzip
        FileInputStream input(fileName.c_str());
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
                    xmlFile = mapping.at("scenario.xml").makeAbsolute(sTempDir).toString();
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
        throw xmlpp::exception("Unrecognized file format");
    }

    // reading the XML file
    DOMParser parser;
    Document *document = parser.parse(xmlFile);
    Element *root = document->documentElement();
    ioConfig = IOConfig::detect(xmlFile, root, isArchive);

    if (root->localName() != ioConfig.rootName())
    {
        throw xmlpp::exception("Bad document content type: " + ioConfig.rootName() + " expected");
    }
    // getting the user interface
    try
    {
        string attr = root->getAttribute("interface");
        if (!attr.empty())
        {
            uiInterface = stringToInterface(attr);
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
    Element *element;
    if (ioConfig.hasMetadata())
    {
        element = root->getChildElement("metadata");
        if (element)
        {
            mMetadata.fromXML(element);
        }
    }
    element = root->getChildElement(ioConfig.plotName());
    if (!element)
    {
        throw xmlpp::exception("Missing \"" + ioConfig.plotName() + "\" section");
    }
    else
    {
        tPlot.fromXML(ioConfig, element, checkFiles);
    }
    element = root->getChildElement("notes");
    if (!element)
    {
        throw xmlpp::exception("Missing \"notes\" section");
    }
    else
    {
        sNotes = element->innerText();
    }
    element = root->getChildElement(ioConfig.propertiesName());
    if (!element)
    {
        throw xmlpp::exception("Missing \"" + ioConfig.propertiesName() + "\" section");
    }
    else
    {
        lProperties.fromXML(ioConfig, element);
    }
    element = root->getChildElement("characters");
    if (!element)
    {
        throw xmlpp::exception("Missing \"characters\" section");
    }
    else
    {
        lCharacters.fromXML(ioConfig, element);
    }
    element = root->getChildElement("history");
    if (!element)
    {
        throw xmlpp::exception("Missing \"history\" section");
    }
    else
    {
        tHistory.fromXML(ioConfig, element, checkFiles);
    }
    element = root->getChildElement("music");
    if (!element)
    {
        throw xmlpp::exception("Missing \"music\" section");
    }
    else
    {
        tMusic.fromXML(ioConfig, element, checkFiles);
    }
    element = root->getChildElement("effects");
    if (!element)
    {
        throw xmlpp::exception("Missing \"effects\" section");
    }
    else
    {
        tEffects.fromXML(ioConfig, element, checkFiles);
    }

    document->release();
}

void Scenario::toFile(const string &fileName) const
{
    using namespace Poco;
    using namespace Poco::XML;

    // file mapping (for archives)
    FileMapping fileMapping;
    Document *document = new Document;
    Element *root = document->createElement(ioConfig.rootName());
    document->appendChild(root);
    root->setAttribute("version", ioConfig.version().shortVersion());
    root->setAttribute("interface", interfaceToString(uiInterface));
    Element *tmp;
    if (ioConfig.hasMetadata())
    {
        tmp = document->createElement("metadata");
        root->appendChild(tmp);
        mMetadata.toXML(tmp);
    }
    tmp = document->createElement(ioConfig.plotName());
    root->appendChild(tmp);
    tPlot.toXML(ioConfig, tmp, fileMapping);
    tmp = document->createElement("notes");
    root->appendChild(tmp);
    tmp->appendChild(document->createTextNode(sNotes));
    tmp = document->createElement(ioConfig.propertiesName());
    root->appendChild(tmp);
    lProperties.toXML(ioConfig, tmp);
    tmp = document->createElement("characters");
    root->appendChild(tmp);
    lCharacters.toXML(ioConfig, tmp);
    tmp = document->createElement("history");
    root->appendChild(tmp);
    tHistory.toXML(ioConfig, tmp, fileMapping);
    tmp = document->createElement("music");
    root->appendChild(tmp);
    tMusic.toXML(ioConfig, tmp, fileMapping);
    tmp = document->createElement("effects");
    root->appendChild(tmp);
    tEffects.toXML(ioConfig, tmp, fileMapping);
    // saved XML file and temporary directory (if needed)
    string xmlFile(fileName);
    File tempDir;
    if (ioConfig.isArchived())
    {
        tempDir = TemporaryFile::tempName();
        tempDir.createDirectory();
        TemporaryFile::registerForDeletion(tempDir.path());
        xmlFile = tempDir.path() + "/scenario.xml";        
    }
    DOMWriter writer;
    writer.setNewLine("\n");
    UTF8Encoding encoding;
    writer.setEncoding("UTF-8", encoding);
    writer.setOptions(XMLWriter::WRITE_XML_DECLARATION | XMLWriter::PRETTY_PRINT);
    FileOutputStream outputXML(xmlFile.c_str());
    writer.writeNode(outputXML, document);
    outputXML.close();
    if (ioConfig.isArchived())
    {
        // adding files to the archive
        for (FileMapping::Iterator it = fileMapping.begin(); it != fileMapping.end(); it++)
        {
            File file(it.file());
            // only if the file exists and is readable
            if (file.exists() && file.canRead())
            {
                Path path(tempDir.path());
                path.append(it.destination());
                File(path.parent()).createDirectory();
                file.copyTo(path.toString());
            }
        }
        // compressing it
        FileOutputStream output(fileName.c_str());
        Zip::Compress comp(output, true);
        comp.addRecursive(tempDir.path());
        comp.close();
        tempDir.remove(true);
    }

    document->release();
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

Scenario::UserInterface Scenario::stringToInterface(const std::string& interface)
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
