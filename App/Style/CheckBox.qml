import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Shapes 1.12
import QtQuick.Layouts 1.12
CheckBox {
    id: control
    // leftPadding:0
    implicitWidth: 45
    contentItem: /*RowLayout{
        spacing: text.contentWidth>0?20:0
        opacity: enabled ? 1.0 : 0.3*/
        Text {
            id:text
            Layout.alignment: Qt.AlignVCenter
            text: control.text
            font: control.font
           // x:rect.x+rect.width
            color: horusTheme.textColor
            verticalAlignment: Text.AlignVCenter
            leftPadding: control.indicator.width + control.spacing
        }



        // leftPadding: control.indicator.width + control.spacing
   // }

    indicator: /*Rectangle {
        id:rect
        implicitWidth: 22
        implicitHeight: 22
        anchors.left:text.right
        anchors.leftMargin: 20
        y: parent.height / 2 - height / 2
        radius: 3
        border.color: control.down ? "#848484" : "#2E2E2E"*/
               Rectangle {
        Layout.alignment: Qt.AlignVCenter
        id:rect
       //  x:text.x+text.contentWidth
        implicitWidth: 22
        implicitHeight: 22
        //anchors.left:text.right
        //anchors.leftMargin: 20
        y: parent.height / 2 - height / 2
        radius: 3
        border.color: control.hovered ? horusTheme.highlightedBorderColor: horusTheme.accentColor

        Shape {
            // anchors.fill:rect
            visible: control.checked

            ShapePath{
                startX:rect.width*0.2
                startY:rect.height*0.5
                strokeWidth: 1.5
                strokeColor: horusTheme.accentColor
                joinStyle: ShapePath.RoundJoin
                capStyle: ShapePath.RoundCap
                PathLine{
                    // x:tabButton.x
                    //y:tabButton.y
                    relativeX: rect.width*0.4
                    relativeY: rect.height*0.4
                }
                PathLine{
                    relativeX: rect.width*0.6
                    relativeY: -rect.height*0.8
                }
            }

        }
    }

}
