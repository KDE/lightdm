#include "lightdmkcm.h"

#include <KApplication>
#include <KCmdLineArgs>
#include <KAboutData>

int main(int argc, char *argv[])
{
    //this will all be discarded when turned into KCM

    KAboutData aboutData("lightdmconfig", 0, ki18n("LightDM KDE Config"),
                         "0.1", ki18n("Description here"),
                         KAboutData::License_GPL,
                         ki18n("(c) KDE"),
                         KLocalizedString(), "", "kde-devel@kde.org");


    KCmdLineArgs::init(argc, argv, &aboutData);
    KApplication a;
    LightDMKcm w;
    w.show();

    return a.exec();
}
