#include "configwrapper.h"

#include "config.h"

#include <KSharedConfig>
#include <KConfigGroup>

#include <QDebug>

ConfigWrapper::ConfigWrapper(QObject *parent) :
    QObject(parent)
{
    m_config = KSharedConfig::openConfig(LIGHTDM_CONFIG_DIR "/lightdm-kde-greeter.conf", KConfig::SimpleConfig);
}

QVariant ConfigWrapper::readEntry(const QString &key, const QVariant &aDefault) const
{
    //FIXME I should use a KConfigSkeleton which loads the KCFG, then remove the "default" parameter

    KConfigGroup configGroup = m_config->group("greeter-details");

    return configGroup.readEntry(key, aDefault);
}
