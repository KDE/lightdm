#include "lightdmkcm.h"

#include <KTabWidget>
#include <KLocalizedString>
#include <KPluginFactory>

#include <QHBoxLayout>

#include "themeconfig.h"
#include "coreconfig.h"

K_PLUGIN_FACTORY(LightDMKcmFactory, registerPlugin<LightDMKcm>();)
K_EXPORT_PLUGIN(LightDMKcmFactory("kcm_lightdm", "kcm_lightdm"))


LightDMKcm::LightDMKcm(QWidget *parent, const QVariantList &args) :
    KCModule(LightDMKcmFactory::componentData(), parent, args)
{
    setNeedsAuthorization(true);

    QHBoxLayout* layout = new QHBoxLayout(this);
    KTabWidget* tabWidget = new KTabWidget(this);
    layout->addWidget(tabWidget);

    m_coreConfig = new CoreConfig(this);
    m_themeConfig = new ThemeConfig(this);

    tabWidget->addTab(m_themeConfig, i18n("Theme"));
    tabWidget->addTab(m_coreConfig, i18n("General"));
}

void LightDMKcm::onSave()
{
    m_themeConfig->save();
}
