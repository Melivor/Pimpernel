import QtQuick 2.0
import QtQuick.Controls 2.12

Button{
    id:animateButton
    //anchors.bottom: saveButton.top
    property var model:parent.dmodel
    implicitWidth: 100
    implicitHeight: 30
   // width: parent.width
    text:"Animate"
    onClicked:animateDialog.open()
}
