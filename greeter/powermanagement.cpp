/*************************************************************************************
 *  Copyright (C) 2012 by Alejandro Fiestas Olivares <afiestas@kde.org>              *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/

#include "powermanagement.h"

static const QString interface("org.freedesktop.UPower");
static const QString path("/org/freedesktop/UPower");

PowerManagement::PowerManagement(QObject* parent)
    : QObject(parent)
{
    m_upower = new org::freedesktop::UPower(interface, path, QDBusConnection::systemBus(), this);
    m_lidIsClosed = m_upower->lidIsClosed();
    connect(m_upower, SIGNAL(Changed()), SLOT(upowerChanged()));
}

PowerManagement::~PowerManagement()
{
}

void PowerManagement::upowerChanged()
{
    bool lidIsClosed = m_upower->lidIsClosed();

    //if lid has changed to closed from not closed, suspend
    if (lidIsClosed && !m_lidIsClosed) {
        m_upower->Suspend();
    }
        
    m_lidIsClosed = lidIsClosed;
}
