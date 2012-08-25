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
#ifndef CONFIGWRAPPER_H
#define CONFIGWRAPPER_H

#include <QObject>
#include <KUrl>

#include <Plasma/ConfigLoader>


/** This class exposes the lightdm-kde config to QML*/

class ConfigWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ConfigWrapper(const KUrl &configPath, QObject *parent = 0);

    Q_INVOKABLE QVariant readEntry(const QString &key) const;

private:
    Plasma::ConfigLoader *m_config;

};

#endif // CONFIGWRAPPER_H
