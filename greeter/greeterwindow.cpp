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

#include <QtGui/QWidget>
#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDesktopWidget>
#include <QDir>
#include <QPixmap>
#include <QShortcut>
#include <QtCore/QDebug>
#include <QtDeclarative/qdeclarative.h>

#include <QLightDM/Greeter>
#include <QLightDM/UsersModel>
#include <QLightDM/SessionsModel>
#include <QLightDM/Power>

#include <kdeclarative.h>

#include <KConfig>
#include <KConfigGroup>
#include <KStandardDirs>
#include <KUrl>
#include <KDebug>
#include <Plasma/Theme>


#include "components/lineedit.h"
#include "components/modelcombobox.h"
#include "extrarowproxymodel.h"
#include "faceimageprovider.h"
#include "configwrapper.h"


#include <config.h>

GreeterWindow::GreeterWindow(QWidget *parent)
    : QDeclarativeView(parent),
      m_greeter(new QLightDM::Greeter(this))
{
    QRect screen = QApplication::desktop()->rect();
    setGeometry(screen);
    
    m_greeter->connectSync();
    
    //add a new plasma widget which is a plasma line edit in password mode.
    qmlRegisterType<LineEdit>("MyLibrary", 1, 0, "LineEdit");
    qmlRegisterType<ModelComboBox>("MyLibrary", 1, 0, "ModelComboBox");

    KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.initialize();
    //binds things like kconfig and icons
    kdeclarative.setupBindings();

    ExtraRowProxyModel* usersModel = new ExtraRowProxyModel(this);
    usersModel->setSourceModel(new QLightDM::UsersModel(this));

    if (m_greeter->hasGuestAccountHint()) {
        int guestRowId = usersModel->appendRow();
        usersModel->setRowText(guestRowId, 0, i18n("Guest"));
        usersModel->setRowData(guestRowId, 0, "*guest", QLightDM::UsersModel::NameRole);
    }

    engine()->addImageProvider("face", new FaceImageProvider(usersModel));

//     scriptEngine = kdeclarative.scriptEngine();
//     registerDataEngineMetaTypes(scriptEngine);
   
    //FIXME set the engine to ban ALL network activity.

    KConfig config(LIGHTDM_CONFIG_DIR "/lightdm-kde-greeter.conf");
    KConfigGroup configGroup = config.group("greeter");

    rootContext()->setContextProperty("config", new ConfigWrapper(this));
    rootContext()->setContextProperty("screenSize", size());
    rootContext()->setContextProperty("greeter", m_greeter);
    rootContext()->setContextProperty("usersModel", usersModel);
    rootContext()->setContextProperty("sessionsModel", new QLightDM::SessionsModel(this));
    rootContext()->setContextProperty("power", new QLightDM::PowerInterface(this));
    rootContext()->setContextProperty("plasmaTheme", Plasma::Theme::defaultTheme());


    QString theme = configGroup.readEntry("theme-name", "classic");
    KUrl source = KGlobal::dirs()->locate("appdata", "themes/" + theme + "/main.qml");

    if (source.isEmpty()) {
        kError() << "Cannot find QML file for" << theme << "theme. Falling back to \"classic\" theme.";
        source = KGlobal::dirs()->locate("appdata", "themes/classic/main.qml");
        if (source.isEmpty()) {
            kFatal() << "Cannot find QML file for \"classic\" theme. Something is wrong with this installation. Aborting.";
        }
    }
    kDebug() << "Loading" << source;
    setSource(source);

    // Shortcut to take a screenshot of the screen. Handy because it is not
    // possible to take a screenshot of the greeter in test mode without
    // including the cursor.
    QShortcut* cut = new QShortcut(this);
    cut->setKey(Qt::CTRL + Qt::ALT + Qt::Key_S);
    connect(cut, SIGNAL(activated()), SLOT(screenshot()));
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

