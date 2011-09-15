#ifndef HEADER_ITEMFACTORY
#define HEADER_ITEMFACTORY

#include "Item.h"
#include "SoundItem.h"

class ItemFactory
{
    public:
        static Item* copyItem(Item *item);
        static Item* createItem(Item::Type type, const std::string &content, Item::State state);
};

#endif
