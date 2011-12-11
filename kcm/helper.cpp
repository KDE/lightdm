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


KDE4_AUTH_HELPER_MAIN("org.kde.kcontrol.kcmlightdm", Helper);

#include "moc_helper.cpp"
