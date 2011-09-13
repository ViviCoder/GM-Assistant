#ifndef HEADER_SOUNDITEM
#define HEADER_SOUNDITEM

#include "Item.h"

class SoundItem: public Item
{
    public:
        // constructor
            SoundItem(const std::string &content="", State state=sNone);
        // accessors
        Type type() const;
};

#endif
