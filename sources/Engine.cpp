#include "Engine.h"

using namespace std;

// constructors

Engine::Engine():iMusic(lMusic.begin()), iEffects(lEffects.begin())
{
}

Engine::Engine(const string &fileName) throw(xmlpp::exception): iMusic(lMusic.begin()), iEffects(lEffects.begin())
{
    try
    {
        fromFile(fileName);
    }
    catch (xmlpp::exception)
    {
        clear();
        throw;
    }
    iMusic = lMusic.begin();
    iEffects = lEffects.begin();
}

// methods

void Engine::fromFile(const std::string &fileName) throw(xmlpp::exception)
{
    using namespace xmlpp;

    DomParser parser(fileName);
    Document *document = parser.get_document();
    Element *root = document->get_root_node();
    if (root->get_name()!="game")
    {
        throw xmlpp::exception("Bad document content type: game expected");
    }
    // now loading the different parts of the game
    Node::NodeList node = root->get_children("scenario");
    if (!node.empty())
    {
        tScenario.fromXML(*dynamic_cast<Element*>(node.front()));
    }
    node = root->get_children("notes");
    if (!node.empty())
    {
        Element *elem = dynamic_cast<Element*>(node.front());
        if (elem->has_child_text())
        {
            sNotes = elem->get_child_text()->get_content();
        }
        else
        {
            sNotes = "";
        }
    }
    node = root->get_children("characters");
    // not yet implemented
    node = root->get_children("history");
    if (!node.empty())
    {
        lHistory.fromXML(*dynamic_cast<Element*>(node.front()));
    }
    node = root->get_children("music");
    if (!node.empty())
    {
        lMusic.fromXML(*dynamic_cast<Element*>(node.front()));
    }
    node = root->get_children("effects");
    if (!node.empty())
    {
        lEffects.fromXML(*dynamic_cast<Element*>(node.front()));
    }
}

void Engine::toFile(const string &fileName) const
{
    using namespace xmlpp;

    Document document;
    Element *root = document.create_root_node("game");
    Element *tmp = root->add_child("scenario");
    tScenario.toXML(*tmp);
    tmp = root->add_child("notes");
    tmp->add_child_text(sNotes);
    tmp = root->add_child("characters");
    // not yet implemented
    tmp = root->add_child("history");
    lHistory.toXML(*tmp);
    tmp = root->add_child("music");
    lMusic.toXML(*tmp);
    tmp = root->add_child("effects");
    lEffects.toXML(*tmp);
    document.write_to_file_formatted(fileName,"UTF-8");
}

// accessors

Tree& Engine::scenario()
{
    return tScenario;
}

string& Engine::notes()
{
    return sNotes;
}

List& Engine::history()
{
    return lHistory;
}

List& Engine::music()
{
    return lMusic;
}

List& Engine::effects()
{
    return lEffects;
}

void Engine::clear()
{
    tScenario.clear();
    sNotes = "";
    lHistory.clear();
    lMusic.clear();
    lEffects.clear();
    iMusic = lMusic.begin();
    iEffects = lEffects.begin();
}
