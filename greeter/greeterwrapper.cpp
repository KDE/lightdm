/*
This file is part of LightDM-KDE.

Copyright 2012 David Edmundson <kde@davidedmundson.co.uk>

LightDM-KDE is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LightDM-KDE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LightDM-KDE.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "greeterwrapper.h"

#include <KConfig>
#include <KConfigGroup>
#include <KDebug>

GreeterWrapper::GreeterWrapper(QObject *parent) :
    QLightDM::Greeter(parent)
{
    connectSync();
    KSharedConfig::Ptr config = KSharedConfig::openConfig("state-kde");

    //store a different config per display so last user is remembered per session
    m_configGroup = config->group(QLatin1String("lightdm_") + qgetenv("DISPLAY"));
}

QString GreeterWrapper::lastLoggedInUser() const
{
    //use suggested user from lightdm.conf if they exist, otherwise load from local config file of last logged in user.
    //if nothing exists, return nothing
    if (selectGuestHint()) {
        return "*guest";
    }
    if (!selectUserHint().isEmpty()) {
        return selectUserHint();
    }

    return m_configGroup.readEntry("lastUser");
}

QString GreeterWrapper::guestLoginName() const
{
    return QLatin1String("*guest");
}

bool GreeterWrapper::startSessionSync(const QString &session)
{
    Q_EMIT aboutToLogin();
    saveLastUser(authenticationUser());
    return QLightDM::Greeter::startSessionSync(session);
}

void GreeterWrapper::saveLastUser(const QString &user)
{
    m_configGroup.writeEntry("lastUser", user);
    //force a sync as our greeter gets killed
    m_configGroup.sync();
}
