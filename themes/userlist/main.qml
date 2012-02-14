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
            property real currentOpacity: 0

            height: 40
            width: parent.width

            // Padding between the current frame and the other items
            property int outerPadding: 6
            property int padding: 6

            PlasmaCore.FrameSvgItem {
                anchors.fill: parent
                imagePath: "translucent/dialogs/background"
                opacity: currentOpacity
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
                font.pointSize: 12
                text: display
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
                onPopupVisibleChanged: {
                    // If we don't disable interactive when the popup is visible,
                    // moving the mouse flicks the view!
                    wrapper.ListView.view.interactive = !popupVisible;
                }
                currentIndex: indexForData(session, sessionsModel.key)
            }

            MouseArea {
                anchors.fill: parent
                enabled: !isCurrent
                onClicked: wrapper.ListView.view.currentIndex = index;
            }

            function startLogin() {
                var session = sessionCombo.itemData(sessionCombo.currentIndex);
                if (session == "") {
                    session = "default";
                }
                login(name, passwordInput.text, session);
            }

            states: State {
                name: "Current"
                PropertyChanges {
                    target: wrapper
                    currentOpacity: 1
                    outerPadding: 18
                    height: 80
                }
                PropertyChanges {
                    target: loginText
                    font.pointSize: 14
                }
            }

            transitions: Transition {
                to: "Current"
                reversible: true
                SequentialAnimation {
                    // Animate sizes first so that current-only items do not appear
                    // outside of the frame boundaries
                    NumberAnimation {
                        duration: 200
                        properties: "outerPadding,height,font.pointSize"
                    }
                    NumberAnimation {
                        duration: 100
                        properties: "currentOpacity"
                    }
                }
            }

            onIsCurrentChanged: {
                if (isCurrent) {
                    passwordInput.setFocus();
                    wrapper.state = "Current";
                } else {
                    wrapper.state = "";
                }
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
