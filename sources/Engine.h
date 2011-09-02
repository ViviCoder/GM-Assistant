#ifndef HEADER_ENGINE
#define HEADER_ENGINE

#include "Tree.h"
#include "List.h"

class Engine
{
    private:
        Tree tScenario;
        std::string sNotes;
//        std::vector<Character> vCharacters;
        List lHistory,lMusic,lEffects;
        List::iterator iMusic,iEffects;
    public:
        // constructors
        Engine();
        Engine(const std::string &fileName);
        // loading and saving methods
        void fromFile(const std::string &fileName);
        void toFile(const std::string &fileName) const;
        // accessors
        Tree& scenario();
        std::string& notes();
//        Character& character(int index);
        List& history();
        List& music();
        List& effects();
        // populating
//      addCharacter(const Character character&);
        void clear();
};

#endif
