#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <iostream>

#include "MainWindow.h"
#include "List.h"

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

    // Display of the main window
    MainWindow main;
    main.show();

    return app.exec();
}
