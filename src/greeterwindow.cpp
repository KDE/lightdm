#include "greeterwindow.h"

#include <QtGui/QWidget>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QApplication>
#include <QDesktopWidget>
#include <QtCore/QDebug>

#include <QLightDM/Greeter>
#include <QLightDM/UsersModel>
#include <QLightDM/SessionsModel>

//FIXME use the KDeclarative from KDE (requires >= 4.8)
#include "libkdeclarative/kdeclarative.h"

#include <KUrl>

GreeterWindow::GreeterWindow(QWidget *parent)
    : QDeclarativeView(parent),
      m_greeter(new QLightDM::Greeter(this))
{
    QRect screen = QApplication::desktop()->rect();
    setGeometry(screen);
    
    m_greeter->connectSync();


    KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.initialize();
    //binds things like kconfig and icons
    kdeclarative.setupBindings();

//     scriptEngine = kdeclarative.scriptEngine();
//     registerDataEngineMetaTypes(scriptEngine);
   
    //FIXME set the engine to ban ALL network activity.


    //FIXME load from /etc/lightdm/lightdm-kde.conf
    QString theme = "classic";

    //FIXME use KStandardDirs and such.
    KUrl source = "/usr/local/share/apps/lightdm-kde/themes/" + theme + "/main.qml";
    this->setSource(source);

    connect(m_greeter, SIGNAL(quit()), SLOT(close()));
    connect(m_greeter, SIGNAL(connected()), SLOT(onGreeterConnected()));

    rootContext()->setContextProperty("screenSize", size());
    rootContext()->setContextProperty("greeter", m_greeter);
    rootContext()->setContextProperty("usersModel", new QLightDM::UsersModel(this));
    rootContext()->setContextProperty("sessionsModel", new QLightDM::SessionsModel(this));
}

GreeterWindow::~GreeterWindow()
{
}

void GreeterWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    rootContext()->setContextProperty("screenSize", size());
}


void GreeterWindow::onGreeterConnected()
{
    qDebug() << "connected!";

//    KUrl themeUrl(m_greeter->theme());
//    QSettings themeInfo(themeUrl.path(), QSettings::IniFormat);

//    KUrl themeDirectory(themeUrl.directory(KUrl::AppendTrailingSlash));

//    KUrl qmlUrl(themeDirectory, themeInfo.value("theme/qmlfile").toString());

//    qDebug() << "********";
//    qDebug() << m_greeter->property("theme/background").toString();
    
//    if (qmlUrl.isEmpty()) {
//        qDebug() << "No qmlfile supplied in";
//        close();
//    }
//    else {
//    }
}

#include "moc_greeterwindow.cpp"

