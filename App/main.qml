import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import "Style"
import "qrc:/ItemView"
import Qt.labs.settings 1.1
import Qt.labs.platform 1.1
Window {
    id:mainWindow
    visible: true
    visibility: Window.Maximized
    title: qsTr("Generator")
    HorusTheme{
        id:horusTheme
    }
    Settings{
        id:settings
        property url folder:value("folder", StandardPaths.DocumentsLocation)
        property alias currentIndex: painterChoice.currentIndex
        property alias pngWidth: exportSettings.pngWidth
        property alias pngHeight: exportSettings.pngHeight
        property alias name: saveDialog.name
    }
    SaveDialog{
        id:saveDialog
        onAccepted:{
            loader.item.save(name)
        }

    }

    ExportDialog{
        id:exportDialog
        folder: settings.folder

       // y:mainWindow.height/2-height/2
        onAccepted:{
            exportSettings.open()
            settings.setValue("folder", folder)
            close()
        }
    }
    ExportSettingsDialog{
        id:exportSettings
        //x:parent.width/2-width/2
        //y:parent.height/2-height/2
        onAccepted: {
            loader.item.saveAsPng(exportDialog.fileUrl, pngWidth, pngHeight)
            close()
        }
    }


    Item{
        id:loader
        anchors.fill:parent
        anchors.leftMargin: rect.width
        property var item
        //  sourceComponent: Component{PolygonPainter{}}
    }
    Rectangle{
        height: 150
        anchors.bottom: parent.bottom
        anchors.right:parent.right
        anchors.left:rect.right
        ListView{
            width: parent.width
            height: parent.height
            clip:true
            delegate: Rectangle{
                implicitHeight: 150
                implicitWidth: visible?250:0
                enabled: visible

                visible: loader.item.settings.activeModel.name!==display && display!==loader.item.settings.activeModel.root
                Image{
                    id:image
                    anchors.fill:parent
                    source:parent.visible?loader.item.pngPath(display):""
                    //source:loader.item.pngPath()+display+".png"
                }

                Text{
                    anchors.centerIn: parent
                    text:display
//                    Component.onCompleted: {
//                        var str=loader.item.pngPath(display)
//                        console.log("Data: ",str, loader.item.settings.activeModel.root)
//                        image.source=str
//                    }
                }
                MouseArea{
                    anchors.fill:parent
                    onClicked: {

                        loader.item.settings.copyProfilSettings(display)
                    }
                }
            }
            model:loader.item.settings
            orientation: ListView.Horizontal
        }
    }

    Rectangle{
        id:rect
        width: 400
        anchors.left: parent.left
        height: parent.height
        color:horusTheme.backgroundColor
        border.color: horusTheme.borderColor
        opacity: 0.75


        ComboBox{
            id:painterChoice
            model:painterList
            textRole:"name"
            property int previousIndex: 0
            anchors.top:parent.top
            anchors.topMargin: 0
            width: parent.width
            Component.onCompleted: {

                painterList[currentIndex].init()
                loader.item=painterList[currentIndex].item()
                painterList[currentIndex].item().parent=loader
                loader.item.anchors.fill=loader
                settingsView.model=painterList[currentIndex].item().settings.activeModel
            }
            onCurrentIndexChanged: {
                painterList[previousIndex].release()
                painterList[currentIndex].init()
                //console.log("current is",painterList[currentIndex] )
                loader.item=painterList[currentIndex].item()
                painterList[currentIndex].item().parent=loader
                loader.item.anchors.fill=loader
                settingsView.model=painterList[currentIndex].item().settings.activeModel
            }
        }

        ItemListViewBySection{
            id:settingsView
            anchors.top:painterChoice.bottom
            anchors.topMargin: 20
            anchors.bottomMargin: 10
            anchors.bottom:saveButton.top
            width: parent.width
        }

        Button{
            id:saveButton
            anchors.bottom: exportButton.top
            width: parent.width
            text:"Save"
            onClicked:saveDialog.open()
        }

        Button{
            id:exportButton
            anchors.bottom: parent.bottom
            width: parent.width
            text:"Export..."
            onClicked:exportDialog.open()
        }
    }
}
