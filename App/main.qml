import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Styles 1.4

import Qt.labs.settings 1.1
import Qt.labs.platform 1.1
import "qrc:/import"

ApplicationWindow {
    id:mainWindow
    visible: true
    visibility: Window.Maximized
    title: qsTr("Pimpernel")
    property bool fullScreen: false
    property bool showExample: false
    property bool showBar: false
    //FocusScope{
    //  anchors.fill:parent
    property bool darkTheme:false

    Pane{
        anchors.fill:parent
    }
    Component.onCompleted:if(darkTheme){
                              HorusTheme.accentColor="#be565b"

                          }

    function setDarkTheme(){
        palette.text="#ffffff"
        palette.windowText="#ffffff"
        palette.buttonText="#ffffff"
        palette.alternateBase="#474747"
        palette.window="#353535"
        palette.base="#292929"
        palette.mid="#202020"
        palette.button="#ff0000"
        palette.light="#0000ff"
        palette.midLight="#00ff00"
        palette.highLight="#387F6B"
        HorusTheme.accentColor="#be565b"
        darkTheme=true
    }
    function setLightTheme(){
        palette.text="#4b4b4b"
        palette.windowText="#4b4b4b"
        palette.buttonText="#4b4b4b"
        palette.alternateBase="#fafafa"
        palette.window="#f0f0f0"
        palette.base="#ffffff"
        palette.mid="#a0a0a0"
        palette.button="#f0f0f0"
        palette.light="#ffffff"
        palette.midLight="#e3e3e3"
        palette.highLight="#0078d7"

        HorusTheme.accentColor=Qt.hsla(0,0,0.15,0.95)
        darkTheme=false
    }

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
            else if(!fullScreen && mouseArea.mouseX<rect.width+15){
                barTimer.restart()
            }
            else{
                showBar=false
            }
        }

        interval: 2000
        running: false
    }
    Menu{
        id:menu
        MenuItem { text: darkTheme?"Set light theme":"Set dark theme"
        onTriggered:darkTheme?setLightTheme():setDarkTheme()

        }
    }

    MouseArea{
        id:mouseArea
        anchors.fill:parent
        hoverEnabled: true
        acceptedButtons: Qt.RightButton
        onClicked: {
            console.log("Right button clicked", mouseX, "/",rect.width+15 )
            if(!fullScreen && mouseX<rect.width+15){
                console.log("Right button clicked in rithh")
                menu.open(mouseX, mouseY)
            }

        }

        onMouseYChanged:{
            // console.log(mouseY,"/",mainWindow.height-150)

            if(mouseY>(mainWindow.height-150) && (!fullScreen && mouseX>rect.width+15)){
                exampleTimer.start()
                showExample=true;

            }
        }
        onMouseXChanged: {
            if(fullScreen && mouseX<15){
                showBar=true
                barTimer.start()
            }
            else if(!fullScreen && mouseX<rect.width+15){
                showBar=true
                barTimer.start()
            }
            if(focus === false && mouseX > rect.width+15){
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


    Settings{
        id:settings
        property url folder:value("folder", StandardPaths.DocumentsLocation)
        //property alias currentIndex: painterChoice.currentIndex
        property alias pngWidth: exportSettings.pngWidth
        property alias pngHeight: exportSettings.pngHeight
        property alias name: saveDialog.name
        property alias settingsViewWidth: rect.unfoldWidth
        property alias palette: mainWindow.palette
        property alias darkTheme: mainWindow.darkTheme
        //property alias accentColor: HorusTheme.accentColor
        // property alias name: value
    }
    AnimateDialog{
        id:animateDialog
        model:loader.item.settings.getAnimator()
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
        onAccepted:{
            exportSettings.open()
            settings.setValue("folder", folder)
            close()
        }
    }
    ExportSettingsDialog{
        id:exportSettings
        onAccepted: {
            loader.item.saveAsPicture(exportDialog.fileUrl, pngWidth, pngHeight)
            close()
        }
    }
    Item{
        id:loader
        anchors.fill:parent
        anchors.leftMargin: rect.width
        property var item
    }
    ExamplesView{
        id:exampleListView

    }



    SettingsView
    {
        id:rect

    }

    Button{
        anchors.top: loader.top
        anchors.bottom: loader.bottom

        anchors.left:rect.right
        opacity: 1
        width: 15
        text:fullScreen?">":"<"
        visible: showBar
        onClicked: {

            fullScreen=!fullScreen
        }
        MouseArea{
            anchors.left:parent.left
            anchors.top:parent.top
            anchors.bottom: parent.bottom
            width: 5
            cursorShape: Qt.SplitHCursor
            hoverEnabled: true
            property real mouseLastX
            onClicked: mouseLastX=mouseX
            onMouseXChanged: if(pressed){
                                 rect.unfoldWidth-=(mouseLastX-mouseX)                                //rect.width=rect.width-(mouseLastX-mouseX)
                                 //mouseLastX=mouseX
                             }
        }

    }
    //}



    //}
}
