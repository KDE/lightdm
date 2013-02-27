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
import QtQuick 1.1
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.lightdm 0.1 as LightDM


Item {
    id: screen

    LightDM.ScreenManager {
        id: screenManager
        delegate: Image {
             // default to keeping aspect ratio
            fillMode: config.readEntry("BackgroundKeepAspectRatio") == false ? Image.Stretch : Image.PreserveAspectCrop;
            //read from config, if there's no entry use plasma theme
            source: config.readEntry("Background") ? config.readEntry("Background"): theme.wallpaperPathForSize(width,height);
            smooth: true
        }
    }

    Item { //recreate active screen at a sibling level which we can anchor in.
        id: activeScreen
        x: screenManager.activeScreen.x
        y: screenManager.activeScreen.y
        width: screenManager.activeScreen.width
        height: screenManager.activeScreen.height
    }

    function doSessionSync() {
       var session = sessionButton.dataForIndex(sessionButton.currentIndex);
       if (session == "") {
           session = "default";
       }
       greeter.startSessionSync(session);
    }

    ParallelAnimation {
        id: loginAnimation
        NumberAnimation { target: welcomeLabel; property: "opacity"; to: 0; duration: 400; easing.type: Easing.InOutQuad }
        NumberAnimation { target: feedbackLabel; property: "opacity"; to: 0; duration: 400; easing.type: Easing.InOutQuad }
        NumberAnimation { target: usersList; property: "opacity"; to: 0; duration: 400; easing.type: Easing.InOutQuad }
        NumberAnimation { target: authManager; property: "opacity"; to: 0; duration: 400; easing.type: Easing.InOutQuad }
        NumberAnimation { target: sessionButton; property: "opacity"; to: 0; duration: 400; easing.type: Easing.InOutQuad }
        NumberAnimation { target: powerBar; property: "opacity"; to: 0; duration: 400; easing.type: Easing.InOutQuad }
        onCompleted: doSessionSync()
    }

    Component.onCompleted: {
        setTabOrder([usersList, authManager, sessionButton, powerBar]);
        usersList.forceActiveFocus();
    }

    function setTabOrder(lst) {
        var idx;
        var lastIdx = lst.length - 1;
        for (idx = 0; idx <= lastIdx; ++idx) {
            var item = lst[idx];
            item.KeyNavigation.backtab = lst[idx > 0 ? idx - 1 : lastIdx];
            item.KeyNavigation.tab = lst[idx < lastIdx ? idx + 1 : 0];
        }
    }

    PlasmaComponents.Label {
        visible: false
        id: welcomeLabel
        anchors.horizontalCenter: activeScreen.horizontalCenter
        anchors.top: activeScreen.top
        anchors.topMargin: 5
        font.pointSize: 14
        text: i18n("Welcome to %1", greeter.hostname);
    }

    FeedbackLabel {
        id: feedbackLabel
        anchors.horizontalCenter: activeScreen.horizontalCenter
        anchors.top: welcomeLabel.bottom
        anchors.topMargin: 5
        font.pointSize: 14
        text: authManager.feedbackText
    }

    property int userItemWidth: 120
    property int userItemHeight: 80
    property int userFaceSize: 64

    property int padding: 6

    Component {
        id: userDelegate

        Item {
            id: wrapper

            property bool isCurrent: ListView.isCurrentItem
            property bool activeFocus: ListView.view.activeFocus

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
                id: frameFocus
                anchors {
                    fill: frame
                    leftMargin: -margins.left
                    topMargin: -margins.top
                    bottomMargin: -margins.bottom
                    rightMargin: -margins.right
                }
                imagePath: "widgets/button"
                prefix: "hover"
                visible: wrapper.isCurrent
                opacity: wrapper.activeFocus ? 1 : 0
                Behavior on opacity {
                    NumberAnimation { duration: 100 }
                }
            }

            PlasmaCore.FrameSvgItem {
                id: frame
                anchors.centerIn: face
                width: face.width + padding * 2
                height: face.height + padding * 2
                imagePath: "widgets/lineedit"
                prefix: "base"
                enabledBorders: "NoBorder"
            }

            PlasmaCore.FrameSvgItem {
                id: frameHover
                anchors.fill: frame
                imagePath: "widgets/lineedit"
                prefix: "hover"
                opacity: (mouseArea.containsMouse && !(wrapper.isCurrent && wrapper.activeFocus)) ? 1 : 0
                Behavior on opacity {
                    NumberAnimation { duration: 100 }
                }
            }

            Face {
                id: face
                width: userFaceSize
                height: userFaceSize
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                source: model.imagePath
                loggedIn: model.loggedIn
            }

            Text {
                id: loginText
                anchors.top: face.bottom
                anchors.topMargin: padding * 1.5
                width: parent.width
                text: display
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
                maximumLineCount: 2
                wrapMode: Text.Wrap
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    wrapper.ListView.view.currentIndex = index;
                    wrapper.ListView.view.forceActiveFocus();
                }
            }
        }
    }

    function startSessionSync()
    {
        loginAnimation.start();
    }


    function indexForUserName(name) {
        var index;
        for (index = 0; index < usersList.count; ++index) {
            if (usersList.contentItem.children[index].username == name) {
                return index;
            }
        }
        return 0;
    }

    ListView {
        id: usersList
        anchors {
            horizontalCenter: activeScreen.horizontalCenter
            bottom: authManager.top
            bottomMargin: 24
        }
        width: activeScreen.width
        height: userItemHeight
        currentIndex: indexForUserName(greeter.lastLoggedInUser)
        model: LightDM.UsersModel{
                showGuest: greeter.hasGuestAccount
            }
        cacheBuffer: count * 80
        focus: true

        delegate: userDelegate

        orientation: ListView.Horizontal

        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: width / 2 - userItemWidth / 2
        preferredHighlightEnd: width / 2 + userItemWidth / 2

        //if the user presses down or enter, focus password
        //if user presses any normal key
        //copy that character pressed to the pasword box and force focus

        //can't use forwardTo as I want to switch focus. Also it doesn't work.
        Keys.onPressed: {
            if (event.key == Qt.Key_Down ||
                event.key == Qt.Key_Enter ||
                event.key == Qt.Key_Return) {
                authManager.currentItem.forceActiveFocus();
            } else if (event.key & Qt.Key_Escape) {
                //if special key, do nothing. Qt.Escape is 0x10000000 which happens to be a mask used for all special keys in Qt.
            } else {
                //this is perfectly likely to fail in the event that it is on a text prompt being shown.
                try {
                    authManager.currentItem.text += event.text;
                }
                catch(error) {
                    //do nothing
                }
                authManager.currentItem.forceActiveFocus();
            }
        }

        onCurrentItemChanged: authManager.startAuth(usersList.currentItem.username);
    }


    LightDM.AuthManager {
        id: authManager
        height: 30

        anchors {
            horizontalCenter: activeScreen.horizontalCenter
            bottom: activeScreen.verticalCenter
        }

        textFieldItem: PlasmaComponents.TextField {
            id: passwordInput
            width: 200
            height: parent.parent.height

            echoMode: TextInput.Password
            placeholderText: i18n("Password")
            onAccepted: greeter.respond(passwordInput.text)

            Keys.onEscapePressed: {
                usersList.forceActiveFocus()
            }

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
                onClicked: greeter.respond(passwordInput.text);
            }

        }

        loginButtonItem: PlasmaComponents.Button {
            width: userFaceSize + 2 * padding
            height: parent.parent.height

            iconSource: "go-jump-locationbar"
            text: i18n("Login")
            onClicked: startSessionSync();
        }
    }

    ListButton {
        id: sessionButton
        anchors {
            top: authManager.bottom
            topMargin: 24
            bottom: powerBar.top
            horizontalCenter: activeScreen.horizontalCenter
        }

        model: LightDM.SessionsModel{}
        dataRole: "key"
        currentIndex: {
            index = indexForData(usersList.currentItem.usersession)
            if (index >= 0) {
                return index;
            }
            index = indexForData(greeter.defaultSession)
            if (index >= 0) {
                return index;
            }
            return 0;
        }
    }

   PowerBar {
       id: powerBar
       anchors.bottom: activeScreen.bottom
       anchors.right: activeScreen.right
       enabledBorders: "LeftBorder|TopBorder"
   }
}
