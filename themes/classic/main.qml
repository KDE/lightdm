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
import QtQuick 1.0
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore
import MyLibrary 1.0 as LightDMPlasmaWidgets

Item {
    width: screenSize.width;
    height: screenSize.height;

    Image {
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
        //read from config, if there's no entry use plasma theme
        source: config.readEntry("Background", "") ? config.readEntry("Background", ""): plasmaTheme.wallpaperPath();
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
                source: config.readEntry("Logo","")
                fillMode: Image.PreserveAspectFit
                height: 100
                anchors.horizontalCenter: parent.horizontalCenter
                smooth: true
            }


            Text {
                anchors.horizontalCenter: parent.horizontalCenter;
                id: feedbackLabel;
                font.pointSize: 9
                text: config.readEntry("GreetMessage",i18n("Welcome to %1", greeter.hostname));
            }

            Row {
                Image {
                    source: "image://icon/meeting-participant"
                    height: usernameInput.height;
                    fillMode: Image.PreserveAspectFit

                }

                //not actually a password!
                LightDMPlasmaWidgets.LineEdit {
                    id: usernameInput;
                    width: 250;
                    height: 30;
                    clickMessage: "Username";
                    clearButtonShown: true;
                    onReturnPressed: {
                        passwordInput.setFocus();
                    }
                    
                     Component.onCompleted: {
                        usernameInput.setFocus();
                    }
                }
            }


            Row {
                Image {
                    source: "image://icon/object-locked.png"
                    height: passwordInput.height;
                    fillMode: Image.PreserveAspectFit

                }

                LightDMPlasmaWidgets.LineEdit {
                    id: passwordInput;
                    width: 250;
                    height: 30;
                    clickMessage: i18n("Password");
                    passwordMode: true;
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
                text: i18n("Suspend")
                icon: QIcon("system-suspend")
                enabled: power.canSuspend;
                onClicked: {power.suspend();}
            }
            
            PlasmaWidgets.IconWidget {
                text: i18n("Hibernate")
                icon: QIcon("system-suspend-hibernate")
                enabled: power.canHibernate;
                onClicked: {power.hibernate();}
            }

            PlasmaWidgets.IconWidget {
                text: i18n("Restart")
                icon: QIcon("system-reboot")
                enabled: power.canRestart;
                onClicked: {power.restart();}
            }
            
            PlasmaWidgets.IconWidget {
                text: i18n("Shutdown")
                icon: QIcon("system-shutdown")
                enabled: power.canShutdown;
                onClicked: {power.shutDown();}
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
