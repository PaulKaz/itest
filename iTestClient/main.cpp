/*******************************************************************
 This file is part of iTest
 Copyright (C) 2005-2016 Michal Tomlein

 iTest is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public Licence
 as published by the Free Software Foundation; either version 2
 of the Licence, or (at your option) any later version.

 iTest is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public Licence for more details.

 You should have received a copy of the GNU General Public Licence
 along with iTest; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 ********************************************************************/

#include "main.h"

#include <QSettings>
#include <QTranslator>
#include <QHostAddress>

int main(int argc, char *argv[])
{
#ifdef Q_OS_MAC
    QFont::insertSubstitution(".Lucida Grande UI", "Lucida Grande");
#endif

    MTApplication app(argc, argv);
    app.setApplicationName("iTestClient");
    app.setApplicationVersion(ITEST_VERSION);

    QString host = "";
    quint16 port = 0;

    if (app.arguments().size() == 5) {
        if (app.arguments().at(1) == "--host" && app.arguments().at(3) == "--port") {
            host = app.arguments().at(2);
            port = app.arguments().at(4).toUShort();
        }
    }


#ifdef Q_OS_WIN32
    if (QSysInfo::WindowsVersion > QSysInfo::WV_6_1)
        QApplication::setStyle("windowsxp");
#endif

    QSettings settings("Michal Tomlein", "iTest");
    QString lang = settings.value("lang").toString();
    if (lang.length() > 2)
        lang.clear();
    if (lang.isEmpty())
        lang = QLocale::system().name().split('_').first();
    if (lang != "en") {
        QTranslator *translator = new QTranslator;
        translator->load(QString("iTestClient-%1.qm").arg(lang), ":/i18n");
        app.installTranslator(translator);
    }

    if(port > 0) {
        QSettings settings("Michal Tomlein", "iTest");
        settings.setValue("client/serverName", host);
        settings.setValue("client/serverPort", port);
    }

    MainWindow *itest_window = new MainWindow;    
    app.setAppMainWindow(itest_window);
    itest_window->showFullScreen();
    itest_window->connectSocket();
    return app.exec();
}
