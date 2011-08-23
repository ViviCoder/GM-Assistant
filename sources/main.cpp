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
    List list;
    list.insert(0,"Premier élément",sNone);
    list.insert(1,"n'importe quoi",sSuccess);
    list.insert(1,"deuxième (en fait troisième)",sProgress);
    list.insert(0,"Avant premier élément",sFailure);
    list.insert(3,"encore un", sSuccess);
    list.insert(3,"élément à supprimer",sProgress);
    list.remove(3);
    list.move(3,2);
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

    list.clear();
    for (List::iterator it = list.begin(); it != list.end(); it++)
    {
        cout << (*it).first << endl;    
    }
    cout << endl;

    Tree tree;
    tree.insert("0","Hein ?",sNone);
    tree.insert("0_0","Quoi ?",sNone);
    tree.insert("0_1","Oui ?",sSuccess);
    tree.insert("0_2","Ben non...",sProgress);
    tree.insert("1","Échec !",sFailure);
    tree.insert("2","Branche",sSuccess);
    tree.insert("2_0","Encore une feuille",sProgress);
    tree.insert("2_0","Feuille",sNone);
    tree.insert("3","Autre branche",sFailure);
    tree.insert("3_0","Grande feuille",sProgress);
    tree.insert("3_0_0","Et encore une !",sFailure);
    tree.insert("3_1","Bouh",sNone);
    tree.insert("3_2","Re-bouh",sNone);
    tree.insert("3_2_0","Boubou",sSuccess);
    
    tree.remove("3_1");
    tree.move("3_1","3_0");
    tree.move("2","1_0");

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
