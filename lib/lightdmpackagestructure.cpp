#include "lightdmpackagestructure.h"

LightDMPackageStructure::LightDMPackageStructure(QObject *parent) :
    Plasma::PackageStructure(parent, i18n("LightDM Theme"))
{
    setServicePrefix("lightdm-theme");
    setDefaultPackageRoot("lightdm-kde-greeter/themes/");

    addFileDefinition("configfile", "config/main.xml", i18nc("File Definition", "Config structure"));
    addFileDefinition("configui", "config/config.ui", i18nc("File Definition", "Config UI"));
    addFileDefinition("preview", "preview.png", i18nc("File Definition", "Image of theme"));

    addFileDefinition("mainscript", "ui/main.qml", i18nc("File Definition", "Main theme script"));
    setRequired("mainscript", true);

}
