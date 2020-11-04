import QtQuick 2.12
import QtQuick.Controls 2.13

Slider {
    id:control

    handle: Rectangle{
        border.color: horusTheme.accentColor
        x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
        y: control.topPadding + control.visualPosition*(control.availableHeight - height)
        implicitHeight: 10
        implicitWidth:10
        radius: 5
    }
}
