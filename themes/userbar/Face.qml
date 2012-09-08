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
import org.kde.qtextracomponents 0.1 as QtExtraComponents

Item {
    id : base
    property alias source: image.source

    Image {
        id: image
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        smooth: true

        onStatusChanged: {
            if (image.status === Image.Error) {
                image.visible = false;
                icon.visible = true;
            }
        }
    }

    QtExtraComponents.QIconItem {
        id: icon
        anchors.fill: parent
        icon: "user-identity"
        visible: false
    }
}
