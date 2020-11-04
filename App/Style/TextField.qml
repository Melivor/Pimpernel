import QtQuick 2.12
import QtQuick.Controls 2.13

TextField {
    id:root
color: acceptableInput?palette.text:"red"
//horizontalAlignment: Qt.AlignHCenter
implicitWidth:135
background: Rectangle {
    //visible: editable?true:false
    implicitWidth: root.implicitWidth+10 //
    implicitHeight: 30
    border.width: root.activeFocus ? 2 : 1
    color: root.palette.base
    border.color: root.activeFocus ? root.palette.highlight :root.palette.mid
    radius: horusTheme.baseRadius
}
}
