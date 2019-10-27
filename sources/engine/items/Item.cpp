/*************************************************************************
* Copyright © 2011-2019 Vincent Prat & Simon Nicolas
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

#include "Item.h"

using namespace std;

Item::Item(const string &content, State state, bool expanded): sState(state), sContent(content), bExpanded(expanded)
{
}

Item::~Item()
{
}

string Item::content() const
{
    return sContent;
}

void Item::setContent(const string &content)
{
    sContent = content;
}

Item::State Item::state() const
{
    return sState;
}

void Item::setState(State state)
{
    sState = state;
}

Item::Type Item::type() const
{
    return tBasic;
}

void Item::fromXML(const IOConfig&, const xmlpp::Element&, bool)
{
}

void Item::toXML(const IOConfig&, xmlpp::Element&, FileMapping&)
{
}

string Item::stateToStr(State state)
{
    switch (state)
    {
        case sNone: return "none"; break;
        case sProgress: return "progress";  break;
        case sFailure:  return "failure";   break;
        case sSuccess:  return "success";   break;
    }
    return "";
} 

string Item::typeToStr(Type type, const IOConfig &config)
{
    switch (type)
    {
        case tFile:     return "file"; break;
        case tSound:    return "sound"; break;
        case tImage:    if (config.hasImages())
                        {
                            return config.imageName();
                            break;
                        }
        case tBasic:    return "basic"; break;
    }
    return "";
}

Item::State Item::strToState(const string &name)
{
    if (name=="none")   return sNone;
    else if (name=="progress")  return sProgress;
    else if (name=="failure")   return sFailure;
    else if (name=="success")   return sSuccess;
    else    throw xmlpp::exception("Unrecognized \""+name+"\" item state");
}

Item::Type Item::strToType(const string &name, const IOConfig &config)
{
    if (name=="basic")  return tBasic;
    else if (name=="file") return tFile;
    else if (name=="sound") return tSound;
    else if (config.hasImages() && name == config.imageName()) return tImage;
    else    throw xmlpp::exception("Unrecognized \""+name+"\" item type");
}

bool Item::is(Type type, Type isType)
{
    if (isType == tBasic)
    {
        // tBasic is the base type
        return true;
    }
    if (type == tBasic)
    {
        return false;
    }
    if (type == isType)
    {
        // types are the same
        return true;
    }
    // recursively
    return is(parentType(type), isType);
}

Item::Type Item::parentType(Type type)
{
    switch (type)
    {
        case tFile:     return tBasic;  break;
        case tSound:    return tFile;   break;
        case tImage:    return tFile;   break;
        default:        return tBasic;
    }    
}

bool Item::expanded() const
{
    return bExpanded;
}

void Item::setExpanded(bool expanded)
{
    bExpanded = expanded;
}

std::string Item::boolToStr(bool value)
{
    if (value)
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

bool Item::strToBool(const string &name)
{
    if (name == "true") return true;
    if (name == "false") return false;
    else    throw xmlpp::exception("Unrecognized \""+name+"\" boolean");
}
