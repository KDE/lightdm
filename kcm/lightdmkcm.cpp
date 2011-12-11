#include "lightdmkcm.h"

#include <KTabWidget>
#include <KLocalizedString>

#include <QHBoxLayout>

#include "themeconfig.h"
#include "coreconfig.h"


LightDMKcm::LightDMKcm(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    KTabWidget* tabWidget = new KTabWidget(this);
    layout->addWidget(tabWidget);

    CoreConfig *config = new CoreConfig(this);
    ThemeConfig *themeConfig = new ThemeConfig(this);

    tabWidget->addTab(themeConfig, i18n("Theme"));
    tabWidget->addTab(config, i18n("General"));
}
