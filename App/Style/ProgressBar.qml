import QtQuick 2.12
import QtQuick.Controls 2.13

ProgressBar {
    id:control
    property alias color: rect.color
    background: Rectangle {
            implicitWidth: 200
            implicitHeight: 6
            color: horusTheme.emptyColor
            radius: 3
        }
    contentItem: Item {
            implicitWidth: 200
            implicitHeight: 4

            Rectangle {
                id:rect
                width: control.visualPosition * parent.width
                height: parent.height
                radius: 2
                color: horusTheme.accentColor
            }
        }
}
