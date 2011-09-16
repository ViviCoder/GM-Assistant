#include "Skill.h"

using namespace std;

// constructor

Skill::Skill(const string &name, int value): sName(name), iValue(value)
{
}

// accessors

string Skill::name() const
{
    return sName;
}

int Skill::value() const
{
    return iValue;
}

void Skill::setValue(int value)
{
    iValue = value;
}
