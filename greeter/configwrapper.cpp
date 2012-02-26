/*
This file is part of LightDM-KDE.

Copyright 2011, 2012 David Edmundson <kde@davidedmundson.co.uk>

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
#include "configwrapper.h"

#include "config.h"

#include <KSharedConfig>
#include <KConfigGroup>

#include <QDebug>

ConfigWrapper::ConfigWrapper(QObject *parent) :
    QObject(parent)
{
    m_config = KSharedConfig::openConfig(LIGHTDM_CONFIG_DIR "/lightdm-kde-greeter.conf", KConfig::SimpleConfig);
}

QVariant ConfigWrapper::readEntry(const QString &key, const QVariant &aDefault) const
{
    //FIXME I should use a KConfigSkeleton which loads the KCFG, then remove the "default" parameter

    KConfigGroup configGroup = m_config->group("greeter-settings");

    return configGroup.readEntry(key, aDefault);
}
