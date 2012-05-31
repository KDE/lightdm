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
#include "coreconfig.h"
#include "ui_coreconfig.h"
#include "usersmodel.h"

#include "config.h"

#include <QLightDM/SessionsModel>
#include <QLightDM/UsersModel>

#include <KAuth/Action>
#include <KConfig>
#include <KConfigGroup>
#include <KDebug>

static const char* GUEST_NAME = "*guest";

inline QVariant currentItemData(QComboBox* combo, int role)
{
    return combo->itemData(combo->currentIndex(), role);
}

inline void setCurrentItemFromData(QComboBox* combo, int role, const QVariant& data)
{
    int index = combo->findData(data, role);
    if (index == -1) {
        kWarning() << "Couldn't find" << data << "(role" << role << ") in combobox" << combo;
        return;
    }
    combo->setCurrentIndex(index);
}

CoreConfig::CoreConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoreConfig)
{
    ui->setupUi(this);

    // Hide for now: lightdm does not support those yet :/
    ui->autoLoginSessionLabel->hide();
    ui->autoLoginSession->hide();
    ui->autoLoginTimeoutLabel->hide();
    ui->autoLoginTimeout->hide();

    // Hide for now: implementation is not complete
    ui->serverWidget->hide();

    m_usersModel = new UsersModel(this);
    ui->autoLoginUser->setModel(m_usersModel);

    ui->autoLoginSession->setModel(new QLightDM::SessionsModel(this));

    loadFromConfig();

    connect(ui->allowGuest, SIGNAL(toggled(bool)), SIGNAL(changed()));
    connect(ui->allowGuest, SIGNAL(toggled(bool)), SLOT(onAllowGuestChanged(bool)));
    connect(ui->autoLogin, SIGNAL(toggled(bool)), SIGNAL(changed()));
    connect(ui->autoLoginUser, SIGNAL(currentIndexChanged(int)), SIGNAL(changed()));
    connect(ui->autoLoginSession, SIGNAL(currentIndexChanged(int)), SIGNAL(changed()));
    connect(ui->autoLoginTimeout, SIGNAL(valueChanged(int)), SIGNAL(changed()));
    connect(ui->enableXdmcp, SIGNAL(toggled(bool)), SIGNAL(changed()));
    connect(ui->enableVnc, SIGNAL(toggled(bool)), SIGNAL(changed()));
}

CoreConfig::~CoreConfig()
{
    delete ui;
}

void CoreConfig::loadFromConfig()
{
    KConfig config((LIGHTDM_CONFIG_DIR "/lightdm.conf"));

    KConfigGroup seatDefaultsGroup = config.group("SeatDefaults");
    ui->allowGuest->setChecked(seatDefaultsGroup.readEntry("allow-guest", true));

    m_usersModel->setShowGuest(ui->allowGuest->isChecked());

    QString user = seatDefaultsGroup.readEntry("autologin-user");
    if (user.isEmpty() && seatDefaultsGroup.readEntry("autologin-guest", false)) {
        // from user (string), guest (bool) to combobox user
        user = GUEST_NAME;
    }
    if (!user.isEmpty()) {
        setCurrentItemFromData(ui->autoLoginUser, QLightDM::UsersModel::NameRole, user);
    }
    ui->autoLogin->setChecked(!user.isEmpty());

    QString session = seatDefaultsGroup.readEntry("autologin-session");
    if (!session.isEmpty()) {
        setCurrentItemFromData(ui->autoLoginSession, QLightDM::SessionsModel::IdRole, session);
    }
    ui->autoLoginTimeout->setValue(seatDefaultsGroup.readEntry("autologin-user-timeout", 0) / 60);

    ui->enableXdmcp->setChecked(config.group("XDMCPServer").readEntry("enabled", false));
    ui->enableVnc->setChecked(config.group("VNCServer").readEntry("enabled", false));
}

QVariantMap CoreConfig::save()
{
    QVariantMap args;
    if (ui->autoLogin->isChecked()) {
        // from combobox user to user (string), guest (bool)
        QString user = currentItemData(ui->autoLoginUser, QLightDM::UsersModel::NameRole).toString();
        bool guest;
        if (user == GUEST_NAME) {
            guest = true;
            user.clear();
        } else {
            guest = false;
        }
        args["core/SeatDefaults/autologin-user"] = user;
        args["core/SeatDefaults/autologin-guest"] = guest;
        // If LightDM gains support for these settings before us, we should not overwrite them
        //args["core/SeatDefaults/autologin-session"] = currentItemData(ui->autoLoginSession, QLightDM::SessionsModel::IdRole);
        //args["core/SeatDefaults/autologin-user-timeout"] = ui->autoLoginTimeout->value() * 60;
    } else {
        args["core/SeatDefaults/autologin-user"].clear();
        args["core/SeatDefaults/autologin-guest"] = false;
    }
    args["core/SeatDefaults/allow-guest"] = ui->allowGuest->isChecked();
    args["core/XDMCPServer/enabled"] = ui->enableXdmcp->isChecked();
    args["core/VNCServer/enabled"] = ui->enableVnc->isChecked();
    return args;
}

void CoreConfig::onAllowGuestChanged(bool allow)
{
    m_usersModel->setShowGuest(allow);
}
