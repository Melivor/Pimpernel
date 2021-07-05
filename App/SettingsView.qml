import QtQuick 2.0
import QtQuick.Controls 2.12
import "qrc:/ItemView"
import Qt.labs.settings 1.1
import "qrc:/import"
Rectangle{
    id:rect
    visible: fullScreen?false:true
    property int unfoldWidth:400
    width: fullScreen?0:unfoldWidth
    anchors.left: parent.left
    height: parent.height
    color:HorusTheme.backgroundColor
    border.color: HorusTheme.borderColor
    opacity: 0.75


    Settings{
        id:settings
        property alias currentIndex: painterChoice.currentIndex

    }
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
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        anchors.bottom:startAnimationButton.top
        width: parent.width

    }



    Button{
        id:startAnimationButton
        anchors.bottom: saveButton.top
        width: parent.width
        text:animator.isRunning?"Stop animation":"Start animation"
        property var animator: loader.item.settings.getAnimator()
        onClicked: animator.isRunning?animator.stopAnimation():animator.startAnimation()
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
