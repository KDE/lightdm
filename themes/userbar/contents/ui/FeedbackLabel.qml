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
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore


PlasmaCore.FrameSvgItem {
    id: frame
    imagePath: "translucent/dialogs/background"
    opacity: 0
    property alias text: textItem.text
    property alias font: textItem.font

    property int padding: 18

    width: childrenRect.width + 2 * padding
    height: childrenRect.height + 2 * padding

    onTextChanged:  {
        if (text) {
            anim.start();
        } else {
            frame.opacity = 0;
        }
    }

    SequentialAnimation {
        id: anim
        PropertyAnimation {
            target: frame
            properties: "opacity"
            from: 0
            to: 1
            duration: 100
        }
        PauseAnimation {
            duration: 6000
        }
        PropertyAnimation {
            target: frame
            properties: "opacity"
            to: 0
            duration: 200
        }
    }

    Image {
        id: icon
        x: padding
        y: padding
        width: 22
        height: width
        source: "image://icon/dialog-error.png"
    }

    PlasmaComponents.Label {
        id: textItem
        anchors.left: icon.right
        anchors.leftMargin: 6
        anchors.verticalCenter: icon.verticalCenter
    }
}
