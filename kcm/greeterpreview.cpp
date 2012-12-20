#include "greeterpreview.h"

#include <QDeclarativeEngine>
#include <QDeclarativeContext>

#include <KUrl>
#include <KGlobal>
#include <KDebug>
#include <KStandardDirs>
#include <KLocale>
#include <KComponentData>
#include <kdeclarative.h>

#include "configwrapper.h"

GreeterPreview::GreeterPreview(QWidget *parent) :
    QDeclarativeView(parent)
{
    setBackgroundRole(QPalette::Background);

    KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.initialize();
    //binds things like kconfig and icons
    kdeclarative.setupBindings();

    resize(400, 225);
    scale(0.25, 0.25);
}

void GreeterPreview::setTheme(const QString &theme)
{
    m_theme = theme;
    KGlobal::locale()->insertCatalog("lightdm_theme_" + theme);
    reload();
}

void GreeterPreview::setConfig(const KSharedConfigPtr &config)
{
    m_config = config;
}

void GreeterPreview::reload()
{
    Q_ASSERT(m_config);

    KComponentData componentData("lightdm-kde-greeter");
    KUrl source = KGlobal::dirs()->locate("appdata", "themes/" + m_theme + "/main.qml", componentData);
    kDebug() << "Loading" << source;

    rootContext()->setContextProperty("config", new ConfigWrapper(KGlobal::dirs()->locate("appdata", "themes/" + m_theme + "/main.xml", componentData), m_config, this));

    qDebug() << "trying to load " << m_theme << " found " << source;
    setSource(source);
}
