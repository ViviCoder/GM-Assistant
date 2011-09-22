#ifndef HEADER_CHARACTER
#define HEADER_CHARACTER

#include <vector>
#include <libxml++/libxml++.h>
#include <stdexcept>

class Character
{
    private:
        std::string sName;
        std::string sPlayerName;
        //std::vector<Skill> vSkills;
        std::vector<std::string> vSkills;
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
        std::string& skill(int index) throw(std::out_of_range);
        // populating
        void addSkill(const std::string &skill);
        void removeSkill(int index) throw(std::out_of_range);
        void clearSkills();
};

#endif
