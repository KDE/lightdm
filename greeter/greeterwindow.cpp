#include "greeterwindow.h"

#include <QtGui/QWidget>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QApplication>
#include <QDesktopWidget>
#include <QtCore/QDebug>
#include <QtDeclarative/qdeclarative.h>

#include <QLightDM/Greeter>
#include <QLightDM/UsersModel>
#include <QLightDM/SessionsModel>
#include <QLightDM/Power>


//FIXME use the KDeclarative from KDE (requires >= 4.8)
#include "libkdeclarative/kdeclarative.h"

#include <KStandardDirs>
#include <KUrl>
#include <KDebug>


#include "components/passwordlineedit.h"
#include "components/modelcombobox.h"

GreeterWindow::GreeterWindow(QWidget *parent)
    : QDeclarativeView(parent),
      m_greeter(new QLightDM::Greeter(this))
{
    QRect screen = QApplication::desktop()->rect();
    setGeometry(screen);
    
    m_greeter->connectSync();
    
    //add a new plasma widget which is a plasma line edit in password mode.
    qmlRegisterType<PasswordLineEdit>("MyLibrary", 1, 0, "PasswordLineEdit");
    qmlRegisterType<ModelComboBox>("MyLibrary", 1, 0, "ModelComboBox");

    KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.initialize();
    //binds things like kconfig and icons
    kdeclarative.setupBindings();

//     scriptEngine = kdeclarative.scriptEngine();
//     registerDataEngineMetaTypes(scriptEngine);
   
    //FIXME set the engine to ban ALL network activity.

    rootContext()->setContextProperty("screenSize", size());
    rootContext()->setContextProperty("greeter", m_greeter);
    rootContext()->setContextProperty("usersModel", new QLightDM::UsersModel(this));
    rootContext()->setContextProperty("sessionsModel", new QLightDM::SessionsModel(this));
    rootContext()->setContextProperty("power", new QLightDM::PowerInterface(this));
    
    //FIXME load from /etc/lightdm/lightdm-kde.conf
    QString theme = "shinydemo";

    KUrl source = KGlobal::dirs()->locate("appdata", "themes/" + theme + "/main.qml");
    
    kDebug() << "loading " << source;
    
    if (source.isEmpty()) {
        //FIXME we should probably try falling back to a known theme before crashing out.
        qFatal("Cannot find QML File");
    }
    else {
        this->setSource(source);
    }
}

GreeterWindow::~GreeterWindow()
{
}

void GreeterWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    rootContext()->setContextProperty("screenSize", size());
}



#include "moc_greeterwindow.cpp"

