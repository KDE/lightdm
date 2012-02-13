#include "helper.h"

#include <KDebug>
#include <KConfig>
#include <KConfigGroup>

#include <QFile>

#include "config.h"

enum WhichConfig
{
    CoreConfig,
    GreeterConfig
};


static QSharedPointer<KConfig> openConfig(WhichConfig which)
{
    QString name = QString("%1/%2")
        .arg(LIGHTDM_CONFIG_DIR)
        .arg(which == CoreConfig ? "lightdm.conf" : "lightdm-kde-greeter.conf")
        ;
    QFile file(name);
    if(!file.exists()) {
        // If we are creating the config file, ensure it is world-readable: if
        // we don't do that, KConfig will create a file which is only readable
        // by root
        file.open(QIODevice::WriteOnly);
        file.close();
        file.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::ReadOther);
    }
    return QSharedPointer<KConfig>(new KConfig(file.fileName(), KConfig::SimpleConfig));
}

KAuth::ActionReply Helper::savethemedetails(const QVariantMap &args)
{
    KAuth::ActionReply reply;
    QSharedPointer<KConfig> config = openConfig(GreeterConfig);
    KConfigGroup configGroup = config->group("greeter-details");

    QMap<QString, QVariant>::const_iterator i;
    for (i = args.constBegin() ; i != args.constEnd() ; i++) {
        configGroup.writeEntry(i.key(), i.value());
    }
    config->sync();

    return reply;
}

KAuth::ActionReply Helper::save(const QVariantMap &args)
{
    KAuth::ActionReply errorReply = KAuth::ActionReply::HelperErrorReply;
    QSharedPointer<KConfig> coreConfig = openConfig(CoreConfig);
    QSharedPointer<KConfig> greeterConfig = openConfig(GreeterConfig);

    QMap<QString, QVariant>::const_iterator i;
    for (i = args.constBegin() ; i != args.constEnd() ; i++) {
        QStringList lst = i.key().split("/");
        if (lst.size() != 3) {
            errorReply.setErrorDescription(QString("Invalid key format: %1").arg(i.key()));
            return errorReply;
        }
        QSharedPointer<KConfig> config;
        QString fileName = lst[0];
        QString groupName = lst[1];
        QString keyName = lst[2];

        if (fileName == "core") {
            config = coreConfig;
        } else if (fileName == "greeter") {
            config = greeterConfig;
        } else {
            errorReply.setErrorDescription(QString("Unknown config file: %1").arg(fileName));
            return errorReply;
        }

        config->group(groupName).writeEntry(keyName, i.value());
    }
    coreConfig->sync();
    greeterConfig->sync();

    return KAuth::ActionReply::SuccessReply;
}

KDE4_AUTH_HELPER_MAIN("org.kde.kcontrol.kcmlightdm", Helper);

#include "moc_helper.cpp"
