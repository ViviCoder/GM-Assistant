#ifndef HEADER_ENGINE
#define HEADER_ENGINE

#include "Tree.h"

class Engine
{
    private:
        Tree tScenario,tHistory,tMusic,tEffects;
        std::string sNotes;
//        std::vector<Character> vCharacters;
    public:
        // constructors
        Engine();
        Engine(const std::string &fileName) throw(xmlpp::exception);
        // loading and saving methods
        void fromFile(const std::string &fileName) throw(xmlpp::exception);
        void toFile(const std::string &fileName) const;
        // accessors
        Tree& scenario();
        std::string& notes();
//        Character& character(int index);
        Tree& history();
        Tree& music();
        Tree& effects();
        // populating
//      addCharacter(const Character character&);
        void clear();
};

#endif
