#include "greeterpreview.h"

#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QTimer>


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

void GreeterPreview::setTheme(const Plasma::Package &theme)
{
    m_theme = theme;
    KGlobal::locale()->insertCatalog("lightdm_theme_" + theme.metadata().name());
}

void GreeterPreview::setConfig(const KSharedConfigPtr &config)
{
    m_config = config;
}

void GreeterPreview::reload()
{
    Q_ASSERT(m_config);

    QString source = m_theme.filePath("mainscript");
    kDebug() << "Loading" << source;

    rootContext()->setContextProperty("config", new ConfigWrapper(m_theme.filePath("configfile"), m_config, this));

    qDebug() << "trying to load " << m_theme.metadata().name() << " found " << source;
    setSource(source);
}
