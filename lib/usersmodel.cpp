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

#include "usersmodel.h"

#include <QLightDM/UsersModel>
#include <QFile>

#include <KLocalizedString>

UsersModel::UsersModel(QObject *parent) :
    ExtraRowProxyModel(parent),
    m_showGuest(false)
{
    setSourceModel(new QLightDM::UsersModel(this));
}

//workaround for LightDM sometimes returning the wrong user icon path
//if the provided path does not exist, try path + ".icon" (i.e  ~/.face.icon)
//if that does exist, return that instead
QVariant UsersModel::data(const QModelIndex &index, int role) const
{
    if (role == QLightDM::UsersModel::ImagePathRole) {
        const QString path = ExtraRowProxyModel::data(index, role).toString();
        if (QFile::exists(path)) {
            return path;
        } else if (QFile::exists(path + QLatin1String(".icon"))) {
            return path + QLatin1String(".icon");
        }
    }
    return ExtraRowProxyModel::data(index, role);
}

void UsersModel::setShowGuest(bool showGuest)
{
    if (showGuest == m_showGuest) {
        return;
    }
    m_showGuest = showGuest;

    if (m_showGuest) {
        QStandardItem *guest = new QStandardItem(i18n("Guest"));
        guest->setData("*guest", QLightDM::UsersModel::NameRole);
        extraRowModel()->appendRow(guest);
    } else {
        extraRowModel()->removeRow(0);
    }
}

bool UsersModel::showGuest() const
{
    return m_showGuest;
}
