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
    id: screen
    width: screenSize.width;
    height: screenSize.height;

    property string guestLogin: "*guest"

    Image {
        fillMode: Image.PreserveAspectCrop
        source: plasmaTheme.wallpaperPath()
        anchors.fill: parent
        visible: source != ""
    }

    Connections {
        target: greeter;

        onShowPrompt: {
            greeter.respond(passwordInput.text);
        }

        onAuthenticationComplete: {
            var session = sessionList.itemData(sessionList.currentIndex);
            if (session == "") {
                session = "default";
            }
            if(greeter.authenticated) {
                greeter.startSessionSync(session);
            } else {
                feedbackLabel.text = i18n("Sorry, incorrect password. Please try again.");
                feedbackLabel.showFeedback();
            }
        }
    }

    PlasmaComponents.Label {
        id: welcomeLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 5
        font.pointSize: 14
        text: i18n("Welcome to %1", greeter.hostname);
    }

    FeedbackLabel {
        id: feedbackLabel
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: welcomeLabel.bottom
        anchors.topMargin: 5
        font.pointSize: 14
    }

    property int userItemWidth: 150
    property int userItemHeight: 150
    property int userFaceSize: 48

    property int padding: 20

    Component {
        id: userDelegate

        Item {
            id: wrapper

            property bool isCurrent: ListView.isCurrentItem

            /* Expose current item info to the outer world. I can't find
             * another way to access this from outside the list. */
            property string username: model.name
            property string usersession: model.session

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
                imagePath: "opaque/dialogs/background"
                opacity: 0.618
            }

            Face {
                id: face
                anchors.bottom: loginText.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: padding * 1.5
                width: userFaceSize
                height: userFaceSize
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
        var username = usersList.currentItem.username;
        if (username == guestLogin) {
            greeter.authenticateAsGuest();
        } else {
            greeter.authenticate(username);
        }
    }

    ListView {
        id: usersList
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: loginButtonItem.top
            bottomMargin: 24
        }
        width: parent.width
        height: userItemHeight
        focus: true

        model: usersModel

        cacheBuffer: count * 80

        delegate: userDelegate

        orientation: ListView.Horizontal

        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: width / 2 - userItemWidth / 2
        preferredHighlightEnd: width / 2 + userItemWidth / 2

        KeyNavigation.backtab: loginButton
        KeyNavigation.tab: sessionFocusScope
    }

    Item {
        id: loginButtonItem
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.verticalCenter
        }
        height: 30

        /*PlasmaComponents.*/TextField {
            id: passwordInput
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            height: parent.height
            opacity: usersList.currentItem.username == guestLogin ? 0 : 1
            KeyNavigation.backtab: sessionFocusScope
            KeyNavigation.tab: sessionFocusScope

            echoMode: TextInput.Password
            placeholderText: i18n("Password")
            onAccepted: startLogin();

            PlasmaComponents.ToolButton {
                id: loginButton
                anchors {
                    right: parent.right
                    rightMargin: y
                    verticalCenter: parent.verticalCenter
                }
                width: implicitWidth
                height: width

                iconSource: "go-jump-locationbar"
                onClicked: startLogin();
            }

            Behavior on opacity {
                NumberAnimation { duration: 100 }
            }
        }

        PlasmaComponents.Button {
            id: guestLoginButton
            anchors.horizontalCenter: parent.horizontalCenter
            width: userFaceSize + 2 * padding
            height: parent.height
            opacity: 1 - passwordInput.opacity

            iconSource: loginButton.iconSource
            text: "Login"
            onClicked: startLogin();

            Behavior on opacity {
                NumberAnimation { duration: 100 }
            }
        }
    }

    FocusScope {
        id: sessionFocusScope

        KeyNavigation.backtab: passwordInput
        KeyNavigation.tab: usersList

        property bool showList: false

        anchors {
            top: loginButtonItem.bottom
            topMargin: 24
            bottom: powerBar.top
            horizontalCenter: parent.horizontalCenter
        }

        PlasmaComponents.Button {
            id: sessionButton
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
            focus: !parent.showList

            text: sessionList.itemText(sessionList.currentIndex) + " ⌄"

            onClicked: parent.showList = true

            opacity: parent.showList ? 0 : 1

            Behavior on opacity {
                NumberAnimation { duration: 100 }
            }
        }

        RadioList {
            id: sessionList
            focus: parent.showList
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottomMargin: 40
            }
            opacity: 1 - sessionButton.opacity

            model: sessionsModel
            dataRole: "key"
            currentIndex: indexForData(usersList.currentItem.usersession)
            onCurrentIndexChanged: {
                if (parent.showList) {
                    parent.showList = false;
                }
            }
        }

        function onShowListChanged() {
            if (showList) {
                sessionButton.opacity = 0;
            } else {
                sessionButton.opacity = 1;
            }
        }
    }

    // Bottom "Power" bar
    PlasmaCore.FrameSvgItem {
        id: powerBar
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: childrenRect.width + margins.left
        height: childrenRect.height + margins.top * 2
        imagePath: "translucent/widgets/panel-background"
        prefix: "south-mini"

        enabledBorders: "LeftBorder|TopBorder"

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
