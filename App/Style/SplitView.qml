import QtQuick 2.12
import QtQuick.Controls 2.13

SplitView {
    id:splitView
    property int handleTopMargin
    handle: Rectangle {
        implicitWidth: 4

        implicitHeight: 4
        Rectangle{
            y:handleTopMargin
            implicitWidth: splitView.orientation==Qt.Horizontal?parent.width/4:parent.width
            implicitHeight: splitView.orientation==Qt.Horizontal?parent.height-handleTopMargin:parent.height/4
            color: parent.SplitHandle.hovered ? parent.SplitHandle.pressed ?Qt.darker(horusTheme.borderColor, 1.1):Qt.darker(horusTheme.borderColor, 1.2)
            : horusTheme.borderColor
        }

            //visible: SplitHandle.hovered ?true:false

        }
}
