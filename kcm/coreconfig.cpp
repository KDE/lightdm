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

#include "config.h"

#include <QLightDM/SessionsModel>
#include <QLightDM/UsersModel>

#include <KConfig>
#include <KConfigGroup>


CoreConfig::CoreConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoreConfig)
{
    ui->setupUi(this);

    ui->autoLoginUser->setModel(new QLightDM::UsersModel(this));
    ui->autoLoginSession->setModel(new QLightDM::SessionsModel(this));

}

CoreConfig::~CoreConfig()
{
    delete ui;
}

void CoreConfig::loadFromConfig()
{
    KConfig config((LIGHTDM_CONFIG_DIR "/lightdm/lightdm.conf"));

    ui->allowGuest->setChecked(config.group("SeatDefaults").readEntry("allow-guest",false));


    ui->enableXdmcp->setChecked(config.group("XDMCPServer").readEntry("enabled", false));
    ui->enableVnc->setChecked(config.group("VNCServer").readEntry("enabled", false));
}
