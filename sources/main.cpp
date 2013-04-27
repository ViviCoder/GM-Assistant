/*************************************************************************
* Copyright © 2011-2013 Vincent Prat & Simon Nicolas
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*************************************************************************/

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QTextCodec>
#include <QLibraryInfo>
#include <QSettings>
#include <QDir>

#include "MainWindow.h"

#define MAX_LENGTH  256
#define APPLICATION_NAME    "gm-assistant"

#undef main

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // encoding
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    // initializing settings' stuff
    QCoreApplication::setOrganizationName(APPLICATION_NAME);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    // getting install directory (on Linux only)
    QString install_dir;
#ifdef __linux__
    #ifdef SYSTEM_INSTALL
    QSettings settings(QSettings::SystemScope,APPLICATION_NAME,APPLICATION_NAME);
    #else
    QSettings settings;
    #endif
    settings.beginGroup("directories");
    install_dir = settings.value("install").toString();
    if (!install_dir.isEmpty())
    {
        install_dir += "/";
    }
    settings.endGroup();
#endif
#ifdef _WIN32
	QDir home = QDir::home();
	QString dirName = "GM-Assistant";
	if (home.exists(dirName) || home.mkdir(dirName))
	{
		QSettings settings;
		settings.beginGroup("directories");
		if (settings.value("work").toString().isEmpty())
		{
			settings.setValue("work", QDir::homePath() + "/" + dirName);
		}
		settings.endGroup();
	}
#endif

    QString locale = QLocale::system().name().section('_',0,0);
    QTranslator *translator, *translatorSys;
    // Translation of the software
    translator = new QTranslator(&app);
    translator->load(install_dir + "translations/gmassistant_" + locale);
    app.installTranslator(translator);
    // Translation of predefined Qt strings
    translatorSys = new QTranslator(&app);
    translatorSys->load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(translatorSys);

    // Display of the main window
    MainWindow main(translator, translatorSys);
    main.show();

    return app.exec();
}
