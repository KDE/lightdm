/*
This file is part of LightDM-KDE.

Copyright 2011, 2012 David Edmundson <kde@davidedmundson.co.uk>

LightDM-KDE is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LightDM-KDE is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LightDM-KDE.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "lightdmkcm.h"

#include <KAboutData>
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

    tabWidget->addTab(m_themeConfig, i18n("Theme"));
    tabWidget->addTab(m_coreConfig, i18n("General"));
}

void LightDMKcm::save()
{
    m_themeConfig->save();
}
