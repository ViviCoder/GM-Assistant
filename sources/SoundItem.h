#ifndef HEADER_SOUNDITEM
#define HEADER_SOUNDITEM

#include "Item.h"

class SoundItem: public Item
{
    private:
        std::string sFileName;
    public:
        // constructor
            SoundItem(const std::string &content="", State state=sNone);
        // accessors
        Type type() const;  // overriden
        std::string fileName() const;
        void setFileName(const std::string &fileName);
        // overriden XML-related methods
        void fromXML(const xmlpp::Element &root) throw(xmlpp::exception);
        void toXML(xmlpp::Element &root);
};

#endif
