#ifndef LIGHTDMPACKAGESTRUCTURE_H
#define LIGHTDMPACKAGESTRUCTURE_H

#include <Plasma/PackageStructure>

class LightDMPackageStructure : public Plasma::PackageStructure
{
    Q_OBJECT
public:
    explicit LightDMPackageStructure(QObject *parent = 0, const QVariantList &args = QVariantList());
};

#endif // LIGHTDMPACKAGESTRUCTURE_H
