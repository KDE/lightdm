import QtQuick 1.0
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.core 0.1 as PlasmaCore
import MyLibrary 1.0 as LightDMPlasmaWidgets

Item {
    width: screenSize.width;
    height: screenSize.height;

    Image {
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
        source:config.readEntry("Background", "air.jpeg");
    }

    Connections {
        target: greeter;
        onConnected: {
            console.log("connected");
        }

        onShowPrompt: {
            greeter.respond(passwordInput.text);
        }

        onAuthenticationComplete: {
            if(greeter.authenticated) {
                greeter.startSessionSync(sessionCombo.itemData(sessionCombo.currentIndex));
            }
            else {
                feedbackLabel.text = i18n("Sorry, incorrect password please try again.");
            }
        }
    }

    function login() {
        greeter.authenticate(usernameInput.text);
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
}
