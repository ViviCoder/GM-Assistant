/*************************************************************************
* Copyright © 2011-2018 Vincent Prat & Simon Nicolas
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
#include <QTextCodec>
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

    // initializing settings' stuff
    QCoreApplication::setOrganizationName(APPLICATION_NAME);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    // icon visibility in menus
    app.setAttribute(Qt::AA_DontShowIconsInMenus, false);

    // getting install directory
    QString install_dir(QCoreApplication::applicationDirPath());
#ifdef __linux__
    install_dir += "/../share/games/gm-assistant";
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
    if (!install_dir.isEmpty())
    {
        install_dir += "/";
    }
    
    // Display of the main window
    MainWindow main(install_dir);
    main.show();

    return app.exec();
}
