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
        source: "air.jpeg"
        anchors.fill: parent
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
        anchors.left: parent.left;
        anchors.top: parent.top
        font.pointSize: 9
        text: i18n("Welcome to %1", greeter.hostname);
    }

    FeedbackLabel {
        id: feedbackLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        font.pointSize: 14
    }

    Component {
        id: userDelegate

        Item {
            id: wrapper

            property bool isCurrent: ListView.isCurrentItem

            // Opacity for items which are only visible when we are on the current index
            property real currentOpacity: isCurrent ? 1 : 0
            Behavior on currentOpacity {
                NumberAnimation { duration: 100 }
            }

            onIsCurrentChanged: {
                if (isCurrent) {
                    passwordInput.setFocus();
                }
            }

            height: isCurrent ? 80 : 40
            width: parent.width
            Behavior on height {
                NumberAnimation { duration: 100 }
            }

            PlasmaCore.FrameSvgItem {
                anchors.fill: parent
                imagePath: "translucent/dialogs/background"
                opacity: currentOpacity
            }

            property int outerPadding: isCurrent ? 18 : 6
            property int padding: 6
            Behavior on outerPadding {
                NumberAnimation { duration: 100 }
            }

            Face {
                id: face
                anchors.right: wrapper.left
                anchors.rightMargin: -60
                y: parent.outerPadding
                width: parent.height - 2 * parent.outerPadding
                source: "image://face/" + name
            }

            Text {
                id: loginText
                anchors.top: face.top
                anchors.left: face.right
                anchors.right: loginButton.left
                anchors.rightMargin: wrapper.padding
                anchors.leftMargin: wrapper.padding
                font.pointSize: isCurrent ? 14 : 12
                text: display
                Behavior on font.pointSize {
                    NumberAnimation { duration: 100 }
                }
            }

            LightDMPlasmaWidgets.PasswordLineEdit {
                id: passwordInput
                anchors.top: loginText.bottom
                anchors.left: face.right
                anchors.right: loginButton.left
                anchors.margins: wrapper.padding
                clickMessage: i18n("Password")
                onReturnPressed: startLogin();
                opacity: currentOpacity
            }

            PlasmaWidgets.PushButton {
                id: loginButton
                anchors.top: face.top
                anchors.right: parent.right
                anchors.bottom: face.bottom
                width: height
                anchors.rightMargin: wrapper.outerPadding
                text: ">"
                onClicked: startLogin();
                opacity: currentOpacity
            }

            LightDMPlasmaWidgets.ModelComboBox {
                id: sessionCombo
                model: sessionsModel
                anchors.right: loginButton.right
                anchors.top: loginButton.bottom
                anchors.topMargin: padding
                width: 200;
                opacity: currentOpacity
            }

            function startLogin() {
                var session = sessionCombo.itemData(sessionCombo.currentIndex);
                if (session == "") {
                    session = "default";
                }
                login(name, passwordInput.text, session);
            }
        }
    }

    ListView {
        id: usersList
        anchors.centerIn: parent
        height: parent.height * 0.66
        width: parent.width * 0.66
        focus: true

        model: usersModel

        cacheBuffer: count * 80

        delegate: userDelegate
    }

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
                text: i18n("Shutdown")
                icon: QIcon("system-shutdown")
                enabled: power.canShutdown
                onClicked: power.shutDown();
            }

            PowerButton {
                text: i18n("Suspend")
                icon: QIcon("system-suspend")
                enabled: power.canSuspend;
                onClicked: power.suspend();
            }

            PowerButton {
                text: i18n("Restart")
                icon: QIcon("system-reboot")
                enabled: power.canRestart
                onClicked: power.restart();
            }

            PowerButton {
                text: i18n("Hibernate")
                icon: QIcon("system-suspend-hibernate")
                enabled: power.canHibernate
                onClicked: power.hibernate();
            }
        }
    }

    /*
            Row {
                anchors.horizontalCenter: parent.horizontalCenter;
                spacing:10;

                PlasmaWidgets.PushButton {
                    text: i18n("Login");
                    onClicked: {
                        login();
                    }
                }

                PlasmaWidgets.PushButton {
                    text: i18n("Power");
                    onClicked: {
                        if (powerDialog.opacity == 1) {
                            powerDialog.opacity = 0;
                        } else {
                            powerDialog.opacity = 1;
                        }

                        optionsDialog.opacity = 0;
                    }
                }

                PlasmaWidgets.PushButton {
                    text: i18n("Options");
                    onClicked: {
                        if (optionsDialog.opacity == 1) {
                            optionsDialog.opacity = 0;
                        } else {
                            optionsDialog.opacity = 1;
                        }

                        powerDialog.opacity = 0;
                    }
                }

            }
        }
    }

    PlasmaCore.FrameSvgItem {
        id: powerDialog;
        anchors.top: dialog.bottom
        anchors.topMargin: 3
        anchors.horizontalCenter: parent.horizontalCenter
        imagePath: "translucent/dialogs/background"
        opacity: 0

        Behavior on opacity { PropertyAnimation { duration: 500} }

        width: childrenRect.width + 30;
        height: childrenRect.height + 30;


    }


    PlasmaCore.FrameSvgItem {
        id: optionsDialog;
        anchors.top: dialog.bottom
        anchors.topMargin: 3
        anchors.horizontalCenter: parent.horizontalCenter
        imagePath: "translucent/dialogs/background"
        opacity: 0

        Behavior on opacity { PropertyAnimation { duration: 500} }

        width: childrenRect.width + 30;
        height: childrenRect.height + 30;

        Row {
            spacing: 5
            anchors.centerIn: parent

            Text {
                text: i18n("Session")
                anchors.verticalCenter: parent.verticalCenter;
            }

            LightDMPlasmaWidgets.ModelComboBox {
                id: sessionCombo
                model: sessionsModel;
                anchors.verticalCenter: parent.verticalCenter;
                width: 200;
            }


            Text {
                text: i18n("Language:")
                anchors.verticalCenter: parent.verticalCenter;
            }

            PlasmaWidgets.ComboBox {
                anchors.verticalCenter: parent.verticalCenter;
            }

        }
    }
    */
}
