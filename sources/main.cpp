#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <iostream>

#include "MainWindow.h"
#include "Engine.h"

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Translation of predefined Qt strings
    QString locale = QLocale::system().name().section('_',0,0);
    QTranslator translator;
    translator.load(QString("gmassistant_") + locale);
    app.installTranslator(&translator);

    Engine engine("game.xml");
    List &list = engine.music();
    list.clear();
    list.add("Premier morceau.mp3");
    list.add("Morceau déjà joué.mp3",sSuccess);
    list.add("Morceau non trouvé.mp3",sFailure);
    list.add("Morceau en cours.mp3",sProgress);

    List &list2 = engine.history();
    list2.fromXML("list.xml");

    for (List::iterator it = list.begin(); it != list.end(); it++)
    {
        cout << (*it).content() << endl;    
    }
    cout << endl;
    for (List::iterator it = list.beginUnchecked(); it != list.endUnchecked(); it++)
    {
        cout << (*it).content() << endl;    
    }
    cout << endl;
    for (List::iterator it = list.beginState(sSuccess); it != list.endState(sSuccess); it++)
    {
        cout << (*it).content() << endl;    
    }
    cout << endl;

//    list.toXML("list.xml");

    Tree &tree = engine.scenario();
    tree.fromXML("tree.xml");

    cout << tree["2"].content() << endl;
    tree["2"].setContent("Coucou");
    cout << tree["2"].content() << endl;
    Item item("Autre branche",sFailure);
    tree["2"] = item;
    cout << tree["2"].content() << endl << endl;

    for (Tree::iterator it = tree.begin(); it != tree.end(); it++)
    {
        cout << it.depth() << "-" << (*it).content() << endl;    
    }
    cout << endl;
    
    for (Tree::iterator it = tree.beginUnchecked(); it != tree.endUnchecked(); it++)
    {
        cout << it.depth() << "-" << (*it).content() << endl;    
    }
    cout << endl;

    for (Tree::iterator it = tree.beginState(sFailure); it != tree.endState(sFailure); it++)
    {
        cout << it.depth() << "-" << (*it).content() << endl;    
    }
    cout << endl;

//    tree.toXML("tree.xml");

    engine.toFile("game.xml");

    // Display of the main window
    MainWindow main;
    main.show();

    return app.exec();
}
