#include "configoptions.h"

#include <QFile>
#include <QUiLoader>
#include <QVBoxLayout>

#include <KConfigDialogManager>
#include <KDebug>

#include <kdemacros.h>

#include <Plasma/ConfigLoader>



class AuthKitConfigLoader : public Plasma::ConfigLoader {
public:
    AuthKitConfigLoader(KSharedConfigPtr config, QIODevice *xml, QObject *parent=0);
    QVariantMap entryMap() const;
protected:
    void usrWriteConfig();
private:
    QVariantMap m_entryMap;
};

AuthKitConfigLoader::AuthKitConfigLoader(KSharedConfigPtr config, QIODevice *xml, QObject *parent)
    : Plasma::ConfigLoader(config, xml, parent)
{}

//normal write fails due to needing root, worse it "readConfig" at the end of a write, deleting any values we once had
//we overrise the usrWrite event to save all settings to entry map then retrieve that.
void AuthKitConfigLoader::usrWriteConfig()
{
    m_entryMap.clear();
    foreach(KConfigSkeletonItem* item, items()) {
        m_entryMap["greeter/greeter-settings/" + item->key()] = item->property();
    }
}

QVariantMap AuthKitConfigLoader::entryMap() const
{
    return m_entryMap;
}


ConfigOptions::ConfigOptions(QWidget *parent) :
    QWidget(parent)
{
    new QVBoxLayout(this);
}

void ConfigOptions::setConfig(const KSharedConfigPtr &config)
{
    m_config = config;
}

void ConfigOptions::setTheme(const QDir &themeDir)
{
    if(KDE_ISUNLIKELY(m_config.isNull())) {
        kFatal() << "setConfig must be called before setTheme";
    }

    //delete existing widgets.
    if (!m_wrapperWidget.isNull()) {
        m_wrapperWidget.data()->deleteLater();
    }

    //if contains a valid config
    if (themeDir.exists(QLatin1String("main.xml"))
            && themeDir.exists(QLatin1String("config.ui"))) {
        QFile kcfgFile(themeDir.filePath(QLatin1String("main.xml")));
        kcfgFile.open(QFile::ReadOnly);

        QUiLoader loader;
        QFile uiFile(themeDir.filePath(QLatin1String("config.ui")));
        m_wrapperWidget = loader.load(&uiFile, this);

        //both the following get deleted when the wrapped widget is deleted.
        //FIXME I don't really like having so many dangly pointers about...
        m_config->reparseConfiguration();
        m_configLoader = new AuthKitConfigLoader(m_config, &kcfgFile, m_wrapperWidget.data());
        m_manager = new KConfigDialogManager(m_wrapperWidget.data(), m_configLoader);
        connect(m_manager, SIGNAL(widgetModified()), SLOT(onSettingsChanged()));

        layout()->addWidget(m_wrapperWidget.data());
    }

    Q_EMIT changed(false);
}

void ConfigOptions::onSettingsChanged()
{
    qDebug() << "changed";
    Q_EMIT changed(true);
}

QVariantMap ConfigOptions::save()
{
    if(m_wrapperWidget.isNull()) {
        return QVariantMap();
    }

    m_manager->updateSettings();
    return m_configLoader->entryMap();
}

void ConfigOptions::defaults()
{
    if(m_wrapperWidget.isNull()) {
        return;
    }

    m_manager->updateWidgetsDefault();
}
