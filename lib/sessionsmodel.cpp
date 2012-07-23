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

#include "sessionsmodel.h"

#include <QLightDM/SessionsModel>

#include <KLocalizedString>

SessionsModel::SessionsModel(QObject *parent) :
    ExtraRowProxyModel(parent),
    m_showLastUsedSession(false)
{
    setSourceModel(new QLightDM::SessionsModel(this));


}

void SessionsModel::setShowLastUsedSession(bool showLastUsedSession)
{
    if (showLastUsedSession == m_showLastUsedSession) {
        return;
    }
    m_showLastUsedSession = showLastUsedSession;

    if (m_showLastUsedSession) {
        QStandardItem *guest = new QStandardItem(i18n("Previously Used Session"));
        guest->setData("", QLightDM::SessionsModel::KeyRole);
        extraRowModel()->appendRow(guest);
    } else {
        extraRowModel()->removeRow(0);
    }
}

bool SessionsModel::showLastUsedSession() const
{
    return m_showLastUsedSession;
}
