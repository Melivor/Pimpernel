import QtQuick 2.12
import QtQuick.Controls 2.13

Dialog {
    id:dialog
    width:500
    height:350
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    visible: true
    header:Rectangle{
        radius: horusTheme.baseRadius
        border.color: horusTheme.borderColor
       // width: parent.width
        // color:"red"
        Text{
            anchors.left: parent.left
            anchors.leftMargin: 25
            anchors.verticalCenter: parent.verticalCenter
            text:dialog.title
            font.bold: true
            color:horusTheme.textColor
        }
        height: 30
    }
    background:Rectangle{
        width:dialog.width
        height: dialog.height
        radius: horusTheme.baseRadius
        border.color: horusTheme.borderColor

    }

}
