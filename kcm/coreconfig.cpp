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

    loadFromConfig();
}

CoreConfig::~CoreConfig()
{
    delete ui;
}

void CoreConfig::loadFromConfig()
{
    KConfig config((LIGHTDM_CONFIG_DIR "/lightdm.conf"));

    ui->allowGuest->setChecked(config.group("SeatDefaults").readEntry("allow-guest", true));

    ui->enableXdmcp->setChecked(config.group("XDMCPServer").readEntry("enabled", false));
    ui->enableVnc->setChecked(config.group("VNCServer").readEntry("enabled", false));
}
