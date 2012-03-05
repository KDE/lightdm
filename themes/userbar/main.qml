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
            var session = sessionCombo.itemData(sessionCombo.currentIndex);
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
        var username = usersList.currentItem.username;
        if (username == "*guest") {
            greeter.authenticateAsGuest();
        } else {
            greeter.authenticate(username);
        }
    }

    // Central item. This item is used to position the main items in the screen.
    Item {
        property int widgetHeight: 30

        anchors.horizontalCenter: parent.horizontalCenter
        /* Hack: we want to have 1/3 space above and 2/3 space below the main
         * items. We could use (parent.height - childrenRect.height) / 3 but
         * that causes the view to move down when selecting the guest session
         * because childrenRect.height decreases. Instead we compute a static
         * height for our items.
         */
        y: (parent.height -
            (usersList.height
            + fixedWidgetsColumn.anchors.topMargin
            + 3 * widgetHeight
            + 2 * fixedWidgetsColumn.spacing
            )) / 3
        width: parent.width

        ListView {
            id: usersList
            anchors.horizontalCenter: parent.horizontalCenter
            y: 0
            focus: true
            width: parent.width
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
        Column {
            id: fixedWidgetsColumn
            anchors.top: usersList.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            anchors.topMargin: 2 * padding
            spacing: padding

            LightDMPlasmaWidgets.LineEdit {
                id: passwordInput
                width: parent.width
                height: widgetHeight
                passwordMode: true
                clickMessage: i18n("Password")
                onReturnPressed: startLogin();
                visible: usersList.currentItem.username != "*guest"
            }

            LightDMPlasmaWidgets.ModelComboBox {
                id: sessionCombo
                width: parent.width
                height: widgetHeight
                model: sessionsModel
                currentIndex: indexForData(usersList.currentItem.usersession, sessionsModel.key)
            }

            PlasmaWidgets.PushButton {
                id: loginButton
                anchors.horizontalCenter: parent.horizontalCenter
                height: widgetHeight
                text: i18n("Login")
                onClicked: startLogin();
            }
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
