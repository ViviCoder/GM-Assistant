#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <iostream>

#include "MainWindow.h"
#include "List.h"
#include "Tree.h"

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Translation of predefined Qt strings
    QString locale = QLocale::system().name().section('_',0,0);
    QTranslator translator;
    translator.load(QString("gmassistant_") + locale);
    app.installTranslator(&translator);

    // Tests for the List class
    List list("list.xml");
/*    list.add("Premier élément");
    list.add("n'importe quoi",sSuccess);
    list.insert(1,"deuxième (en fait troisième)",sProgress);
    list.insert(0,"Avant premier élément",sFailure);
    list.insert(3,"encore un", sSuccess);
    list.insert(3,"élément à supprimer",sProgress);
    list.remove(3);
    list.move(3,2);*/
    for (List::iterator it = list.begin(); it != list.end(); it++)
    {
        cout << (*it).first << endl;    
    }
    cout << endl;
    for (List::iterator it = list.beginUnchecked(); it != list.endUnchecked(); it++)
    {
        cout << (*it).first << endl;    
    }
    cout << endl;
    for (List::iterator it = list.beginState(sSuccess); it != list.end(); it++)
    {
        cout << (*it).first << endl;    
    }
    cout << endl;

//    list.toXML("list.xml");

    list.clear();
    for (List::iterator it = list.begin(); it != list.end(); it++)
    {
        cout << (*it).first << endl;    
    }
    cout << endl;

    Tree tree("tree.xml");
/*    tree.add("Hein ?");
    tree.addChild("Quoi ?");
    tree.add("Oui ?",sSuccess);
    tree.add("Ben non...",sProgress);
    tree.add(1,"Échec !",sFailure);
    tree.add("Branche",sSuccess);
    tree.addChild("Encore une feuille",sProgress);
    tree.insert("2_0","Feuille");
    tree.add(1,"Autre branche",sFailure);
    tree.addChild("Grande feuille",sProgress);
    tree.addChild("Et encore une !",sFailure);
    tree.add(2,"Bouh");
    tree.add("Re-bouh");
    tree.addChild("Boubou",sSuccess);
    
    tree.remove("3_1");
    tree.move("3_1","3_0");
    tree.move("2","1_0");*/

    for (Tree::iterator it = tree.begin(); it != tree.end(); it++)
    {
        cout << it.depth() << "-" << (*it).first << endl;    
    }
    cout << endl;
    
    for (Tree::iterator it = tree.beginUnchecked(); it != tree.endUnchecked(); it++)
    {
        cout << it.depth() << "-" << (*it).first << endl;    
    }
    cout << endl;

    for (Tree::iterator it = tree.beginState(sFailure); it != tree.endState(sFailure); it++)
    {
        cout << it.depth() << "-" << (*it).first << endl;    
    }
    cout << endl;

//    tree.toXML("tree.xml");

    tree.clear();
    
    for (Tree::iterator it = tree.begin(); it != tree.end(); it++)
    {
        cout << it.depth() << "-" << (*it).first << endl;    
    }
    cout << endl;

    // Display of the main window
    MainWindow main;
    main.show();

    return app.exec();
}
