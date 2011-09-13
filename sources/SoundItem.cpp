#include "SoundItem.h"

using namespace std;

SoundItem::SoundItem(const string &content, State state): Item(content,state)
{
}

Item::Type SoundItem::type() const
{
    return tSound;
}
