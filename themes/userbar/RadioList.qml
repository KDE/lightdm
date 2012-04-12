import QtQuick 1.0
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.core 0.1 as PlasmaCore

Item {
    id: root
    property int currentIndex: 0
    property string dataRole: ""
    property alias model: repeater.model
    width: childrenRect.width

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

    PlasmaComponents.ButtonColumn {
        id: column
        spacing: 2 // Give some room

        Repeater {
            id: repeater
            PlasmaComponents.RadioButton {
                property string data: model[root.dataRole]
                text: model.display
                checked: root.currentIndex == model.index
                onClicked: root.currentIndex = model.index
            }
        }
    }
}
