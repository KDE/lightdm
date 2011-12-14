#include "helper.h"

#include <KDebug>
#include <KConfig>
#include <KConfigGroup>

#include "config.h"


KAuth::ActionReply Helper::savetheme(const QVariantMap &args)
{
    KAuth::ActionReply reply;

    kDebug() << "saving theme" << args["theme-name"];

    KConfig config(LIGHTDM_CONFIG_DIR "/lightdm-kde-greeter.conf");
    config.group("greeter").writeEntry("theme-name", args["theme-name"]);
    config.sync();

    //FIXME check result somehow...

    return reply;
}

KAuth::ActionReply Helper::savethemedetails(const QVariantMap &args)
{
    KAuth::ActionReply reply;
    KConfig config(LIGHTDM_CONFIG_DIR "/lightdm-kde-greeter.conf");
    KConfigGroup configGroup = config.group("greeter-details");

    QMap<QString, QVariant>::const_iterator i;
    for (i = args.constBegin() ; i != args.constEnd() ; i++) {
        configGroup.writeEntry(i.key(), i.value());
    }
    config.sync();

    return reply;
}


KDE4_AUTH_HELPER_MAIN("org.kde.kcontrol.kcmlightdm", Helper);

#include "moc_helper.cpp"
