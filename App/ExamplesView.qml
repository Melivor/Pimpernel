import QtQuick 2.0
import QtQuick.Controls 2.15

Rectangle{
    id:exampleListView
    height: 0
    // visible: showExample
    anchors.bottom: parent.bottom
    anchors.right:parent.right
    anchors.left:rect.right
    NumberAnimation on height {
        running: showExample
        from: 0; to: 150
    }
    NumberAnimation on height {
        running: !showExample
        from: 150; to: 0
    }
    transitions: Transition {
        PropertyAnimation { property: "height"; duration: 1000 }
    }
    ListView{
        id:listView
        width: parent.width
        height: parent.height
        clip:true

        delegate: Rectangle{
            implicitHeight: 150
            implicitWidth: visible?250:0
            enabled: visible
            visible: display!==loader.item.settings.activeModel.root // && loader.item.settings.activeModel.name!==display
            Image{
                id:image
                anchors.fill:parent
                source:parent.visible?loader.item.pngPath(display):""
                //source:loader.item.pngPath()+display+".png"
            }

            Text{
                id:nameDisplay
                anchors.centerIn: parent
                text:display
                color:"black"
            }
            MouseArea{
                anchors.fill:parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                Menu {
                    id: contextMenuObject
                    MenuItem { text: qsTr("Delete")
                        onTriggered: {
                            console.log("Request delete", display,nameDisplay.text)
                            loader.item.settings.deleteProfilSettings(nameDisplay.text)
                        }
                    }
                }
                onClicked: {
                    if(mouse.button & Qt.RightButton) {
                        contextMenuObject.open(mouseX, mouseY)
                    }
                    else{
                        loader.item.settings.copyProfilSettings(display, false)
                        listView.currentIndex=index
                    }


                }

            }
        }
        model:loader.item.settings
        orientation: ListView.Horizontal
    }

    //Keys.onLeftPressed: console.log("move left")
    //  sourceComponent: Component{PolygonPainter{}}
}
