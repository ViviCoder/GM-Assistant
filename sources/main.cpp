#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QTextCodec>
#include <QLibraryInfo>
#include <iostream>

#include "MainWindow.h"
#include "Engine.h"

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // encoding
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QString locale = QLocale::system().name().section('_',0,0);
    QTranslator translator,translatorSys;
    // Translation of the software
    translator.load(QString("gmassistant_") + locale);
    app.installTranslator(&translator);
    // Translation of predefined Qt strings
    translatorSys.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translatorSys);

    // Display of the main window
    MainWindow main;
    main.show();

    return app.exec();
}
