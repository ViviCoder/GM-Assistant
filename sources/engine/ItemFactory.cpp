#include "ItemFactory.h"

Item* ItemFactory::copyItem(Item *item)
{
    switch (item->type())
    {
        case Item::tBasic:    return new Item(*item);   break;
        case Item::tSound:    return new SoundItem(*dynamic_cast<SoundItem*>(item));    break;
    }
}

Item* ItemFactory::createItem(Item::Type type, const std::string &content, Item::State state)
{
    switch (type)
    {
        case Item::tBasic:    return new Item(content,state);   break;
        case Item::tSound:    return new SoundItem(content,state);    break;
    }
}
