#include "lightdmkcm.h"

#include <KAboutData>
#include <KAuth/Action>
#include <KAuth/ActionReply>
#include <KDebug>
#include <KTabWidget>
#include <KLocalizedString>
#include <KPluginFactory>

#include <QHBoxLayout>

#include "../about.h"
#include "themeconfig.h"
#include "coreconfig.h"

K_PLUGIN_FACTORY(LightDMKcmFactory, registerPlugin<LightDMKcm>();)
K_EXPORT_PLUGIN(LightDMKcmFactory("kcm_lightdm", "kcm_lightdm"))


LightDMKcm::LightDMKcm(QWidget *parent, const QVariantList &args) :
    KCModule(LightDMKcmFactory::componentData(), parent, args)
{
    KAboutData* aboutData = new KAboutData(
        "kcmlightdm",                // appName
        0,                           // catalogName
        ki18n("LightDM KDE Config"), // programName
        "0");                        // version (set by initAboutData)

    initAboutData(aboutData);

    setAboutData(aboutData);

    setNeedsAuthorization(true);

    QHBoxLayout* layout = new QHBoxLayout(this);
    KTabWidget* tabWidget = new KTabWidget(this);
    layout->addWidget(tabWidget);

    m_coreConfig = new CoreConfig(this);
    m_themeConfig = new ThemeConfig(this);

    connect(m_themeConfig, SIGNAL(changed(bool)), SIGNAL(changed(bool)));
    connect(m_coreConfig, SIGNAL(changed(bool)), SIGNAL(changed(bool)));

    tabWidget->addTab(m_themeConfig, i18n("Theme"));
    tabWidget->addTab(m_coreConfig, i18n("General"));
}

void LightDMKcm::save()
{
    QVariantMap args;

    args = m_themeConfig->save();
    args.unite(m_coreConfig->save());

    KAuth::Action saveAction("org.kde.kcontrol.kcmlightdm.save");
    saveAction.setHelperID("org.kde.kcontrol.kcmlightdm");
    saveAction.setArguments(args);
    KAuth::ActionReply reply = saveAction.execute();
    if (reply.failed()) {
        // FIXME: Show a message here
        kWarning() << "save failed:" << reply.errorDescription();
    } else {
        changed(false);
    }
}
