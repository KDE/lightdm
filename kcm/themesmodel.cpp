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
#include "themesmodel.h"

#include "lightdmpackagestructure.h"

#include <QString>
#include <QPixmap>
#include <QList>
#include <QDir>
#include <QSettings>

#include <KStandardDirs>
#include <KGlobal>
#include <KDebug>

#include <Plasma/Package>


class ThemeItem {
public:
    QString id;
    QString name;
    QString description;
    QString author;
    QString version;
    QPixmap preview;
    QString path;
};

ThemesModel::ThemesModel(QObject *parent) :
    QAbstractListModel(parent)
{
    this->load();
}

ThemesModel::~ThemesModel()
{
}

int ThemesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_themes.size();
}

QVariant ThemesModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    const Plasma::Package package(m_themes[row]);
    const Plasma::PackageMetadata metaData = package.metadata();

    switch(role) {
    case Qt::DisplayRole:
        return metaData.name();
    case Qt::DecorationRole:
        /* Drop Through*/
    case ThemesModel::PreviewRole:
        return QPixmap(package.filePath("preview"));
    case ThemesModel::IdRole:
        return metaData.pluginName();
    case ThemesModel::DescriptionRole:
        return metaData.description();
    case ThemesModel::VersionRole:
        return metaData.version();
    case ThemesModel::AuthorRole:
        return metaData.author();
    case ThemesModel::PathRole:
        return package.path();
    }

    return QVariant();
}

void ThemesModel::load()
{
    qDebug() << "loading themes";
    QStringList themeDirPaths = KGlobal::dirs()->findDirs("data", "lightdm-kde-greeter/themes");

    foreach(const QString &themeName, Plasma::Package::listInstalledPaths(themeDirPaths.last()))
    {
        //A new package structure is apparently needed for each new package, otherwise instances of different packages represent the same object in a horrendously broken way
        Plasma::PackageStructure::Ptr packageStructure(new LightDMPackageStructure(this));

        Plasma::Package package(themeDirPaths.last(), themeName, packageStructure);
        if (package.isValid()) {
            addTheme(package);
        }
    }
}

void ThemesModel::addTheme(const Plasma::Package &package)
{
    beginInsertRows(QModelIndex(), m_themes.size(), m_themes.size());
    m_themes.append(package);
    endInsertRows();
}
