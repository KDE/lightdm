#include "configwrapper.h"

#include "config.h"

#include <KSharedConfig>
#include <KConfigGroup>


ConfigWrapper::ConfigWrapper(QObject *parent) :
    QObject(parent)
{
    m_config = KSharedConfig::openConfig(LIGHTDM_CONFIG_DIR "/lightdm-kde-greeter.conf", KConfig::SimpleConfig);
}

QVariant ConfigWrapper::readEntry(const QString &key, const QVariant &aDefault) const
{
    KConfigGroup configGroup = m_config->group("greeter");
    return configGroup.readEntry(key, aDefault);
}
