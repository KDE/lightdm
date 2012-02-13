// KDE
#include <KCmdLineArgs>
#include <KApplication>
#include <KAboutData>
#include <KLocale>

#include "../about.h"
#include "greeterwindow.h"

int main(int argc, char *argv[])
{
    KAboutData aboutData(
        "lightdm-kde-greeter",        // appName
        0,                            // catalogName
        ki18n("LightDM KDE Greeter"), // programName
        "0");                         // version (set by initAboutData)

    initAboutData(&aboutData);

    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    KCmdLineArgs::addCmdLineOptions(options);
    KCmdLineArgs::parsedArgs();
    KApplication app;

    GreeterWindow *w = new GreeterWindow();
    w->show();

    return app.exec();
}

