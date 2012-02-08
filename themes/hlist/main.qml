import QtQuick 1.0
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import MyLibrary 1.0 as LightDMPlasmaWidgets

Item {
    id: screen
    width: screenSize.width;
    height: screenSize.height;

    property string password;
    property string session;

    Image {
        fillMode: Image.PreserveAspectCrop
        source: plasmaTheme.wallpaperPath()
        anchors.fill: parent
        visible: source != ""
    }

    Connections {
        target: greeter;

        onShowPrompt: {
            greeter.respond(screen.password);
        }

        onAuthenticationComplete: {
            if(greeter.authenticated) {
                greeter.startSessionSync(screen.session);
            } else {
                feedbackLabel.text = i18n("Sorry, incorrect password. Please try again.");
                feedbackLabel.showFeedback();
            }
        }
    }

    function login(username, _password, _session) {
        screen.password = _password;
        screen.session = _session;
        greeter.authenticate(username);
    }

    Text {
        id: welcomeLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        font.pointSize: 14
        text: i18n("Welcome to %1", greeter.hostname);
    }

    FeedbackLabel {
        id: feedbackLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: welcomeLabel.bottom
        font.pointSize: 14
    }

    property int userItemWidth: 150
    property int userItemHeight: 150

    property int padding: 20

    Component {
        id: userDelegate

        Item {
            id: wrapper

            property bool isCurrent: ListView.isCurrentItem

            // Opacity for items which are only visible when we are on the current index
            property real currentOpacity: 0

            width: userItemWidth
            height: userItemHeight

            opacity: isCurrent ? 1.0 : 0.618

            Behavior on opacity {
                NumberAnimation {
                    duration: 250
                }
            }

            PlasmaCore.FrameSvgItem {
                anchors.centerIn: face
                width: face.width + padding * 2
                height: face.height + padding * 2
                //imagePath: "translucent/dialogs/background"
                imagePath: "opaque/dialogs/background"
                opacity: 0.618
            }

            Face {
                id: face
                anchors.bottom: loginText.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: padding * 1.5
                width: 48
                height: 48
                source: "image://face/" + name
            }
            Text {
                id: loginText
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: display
            }

            MouseArea {
                anchors.fill: parent
                enabled: !isCurrent
                onClicked: wrapper.ListView.view.currentIndex = index;
            }
        }
    }

    function startLogin() {
        var session = sessionCombo.itemData(sessionCombo.currentIndex);
        if (session == "") {
            session = "default";
        }
        login(name, passwordInput.text, session);
    }

    ListView {
        id: usersList
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.height / 4
        focus: true
        height: userItemHeight

        model: usersModel

        cacheBuffer: count * 80

        delegate: userDelegate

        orientation: ListView.Horizontal

        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: width / 2 - userItemWidth / 2
        preferredHighlightEnd: width / 2 + userItemWidth / 2
    }

    // Fixed widgets
    property int widgetHeight: 30

    Column {
        anchors.top: usersList.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: 200
        anchors.topMargin: 2 * padding
        spacing: padding

        LightDMPlasmaWidgets.PasswordLineEdit {
            id: passwordInput
            width: parent.width
            height: widgetHeight
            clickMessage: i18n("Password")
            onReturnPressed: startLogin();
        }

        LightDMPlasmaWidgets.ModelComboBox {
            id: sessionCombo
            width: parent.width
            height: widgetHeight
            model: sessionsModel
            currentIndex: indexForData(session, sessionsModel.key)
        }

        PlasmaWidgets.PushButton {
            id: loginButton
            anchors.horizontalCenter: parent.horizontalCenter
            height: widgetHeight
            text: i18n("Login")
            onClicked: startLogin();
        }
    }

    // Bottom "Settings" bar
    PlasmaCore.FrameSvgItem {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: childrenRect.width + margins.left
        height: childrenRect.height + margins.top * 2
        imagePath: "translucent/widgets/panel-background"
        prefix: "south-mini"

        // Hack to hide left and bottom corners. Tried to use enabledBorders but failed.
        anchors.rightMargin: -margins.right
        anchors.bottomMargin: -margins.bottom

        Row {
            spacing: 5
            x: parent.margins.left
            y: parent.margins.top

            PowerButton {
                text: i18n("Suspend")
                icon: QIcon("system-suspend")
                enabled: power.canSuspend;
                onClicked: power.suspend();
            }

            PowerButton {
                text: i18n("Hibernate")
                icon: QIcon("system-suspend-hibernate")
                enabled: power.canHibernate
                onClicked: power.hibernate();
            }

            PowerButton {
                text: i18n("Restart")
                icon: QIcon("system-reboot")
                enabled: power.canRestart
                onClicked: power.restart();
            }

            PowerButton {
                text: i18n("Shutdown")
                icon: QIcon("system-shutdown")
                enabled: power.canShutdown
                onClicked: power.shutDown();
            }
        }
    }
}
