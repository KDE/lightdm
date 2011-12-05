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
            console.log("onShowPrompt " + screen.password);
            greeter.respond(screen.password);
        }

        onAuthenticationComplete: {
            if(greeter.authenticated) {
                greeter.startSessionSync(screen.session);
            } else {
                feedbackLabel.text = i18n("Sorry, incorrect password please try again.");
            }
        }
    }

    function login(username, _password, _session) {
        screen.password = _password;
        screen.session = _session;
        console.log(username + " " + screen.password + " " + screen.session);
        greeter.authenticate(username);
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.top: parent.top
        id: feedbackLabel;
        font.pointSize: 9
        text: i18n("Welcome to %1", greeter.hostname);
    }

    ListView {
        id: usersList
        anchors.centerIn: parent
        height: parent.height - feedbackLabel.height * 2
        width: parent.width * 0.8
        focus: true

        model: usersModel

        cacheBuffer: count * 80

        delegate: Item {
            id: wrapper

            property bool isCurrent: ListView.isCurrentItem

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
                opacity: isCurrent ? 1 : 0
                Behavior on opacity {
                    NumberAnimation { duration: 100 }
                }
            }

            property int outerPadding: isCurrent ? 18 : 6
            property int padding: 6
            Behavior on outerPadding {
                NumberAnimation { duration: 100 }
            }

            Rectangle {
                id: face
                anchors.right: wrapper.left
                anchors.rightMargin: -60
                y: parent.outerPadding
                width: parent.height - 2 * parent.outerPadding
                height: width
                color: "gray"
                Behavior on x {
                    NumberAnimation { duration: 100 }
                }
            }

            Text {
                id: loginText
                anchors.top: face.top
                anchors.left: face.right
                anchors.right: loginButton.left
                anchors.rightMargin: wrapper.padding
                anchors.leftMargin: wrapper.padding
                font.pointSize: isCurrent ? 14 : 12
                text: realName
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
                opacity: isCurrent ? 1 : 0
                Behavior on opacity {
                    NumberAnimation { duration: 100 }
                }
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
                opacity: isCurrent ? 1 : 0
                Behavior on opacity {
                    NumberAnimation { duration: 100 }
                }
            }

            LightDMPlasmaWidgets.ModelComboBox {
                id: sessionCombo
                model: sessionsModel
                anchors.right: loginButton.right
                anchors.top: loginButton.bottom
                anchors.topMargin: padding
                width: 200;
                opacity: isCurrent ? 1 : 0
                Behavior on opacity {
                    NumberAnimation { duration: 100 }
                }
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

    /*
    PlasmaCore.FrameSvgItem {
        id: dialog;
        imagePath: "translucent/dialogs/background"
        anchors.centerIn: parent;

        width: childrenRect.width + 80;
        height: childrenRect.height + 40;

        Column {
            spacing: 5
            anchors.centerIn: parent

            Image {
                id: logo
                source: "images/kubuntu_logo.png";
                anchors.centerIn: parent;
            }


            Text {
                anchors.horizontalCenter: parent.horizontalCenter;
                id: feedbackLabel;
                font.pointSize: 9
                text: i18n("Welcome to %1", greeter.hostname);
            }

            Row {
                Image {
                    source: "image://icon/meeting-participant"
                    height: usernameInput.height;
                    fillMode: Image.PreserveAspectFit

                }

                PlasmaWidgets.LineEdit {
                    id: usernameInput;
                    width: 250;
                    height: 30;
                    clickMessage: "Username";
                    clearButtonShown: true;
                }
            }


            Row {
                Image {
                    source: "image://icon/object-locked.png"
                    height: passwordInput.height;
                    fillMode: Image.PreserveAspectFit

                }

//                PlasmaWidgets.LineEdit {
            }

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

        Row {
            spacing: 5
            anchors.centerIn: parent

            PlasmaWidgets.IconWidget {
                text: i18n("Shutdown")
                icon: QIcon("system-shutdown")
                enabled: power.canShutdown;
                onClicked: {power.shutDown();}
            }

            PlasmaWidgets.IconWidget {
                text: i18n("Suspend")
                icon: QIcon("system-suspend")
                enabled: power.canSuspend;
                onClicked: {power.suspend();}
            }

            PlasmaWidgets.IconWidget {
                text: i18n("Restart")
                icon: QIcon("system-reboot")
                enabled: power.canRestart;
                onClicked: {power.restart();}
            }

            PlasmaWidgets.IconWidget {
                text: i18n("Hibernate")
                icon: QIcon("system-suspend-hibernate")
                enabled: power.canHibernate;
                onClicked: {power.hibernate();}
            }
        }

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
