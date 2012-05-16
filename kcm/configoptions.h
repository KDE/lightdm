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

#ifndef CONFIGOPTIONS_H
#define CONFIGOPTIONS_H

#include <QWidget>
#include <QDir>

#include <KSharedConfigPtr>
#include <KSharedConfig>

class AuthKitConfigLoader;
class KConfigDialogManager;

/** This class shows all config options for a given theme, and updates a shared KConfig*/

class ConfigOptions : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigOptions(QWidget *parent = 0);

    void setConfig(const KSharedConfigPtr &config);
    void setTheme(const QDir &themeDir);

    QVariantMap save();
    void defaults();

signals:
    void changed(bool);

public slots:

private slots:
    void onSettingsChanged();

private:
    KSharedConfigPtr m_config;

    QWeakPointer<QWidget> m_wrapperWidget;
    KConfigDialogManager* m_manager;
    AuthKitConfigLoader* m_configLoader;
};

#endif // CONFIGOPTIONS_H
