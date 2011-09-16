#ifndef HEADER_CHARACTER
#define HEADER_CHARACTER

#include <vector>
#include <libxml++/libxml++.h>
#include <stdexcept>
#include "Skill.h"

class Character
{
    private:
        std::string sName;
        std::string sPlayerName;
        std::vector<Skill> vSkill;
    public:
        // constructors
        Character(const std::string &name);    // for NPC
        Character(const std::string &name, const std::string &playerName);    // for PC
        // XML-related methods
        void toXML(xmlpp::Element &root) const;
        void fromXML(const xmlpp::Element &root);
        // accessors
        std::string name() const;
        std::string playerName() const;
        bool isPlaying() const;
        Skill& skill(int index) throw(std::out_of_range);
        // populating
        void addSkill(const Skill &skill);
        void removeSkill(int index) throw(std::out_of_range);
        void clearSkills();
};

#endif
