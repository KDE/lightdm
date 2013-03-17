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
#ifndef THEMECONFIG_H
#define THEMECONFIG_H

#include <QWidget>

#include <KSharedConfig>

namespace Ui {
    class ThemeConfig;
}

class QDir;
class QModelIndex;
class ThemesModel;

class ThemeConfig : public QWidget
{
    Q_OBJECT

public:
    explicit ThemeConfig(QWidget *parent = 0);
    ~ThemeConfig();

    QVariantMap save();
    void defaults();

signals:
    void changed(bool);

private slots:
    void onThemeSelected(const QModelIndex &index);
    void onInstallThemeButtonPressed();

private:
    Ui::ThemeConfig *ui;
    KSharedConfigPtr m_config;

    ThemesModel *m_themesModel;
    QDir themeDir() const;
    QModelIndex findIndexForTheme(const QString& theme) const;
};

#endif // THEMECONFIG_H
