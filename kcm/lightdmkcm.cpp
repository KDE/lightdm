#include "lightdmkcm.h"

#include <KAboutData>
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
    KAboutData* aboutData =
        new KAboutData("kcmlightdm", 0, ki18n("LightDM KDE Config"),
                       "0.1", ki18n("Description here"),
                       KAboutData::License_GPL,
                       ki18n("(c) KDE"),
                       KLocalizedString(), "", "kde-devel@kde.org");
    setAboutData(aboutData);

    setNeedsAuthorization(true);

    QHBoxLayout* layout = new QHBoxLayout(this);
    KTabWidget* tabWidget = new KTabWidget(this);
    layout->addWidget(tabWidget);

    m_coreConfig = new CoreConfig(this);
    m_themeConfig = new ThemeConfig(this);

    connect(m_themeConfig, SIGNAL(changed(bool)), SIGNAL(changed(bool)));

    tabWidget->addTab(m_themeConfig, i18n("Theme"));
    tabWidget->addTab(m_coreConfig, i18n("General"));
}

void LightDMKcm::save()
{
    m_themeConfig->save();
}
