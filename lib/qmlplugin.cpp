/*
    Copyright (C) 2012  David Edmundson <kde@davidedmundson.co.uk>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "qmlplugin.h"

#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>

#include <QDebug>

#include <QLightDM/Power>

#include "sessionsmodel.h"
#include "usersmodel.h"
#include "screensmodel.h"
#include "fakegreeter.h"
#include "configwrapper.h"

void QmlPlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri) {
    Q_UNUSED(uri)

    if (engine->rootContext()->contextProperty("greeter").isNull()) {
        engine->rootContext()->setContextProperty("greeter", new FakeGreeter(0));
    }

    if (engine->rootContext()->contextProperty("config").isNull()) {
        KUrl configPath(engine->baseUrl());
        configPath.setPath(configPath.path() + "/main.xml");
//        engine->rootContext()->setContextProperty(QLatin1String("config"), new ConfigWrapper(configPath);
    }
}

void QmlPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<SessionsModel> (uri, 0, 1, "SessionsModel");
    qmlRegisterType<UsersModel> (uri, 0, 1, "UsersModel");
    qmlRegisterType<ScreensModel> (uri, 0, 1, "ScreensModel");
    
    qmlRegisterType<QLightDM::PowerInterface> (uri, 0, 1, "Power");
}

Q_EXPORT_PLUGIN2(klightdm, QmlPlugin);
