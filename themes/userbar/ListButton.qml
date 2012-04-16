/*
This file is part of LightDM-KDE.

Copyright 2012 Aurélien Gâteau <agateau@kde.org>

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
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore

FocusScope {
    id: root
    property int currentIndex: 0
    property string dataRole: ""
    property alias model: repeater.model

    function itemData(index) {
        var button = column.children[index];
        return button.data;
    }

    function itemText(index) {
        var button = column.children[index];
        return button.text;
    }

    function indexForData(data) {
        var index;
        for (index = 0; index < column.children.length; ++index) {
            if (column.children[index].data == data) {
                return index;
            }
        }
        return -1;
    }

    function indexForItem(item) {
        var index;
        for (index = 0; index < column.children.length; ++index) {
            if (column.children[index] == item) {
                return index;
            }
        }
        return -1;
    }

    states: [
        State {
            name: "closed"
        },
        State {
            name: "opened"
        }
    ]
    state: "closed"

    onActiveFocusChanged: {
        root.state = activeFocus ? "opened" : "closed";
    }

    PlasmaCore.FrameSvgItem {
        id: mainFrame
        anchors.horizontalCenter: parent.horizontalCenter
        width: column.width + margins.left + margins.right
        height: column.height + margins.top + margins.bottom
        imagePath: "widgets/button"
        prefix: "normal"
        clip: true

        PlasmaComponents.ButtonColumn {
            id: column
            spacing: 2 // Give some room

            x: parent.margins.left
            y: parent.margins.top
            width: childrenRect.width

            Repeater {
                id: repeater
                PlasmaComponents.Label {
                    property string data: model[root.dataRole]
                    property bool isCurrent: root.currentIndex == model.index

                    text: model.display
                    font.bold: isCurrent && root.state == "opened"
                    visible: isCurrent || root.state == "opened"
                }
            }
        }

        Behavior on width {
            NumberAnimation { duration: 100 }
        }
        Behavior on height {
            NumberAnimation { duration: 100 }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (root.state == "opened") {
                    var item = column.childAt(parent.margins.left, mouse.y);
                    var index = indexForItem(item);
                    if (index >= 0) {
                        root.currentIndex = index;
                        root.state = "closed";
                    }
                } else {
                    root.forceActiveFocus();
                    root.state = "opened";
                }
            }
        }
    }

    PlasmaCore.FrameSvgItem {
        id: focusFrame
        anchors {
            fill: mainFrame
            leftMargin: -margins.left
            topMargin: -margins.top
            rightMargin: -margins.right
            bottomMargin: -margins.bottom
        }
        width: parent.width
        height: column.height + margins.top + margins.bottom
        imagePath: "widgets/button"
        prefix: "hover"
        opacity: root.activeFocus ? 1 : 0
        Behavior on opacity {
            PropertyAnimation { duration: 100 }
        }
    }

    Keys.onUpPressed: {
        if (root.currentIndex > 0) {
            root.currentIndex--;
        }
    }
    Keys.onDownPressed: {
        if (root.currentIndex < column.children.length - 1) {
            root.currentIndex++;
        }
    }
    Keys.onReturnPressed: {
        root.state = root.state == "opened" ? "closed" : "opened";
    }
}
