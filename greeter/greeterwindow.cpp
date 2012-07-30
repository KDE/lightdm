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
#include "greeterwindow.h"
#include "powermanagement.h"

#include <QtGui/QWidget>
#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDesktopWidget>
#include <QDir>
#include <QPixmap>
#include <QShortcut>
#include <QProcess>
#include <QtCore/QDebug>
#include <QtDeclarative/qdeclarative.h>
#include <QPainter>
#include <QPixmap>
#include <QProcess>

#include <QLightDM/Greeter>
#include <QLightDM/UsersModel>

#include <kdeclarative.h>

#include <KConfig>
#include <KConfigGroup>
#include <KStandardDirs>
#include <KUrl>
#include <KDebug>
#include <KGlobal>
#include <Plasma/Theme>

#include "faceimageprovider.h"
#include "configwrapper.h"
#include "sessionsmodel.h"
#include "usersmodel.h"
#include "greeterwrapper.h"

#include <config.h>

GreeterWindow::GreeterWindow(QWidget *parent)
    : QDeclarativeView(parent),
      m_greeter(new GreeterWrapper(this))
{
    QRect screen = QApplication::desktop()->rect();
    setGeometry(screen);
    
    KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.initialize();
    //binds things like kconfig and icons
    kdeclarative.setupBindings();

    UsersModel* usersModel = new UsersModel(this);

    if (m_greeter->hasGuestAccountHint()) {
        usersModel->setShowGuest(true);
    }

    engine()->addImageProvider("face", new FaceImageProvider(usersModel));

    KConfig config(LIGHTDM_CONFIG_DIR "/lightdm-kde-greeter.conf");
    KConfigGroup configGroup = config.group("greeter");

    QString theme = configGroup.readEntry("theme-name", "userbar");
    KUrl source = KGlobal::dirs()->locate("appdata", "themes/" + theme + "/main.qml");

    if (source.isEmpty()) {
        kError() << "Cannot find QML file for" << theme << "theme. Falling back to \"userbar\" theme.";
        theme = "userbar";
        source = KGlobal::dirs()->locate("appdata", "themes/userbar/main.qml");
        if (source.isEmpty()) {
            kFatal() << "Cannot find QML file for \"userbar\" theme. Something is wrong with this installation. Aborting.";
        }
    }
    kDebug() << "Loading" << source;

    KGlobal::locale()->insertCatalog("lightdm_theme_" + theme);
    
    rootContext()->setContextProperty("config", new ConfigWrapper(KGlobal::dirs()->locate("appdata", "themes/" + theme + "/main.xml"), this));
    rootContext()->setContextProperty("screenSize", size());
    rootContext()->setContextProperty("greeter", m_greeter);
    rootContext()->setContextProperty("usersModel", usersModel);
    rootContext()->setContextProperty("sessionsModel", new SessionsModel(this));
    rootContext()->setContextProperty("plasmaTheme", Plasma::Theme::defaultTheme());

    setSource(source);
    // Prevent screen flickering when the greeter starts up. This really needs to be sorted out in QML/Qt...
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Shortcut to take a screenshot of the screen. Handy because it is not
    // possible to take a screenshot of the greeter in test mode without
    // including the cursor.
    QShortcut* cut = new QShortcut(this);
    cut->setKey(Qt::CTRL + Qt::ALT + Qt::Key_S);
    connect(cut, SIGNAL(activated()), SLOT(screenshot()));

    connect(m_greeter, SIGNAL(aboutToLogin()), SLOT(setRootImage()));

    new PowerManagement(this);
}

GreeterWindow::~GreeterWindow()
{
}

void GreeterWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    rootContext()->setContextProperty("screenSize", size());
    setSceneRect(QRectF(0, 0, width(), height()));
}

void GreeterWindow::setRootImage()
{
    QPixmap pix = QPixmap::grabWindow(winId());
    QProcess process;
    process.start(QFile::encodeName(KGlobal::dirs()->findExe("lightdm-kde-greeter-rootimage")).data(), QIODevice::WriteOnly);
    pix.save(&process, "xpm"); //write pixmap to rootimage
    process.closeWriteChannel();
    process.waitForFinished();
}

void GreeterWindow::screenshot()
{
    QPixmap pix = QPixmap::grabWindow(winId());

    QString path = QDir::temp().absoluteFilePath("lightdm-kde-greeter-screenshot.png");
    bool ok = pix.save(path);
    if (ok) {
        kDebug() << "Saved screenshot as" << path;
    } else {
        kWarning() << "Failed to save screenshot as" << path;
    }
}

#include "moc_greeterwindow.cpp"

