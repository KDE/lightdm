import QtQuick 1.0
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import MyLibrary 1.0 as LightDMPlasmaWidgets

Item {
    id: frontend
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
            console.log("onShowPrompt " + frontend.password);
            greeter.respond(frontend.password);
        }

        onAuthenticationComplete: {
            if(greeter.authenticated) {
                greeter.startSessionSync(frontend.session);
            } else {
                feedbackLabel.text = i18n("Sorry, incorrect password please try again.");
            }
        }
    }

    function login(username, _password, _session) {
        frontend.password = _password;
        frontend.session = _session;
        console.log(username + " " + frontend.password + " " + frontend.session);
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
        delegate: Item {
            id: wrapper

            height: ListView.isCurrentItem ? currentItemRect.height : itemRect.height
            width: parent.width

            PlasmaCore.FrameSvgItem {
                id: currentItemRect
                anchors.left: parent.left
                anchors.right: parent.right
                height: 80
                imagePath: "translucent/dialogs/background"
                opacity: wrapper.ListView.isCurrentItem ? 1 : 0

                Item {
                    // Create extra margins around the whole content
                    anchors.fill: parent
                    anchors.margins: 12

                    property int padding: 6

                    Rectangle {
                        id: currentFace
                        x: parent.padding
                        y: parent.padding
                        width: parent.height - 2 * parent.padding
                        height: width
                        color: "gray"
                    }

                    Text {
                        id: loginText
                        anchors.top: parent.top
                        anchors.left: currentFace.right
                        anchors.right: loginButton.left
                        anchors.margins: parent.padding
                        text: realName
                    }

                    LightDMPlasmaWidgets.PasswordLineEdit {
                        id: passwordInput
                        anchors.top: loginText.bottom
                        anchors.left: currentFace.right
                        anchors.right: loginButton.left
                        anchors.margins: parent.padding
                        clickMessage: i18n("Password")
                        onReturnPressed: startLogin();
                    }

                    PlasmaWidgets.PushButton {
                        id: loginButton
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        width: height
                        anchors.margins: parent.padding
                        text: ">"
                        onClicked: startLogin();
                    }

                    LightDMPlasmaWidgets.ModelComboBox {
                        id: sessionCombo
                        model: sessionsModel
                        anchors.right: parent.right
                        anchors.top: parent.bottom
                        width: 200;
                    }

                }
            } // /currentItemRect

            function startLogin() {
                // Note: I would like to put this function inside the current
                // item rect, but for some reason if I do this I get
                // "startLogin" unknown variable when code tries to call it.
                var session = sessionCombo.itemData(sessionCombo.currentIndex);
                if (session == "") {
                    session = "default";
                }
                login(name, passwordInput.text, session);
            }

            Rectangle {
                id: itemRect
                anchors.left: parent.left
                anchors.right: parent.right
                height: 30
                opacity: wrapper.ListView.isCurrentItem ? 0 : 1
                color: "transparent"
                property int padding: 5
                Rectangle {
                    id: face
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: height
                    anchors.margins: parent.padding
                    color: "gray"
                }

                Text {
                    anchors.left: face.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.margins: parent.padding
                    text: realName
                }
            } // /itemRect
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
