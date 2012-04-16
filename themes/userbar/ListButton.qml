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

    PlasmaCore.FrameSvgItem {
        width: parent.width
        height: column.height
        anchors {
            margins: 6
        }
        imagePath: "widgets/button"
        prefix: "normal"

        PlasmaComponents.ButtonColumn {
            id: column
            spacing: 2 // Give some room

            Repeater {
                id: repeater
                PlasmaComponents.Label {
                    property string data: model[root.dataRole]
                    property bool isCurrent: root.currentIndex == model.index

                    text: model.display
                    font.bold: isCurrent && root.activeFocus
                    visible: isCurrent || root.activeFocus
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (root.activeFocus) {
                    var item = column.childAt(mouse.x, mouse.y);
                    var index = indexForItem(item);
                    if (index >= 0) {
                        root.currentIndex = index;
                    }
                } else {
                    root.forceActiveFocus();
                }
            }
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
}
