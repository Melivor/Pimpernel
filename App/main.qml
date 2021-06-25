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
    title: qsTr("Pimpernel")
    property bool fullScreen: false
    property bool showExample: false
    property bool showBar: false
    //FocusScope{
      //  anchors.fill:parent


    Timer{
        id:exampleTimer
        repeat: false
        onTriggered: if(mouseArea.mouseY<(mainWindow.height-150)){
                          showExample=false
                     }
                     else{
                         exampleTimer.restart()
                     }

        interval: 2000
        running: false
    }
    Timer{
        id:barTimer
        repeat: false
        onTriggered: {
            if(fullScreen && mouseArea.mouseX<15){
                barTimer.restart()
            }
            else if(!fullScreen && mouseArea.mouseX<415){
                barTimer.restart()
            }
            else{
                showBar=false
            }
        }

        interval: 2000
        running: false
    }

    MouseArea{
        id:mouseArea
        anchors.fill:parent
        hoverEnabled: true
        onMouseYChanged:{
           // console.log(mouseY,"/",mainWindow.height-150)

            if(mouseY>(mainWindow.height-150)){
                exampleTimer.start()
                showExample=true;

            }
        }
        onMouseXChanged: {
            if(fullScreen && mouseX<15){
                showBar=true
                barTimer.start()
            }
            else if(!fullScreen && mouseX<415){
                showBar=true
                barTimer.start()
            }
            if(focus === false && mouseX > 415){
                focus=true
            }
        }

       // onClicked:scope.focus=true
        Keys.onPressed:{
            console.log("Key pressed");
            if(event.key === Qt.Key_F5){
                            if(visibility==Window.FullScreen){
                                console.log("F5 pressed")
                                visibility= Window.Maximized
                            }
                            else{
                                visibility=Window.FullScreen
                            }
                        }
            if(event.key === Qt.Key_Escape){
                visibility= Window.Maximized
            }
            if(event.key === Qt.Key_Right){
                listView.incrementCurrentIndex()
                loader.item.settings.setActiveSelection(listView.currentIndex, false)
                //console.log("right click, index ", listView.currentIndex, loader.item.settings.at(listView.currentIndex))
            }

            else if(event.key === Qt.Key_Left){
                listView.decrementCurrentIndex()
                loader.item.settings.setActiveSelection(listView.currentIndex, false)
            }
        }
    }

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

        //Keys.onLeftPressed: console.log("move left")
        //  sourceComponent: Component{PolygonPainter{}}
    }
    Button{
          anchors.top: loader.top
          anchors.bottom: loader.bottom

          anchors.left:loader.left
          opacity: 1
          width: 15
          text:fullScreen?">":"<"
          visible: showBar
          onClicked: {

              fullScreen=!fullScreen
          }

    }
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
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    Menu {
                        id: contextMenuObject
                        MenuItem { text: qsTr("Delete")
                            onTriggered: loader.item.settings.deleteProfilSettings(display)
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
    }

    Rectangle{
        id:rect
        visible: fullScreen?false:true
        width: fullScreen?0:400
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
    //}
}
