import QtQuick 1.0
import org.kde.qtextracomponents 0.1 as ExtraComponents

ExtraComponents.QIconItem {
    id: base
    signal clicked
    
    width: 16
    height: 16
    
    state: mousearea.containsMouse ? ExtraComponents.QIconItem.ActiveState : ExtraComponents.QIconItem.DefaultState
    
    MouseArea {
        id: mousearea
        anchors.fill: parent
        onClicked: base.clicked()
        hoverEnabled: true
    }
}
