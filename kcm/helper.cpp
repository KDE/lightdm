#include "helper.h"

#include <KDebug>
#include <KConfig>
#include <KConfigGroup>

#include <QFile>

#include "config.h"

static QSharedPointer<KConfig> openConfig()
{
    QFile file(LIGHTDM_CONFIG_DIR "/lightdm-kde-greeter.conf");
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

KAuth::ActionReply Helper::savetheme(const QVariantMap &args)
{
    KAuth::ActionReply reply;

    kDebug() << "saving theme" << args["theme-name"];

    QSharedPointer<KConfig> config = openConfig();
    config->group("greeter").writeEntry("theme-name", args["theme-name"]);
    config->sync();

    //FIXME check result somehow...

    return reply;
}

KAuth::ActionReply Helper::savethemedetails(const QVariantMap &args)
{
    KAuth::ActionReply reply;
    QSharedPointer<KConfig> config = openConfig();
    KConfigGroup configGroup = config->group("greeter-details");

    QMap<QString, QVariant>::const_iterator i;
    for (i = args.constBegin() ; i != args.constEnd() ; i++) {
        configGroup.writeEntry(i.key(), i.value());
    }
    config->sync();

    return reply;
}


KDE4_AUTH_HELPER_MAIN("org.kde.kcontrol.kcmlightdm", Helper);

#include "moc_helper.cpp"
