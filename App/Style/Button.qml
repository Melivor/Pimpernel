import QtQuick 2.12
import QtQuick.Controls 2.13

Button {
    id:button


    //font.bold: hovered?true:false
    // topInset:10
    // bottomInset: 10
    implicitHeight: 40
    implicitWidth: 100
    property color color: down?Qt.darker(horusTheme.backgroundColor,1.2):horusTheme.backgroundColor
    property color textColor: down?Qt.darker(horusTheme.textColor,1.2):highlighted?horusTheme.textLightColor:horusTheme.textColor
    property color borderColor:  hovered?horusTheme.highlightedBorderColor:horusTheme.borderColor
    contentItem:Item{
//        anchors.fill: button
//        anchors.bottomMargin: button.bottomInset
//        anchors.topMargin: button.topInset
//        anchors.leftMargin: button.leftInset
//        anchors.rightMargin: button.rightInset
        Text{
            text:button.text
            // font.bold: true
            // color:horusTheme.backgroundColor
            anchors.centerIn: parent
            font: button.font
            opacity: enabled ? 1.0 : 0.3
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            color: button.textColor

        }
        //
    }

    background:Rectangle{
//        anchors.fill: button
//        anchors.margins: padding
        color: button.color
        border.color: button.borderColor
        radius: horusTheme.baseRadius

    }
}
