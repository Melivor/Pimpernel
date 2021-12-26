import QtQuick 2.0
import QtQuick.Controls 2.12
import "qrc:/ItemView"
import Qt.labs.settings 1.1
import "qrc:/import"
import painterList 1.0
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
        model:PainterList.generators
        textRole:"name"
        property int previousIndex: 0
        anchors.top:parent.top
        anchors.topMargin: 0
        width: parent.width
        function loadItem(){
            model[currentIndex].init()
            if(model.length>currentIndex){
                loader.item=model[currentIndex].item()
                model[currentIndex].item().parent=loader
                model[currentIndex].item().anchors.fill=loader
                settingsView.model=model[currentIndex].item().settings.activeModel
            }
        }

        Component.onCompleted: {
            loadItem()
        }
        onCurrentIndexChanged: {
            model[previousIndex].release()
            loadItem()

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
