/*
 This file is part of LightDM-KDE.

 Copyright 2012 David Edmundson <kde@davidedmundson.co.uk>

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
import org.kde.lightdm 0.1 as LightDM

Item {
    id: manager
    property Item activeScreen
    property Component delegate
   
    Repeater {
        id: repeater
        model: LightDM.ScreensModel{}
        delegate : delegateItem
    }

    Component.onCompleted: {
        activeScreen = manager.children[0]
    }

    Component {
        id: delegateItem

        Item {
            x: geometry.x
            width: geometry.width
            y: geometry.y
            height: geometry.height

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    screenManager.activeScreen = parent
                }
            }

            Loader {
                sourceComponent: manager.delegate
                anchors.fill: parent
            }
        }
    }
}
