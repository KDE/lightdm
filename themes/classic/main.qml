import QtQuick 1.0

import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1


Item {
    width:640;
    height:480;
    
    //create a background
    Image {
        opacity: 1
        asynchronous: true;
        source: "/usr/share/wallpapers/Air/contents/images/1920x1200.jpg";
        fillMode: Image.PreserveAspectCrop
        anchors.fill: parent;
    }


    Item {
        id: dialog;

        anchors.centerIn: parent;

        width:416; //actually the same size as in KDM theme Horos
        height:262;

        Image {
            source:  "images/rectangle.png";
            anchors.fill: parent;
        }

        Label {
            anchors.top: parent.top;
            anchors.left: parent.left;
            anchors.margins: 20;
            text: "david-laptop"; //FIXME
            color: "#0b678c";
            font.pixelSize: 12;
            font.bold: true;
        }

        //buttons and stuff
        Row {
            id: bottomPanel;
            anchors.bottom: parent.bottom;
            anchors.left: parent.left;
            anchors.right: parent.right;
            anchors.margins: 20;

            spacing:5;


            ToolButton {
                id: sessionButton;
                iconSource: "system-log-out";

                property SelectionDialog selectionDialog;

                onClicked: {
                    if (!selectionDialog)
                            selectionDialog = selectionDialogComponent.createObject(sessionButton)
                        selectionDialog.open()
                }
            }

            ToolButton {
                iconSource: "system-shutdown";
            }

            Label {

                text: "Sun 31 Janurary 15:59";
                font.pixelSize: 8;
                color: "#0b678c";
                //FIXME make this work
                horizontalAlignment: Text.Right;
            }
        }

        Column {
            anchors.centerIn: parent;
            spacing:6;

            Row {
                Image {
                    source:"images/user_icon.png";
                    height: 44;
                    width: 44;
                    anchors.verticalCenter: parent.verticalCenter;
                }

                TextField {
                    id: usernameBox;
                    anchors.verticalCenter: parent.verticalCenter;
                    width: 200;
                    focus:  true;
                    KeyNavigation.tab: passwordBox;
                }
            }

            Row {
                Image {
                    source:"images/pw_icon.png";
                    height: 44;
                    width: 44;
                    anchors.verticalCenter: parent.verticalCenter;
                }

                TextField {
                    id: passwordBox;
                    anchors.verticalCenter: parent.verticalCenter;
                    width: usernameBox.width;
                    KeyNavigation.tab: usernameBox;
                }
            }




        }
    }


    Component {
        id: selectionDialogComponent

        SelectionDialog {
            id: sessionDialog;
            model:  ListModel {
                id: fruitModel

                ListElement {
                    modelData: "Apple"
                    cost: 2.45
                }
                ListElement {
                    modelData: "Orange"
                    cost: 3.25
                }
                ListElement {
                    modelData: "Banana"
                    cost: 1.95
                }
            }
        }
    }

}
