#include "Character.h"

using namespace std;

// constructors

Character::Character(const string &name): sName(name)
{
}

Character::Character(const string &name, const string &playerName): sName(name), sPlayerName(playerName)
{
}

// accessors

string Character::name() const
{
    return sName;
}

string Character::playerName() const
{
    return sPlayerName;
}

bool Character::isPlaying() const
{
    return sPlayerName == "";
}

void Character::toXML(xmlpp::Element &root) const
{
    // characteristics missing
}

void Character::fromXML(const xmlpp::Element &root)
{
    // idem
}
