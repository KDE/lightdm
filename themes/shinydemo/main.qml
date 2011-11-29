import QtQuick 1.0
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import MyLibrary 1.0 as LightDMPlasmaWidgets

Item {
    width:800
    height: 600

    Image {
        fillMode: Image.PreserveAspectCrop
        source: "air.jpeg"
        anchors.fill: parent
    }

    Connections {
        target: greeter;
        onConnected: {
            console.log("connected");
        }

        onShowPrompt: {
            greeter.provideSecret(passwordInput.text);
        }

        onAuthenticationComplete: {
            if(isAuthenticated) {
                greeter.startSession(sessionCombo.itemData(sessionCombo.currentIndex), "en-GB");
            }
            else {
                feedbackLabel.text = "Sorry, incorrect password please try again,";
            }
        }
    }

    function login() {
        greeter.login(usernameInput.text);
    }

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
                text: "Welcome to "+ greeter.hostname;
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
                LightDMPlasmaWidgets.PasswordLineEdit {
                    id: passwordInput;
                    width: 250;
                    height: 30;
                    clickMessage: i18n("Password");
                    onReturnPressed: {
                        login();
                    }

                }
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
                enabled: greeter.canShutdown;
                onClicked: {greeter.shutDown();}
            }

            PlasmaWidgets.IconWidget {
                text: i18n("Suspend")
                icon: QIcon("system-suspend")
                enabled: greeter.canSuspend;
                onClicked: {greeter.suspend();}
            }

            PlasmaWidgets.IconWidget {
                text: i18n("Restart")
                icon: QIcon("system-reboot")
                enabled: greeter.canRestart;
                onClicked: {greeter.restart();}
            }

            PlasmaWidgets.IconWidget {
                text: i18n("Hibernate")
                icon: QIcon("system-suspend-hibernate")
                enabled: greeter.canHibernate;
                onClicked: {greeter.hibernate();}
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
                text: "Session"
                verticalAlignment: Qt.AlignHCenter;
            }

            LightDMPlasmaWidgets.ModelComboBox {
                id: sessionCombo
                model: sessionsModel;
            }


            Text {
                text: "Keyboard:"
                verticalAlignment: Qt.AlignHCenter;
            }

            PlasmaWidgets.ComboBox {

            }

            Text {
                text: "Language:"
                verticalAlignment: Qt.AlignHCenter;
            }

            PlasmaWidgets.ComboBox {

            }

        }
    }
}
