#ifndef HEADER_SKILL
#define HEADER_SKILL

#include <string>

class Skill
{
    private:
        std::string sName;
        int iValue;
    public:
        // constructor
        Skill(const std::string &name, int value);
        // accessors
        std::string name() const;
        int value() const;
        void setValue(int value);
};

#endif
