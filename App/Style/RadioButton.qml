import QtQuick 2.12
import QtQuick.Controls 2.13

RadioButton {
    id:control
    indicator: Rectangle{
        implicitWidth: 20
        implicitHeight: implicitWidth
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: implicitWidth/2
        opacity: control.down?0.3:1
        border.color: horusTheme.accentColor
        Rectangle{
            width: 10
            height: width
            anchors.centerIn: parent
            radius: width/2
            opacity: control.down?0.3:1
            color: horusTheme.accentColor
            visible: control.checked
        }
    }
}
