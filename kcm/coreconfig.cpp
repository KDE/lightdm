#include "coreconfig.h"
#include "ui_coreconfig.h"

#include "config.h"

#include <QLightDM/SessionsModel>
#include <QLightDM/UsersModel>

#include <KAuth/Action>
#include <KConfig>
#include <KConfigGroup>
#include <KDebug>

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

    ui->autoLoginUser->setModel(new QLightDM::UsersModel(this));
    ui->autoLoginSession->setModel(new QLightDM::SessionsModel(this));

    loadFromConfig();

    connect(ui->allowGuest, SIGNAL(toggled(bool)), SIGNAL(changed()));
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

    ui->allowGuest->setChecked(config.group("SeatDefaults").readEntry("allow-guest", true));

    KConfigGroup lightdmGroup = config.group("LightDM");
    QString user = lightdmGroup.readEntry("autologin-user");
    if (!user.isEmpty()) {
        setCurrentItemFromData(ui->autoLoginUser, QLightDM::UsersModel::NameRole, user);
    }
    ui->autoLogin->setChecked(!user.isEmpty());

    QString session = lightdmGroup.readEntry("autologin-session");
    if (!session.isEmpty()) {
        setCurrentItemFromData(ui->autoLoginSession, QLightDM::SessionsModel::IdRole, session);
    }
    ui->autoLoginTimeout->setValue(lightdmGroup.readEntry("autologin-user-timeout", 0) / 60);

    ui->enableXdmcp->setChecked(config.group("XDMCPServer").readEntry("enabled", false));
    ui->enableVnc->setChecked(config.group("VNCServer").readEntry("enabled", false));
}

QVariantMap CoreConfig::save()
{
    QVariantMap args;
    args["core/LightDM/autologin-user"] = ui->autoLogin->isChecked()
        ? currentItemData(ui->autoLoginUser, QLightDM::UsersModel::NameRole)
        : QString();
    args["core/LightDM/autologin-session"] = currentItemData(ui->autoLoginSession, QLightDM::SessionsModel::IdRole);
    args["core/LightDM/autologin-user-timeout"] = ui->autoLoginTimeout->value() * 60;
    args["core/SeatDefaults/allow-guest"] = ui->allowGuest->isChecked();
    args["core/XDMCPServer/enabled"] = ui->enableXdmcp->isChecked();
    args["core/VNCServer/enabled"] = ui->enableVnc->isChecked();
    return args;
}
