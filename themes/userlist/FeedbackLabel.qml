import QtQuick 1.0
import org.kde.plasma.graphicswidgets 0.1 as PlasmaWidgets
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

    function showFeedback() {
        anim.start();
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

    Text {
        id: textItem
        anchors.left: icon.right
        anchors.leftMargin: 6
        anchors.verticalCenter: icon.verticalCenter
    }
}
