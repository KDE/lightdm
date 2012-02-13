#include "coreconfig.h"
#include "ui_coreconfig.h"

#include "config.h"

#include <QLightDM/SessionsModel>
#include <QLightDM/UsersModel>

#include <KAuth/Action>
#include <KConfig>
#include <KConfigGroup>
#include <KDebug>

CoreConfig::CoreConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoreConfig)
{
    ui->setupUi(this);

    ui->autoLoginUser->setModel(new QLightDM::UsersModel(this));
    ui->autoLoginSession->setModel(new QLightDM::SessionsModel(this));

    loadFromConfig();

    connect(ui->allowGuest, SIGNAL(toggled(bool)), SIGNAL(changed()));
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

    ui->allowGuest->setChecked(config.group("SeatDefaults").readEntry("allow-guest", true));

    ui->enableXdmcp->setChecked(config.group("XDMCPServer").readEntry("enabled", false));
    ui->enableVnc->setChecked(config.group("VNCServer").readEntry("enabled", false));
}

QVariantMap CoreConfig::save()
{
    QVariantMap args;
    args["core/SeatDefaults/allow-guest"] = ui->allowGuest->isChecked();
    args["core/XDMCPServer/enabled"] = ui->enableXdmcp->isChecked();
    args["core/VNCServer/enabled"] = ui->enableVnc->isChecked();
    return args;
}
