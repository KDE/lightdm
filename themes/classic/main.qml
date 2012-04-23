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
//TODO phase this out
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.qtextracomponents 0.1 as ExtraComponents
import org.kde.plasma.core 0.1 as PlasmaCore

import MyLibrary 1.0 as LightDMPlasmaWidgets

Item {
    width: screenSize.width;
    height: screenSize.height;

    Image {
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent
        //read from config, if there's no entry use plasma theme
        source: config.readEntry("Background") ? config.readEntry("Background"): plasmaTheme.wallpaperPath();
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
                passwordInput.selectAll()
                passwordInput.forceActiveFocus()
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

        width: childrenRect.width + 40;
        height: childrenRect.height + 40;

        Column {
            spacing: 5
            anchors.centerIn: parent

            Image {
                id: logo
                source: config.readEntry("Logo")
                fillMode: Image.PreserveAspectFit
                height: 100
                anchors.horizontalCenter: parent.horizontalCenter
                smooth: true
            }


            PlasmaComponents.Label {
                anchors.horizontalCenter: parent.horizontalCenter;
                id: feedbackLabel;
                font.pointSize: 9
                text: config.readEntry("GreetMessage").replace("%hostname%", greeter.hostname);
            }

            Row {
                spacing: 10
                width: childrenRect.width
                height: childRect.height
                
                Grid {
                    columns: 2
                    spacing: 5
                    
                    ExtraComponents.QIconItem {
                        icon: "meeting-participant"
                        height: passwordInput.height;
                        width: passwordInput.height;
                    }

                    /*PlasmaComponents.*/TextField {
                        id: usernameInput;
                        placeholderText: i18n("Username");
                        onAccepted: {
                            passwordInput.focus = true;
                        }
                        
                        KeyNavigation.tab: passwordInput
                    }

                    ExtraComponents.QIconItem {
                        icon: "object-locked"
                        height: passwordInput.height;
                        width: passwordInput.height;
                    }

                    /*PlasmaComponents.*/TextField {
                        id: passwordInput
                        echoMode: TextInput.Password
                        placeholderText: i18n("Password")
                        onAccepted: {
                            login();
                        }
                        KeyNavigation.backtab: usernameInput
                        KeyNavigation.tab: loginButton
                    }
                }
                
                /*PlasmaComponents.*/ToolButton {
                    id: loginButton
                    anchors.verticalCenter: parent.verticalCenter
                    iconSource: "go-next"
                    onClicked: {
                        login();
                    }
                    KeyNavigation.backtab: passwordInput
                    KeyNavigation.tab: usernameInput
                }
            }

            Item {
                height: 10
            }
            
            
            Row {               
                spacing: 5;
                  IconButton {
                    icon: "system-shutdown"                    
                    onClicked: {
                        if (powerDialog.opacity == 1) {
                            powerDialog.opacity = 0;
                        } else {
                            powerDialog.opacity = 1;
                        }
                        optionsDialog.opacity = 0;
                    }
                }

                
                IconButton {
                    icon: "system-log-out"
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

            PlasmaComponents.Label {
                text: i18n("Session")
                anchors.verticalCenter: parent.verticalCenter;
            }

            LightDMPlasmaWidgets.ModelComboBox {
                id: sessionCombo
                model: sessionsModel;
                anchors.verticalCenter: parent.verticalCenter;
                width: 200;
                Component.onCompleted : {
                    sessionCombo.currentIndex = sessionCombo.indexForData("", sessionsModel.key);
                }
            }
        }
    }
}
