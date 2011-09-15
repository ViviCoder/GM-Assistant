#ifndef HEADER_ENGINE
#define HEADER_ENGINE

#include "Tree.h"
#include "SoundEngine.h"
#include "Character.h"

class Engine
{
    private:
        Tree tScenario,tHistory,tMusic,tEffects;
        std::string sNotes;
        SoundEngine sSoundEngine;
        std::vector<Character> vCharacters;
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
        SoundEngine& soundEngine();
        Character& character(int index) throw(std::out_of_range);
        Tree& history();
        Tree& music();
        Tree& effects();
        // populating
        void addCharacter(const Character &character);
        void removeCharacter(int index) throw(std::out_of_range);
        void clear();
};

#endif
