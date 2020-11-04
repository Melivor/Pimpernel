import QtQuick 2.12
import QtQuick.Controls 2.13
ComboBox{
    id:comboBox
   // font.bold: hovered?true:false
    property alias horizontalAlignement: text.horizontalAlignment
    implicitWidth:160
    implicitHeight:40
    leftPadding: 10
    indicator.visible: enabled

    background: Rectangle{
        color: horusTheme.foregroundColor
        //implicitHeight: 50
        radius: horusTheme.baseRadius
        border.width: 0.5
        border.color: hovered?horusTheme.highlightedBorderColor:horusTheme.borderColor
    }
//    delegate: ItemDelegate {
//            width: comboBox.width
//            id:delegate
//            contentItem: Text {

//                text:model.display?model.display:""
//                color: horusTheme.textColor
//                font: comboBox.font
//                elide: Text.ElideRight
//                verticalAlignment: Text.AlignVCenter
//            }
//            highlighted: comboBox.highlightedIndex === index

//        }
    contentItem:Item{

        Text{
            id:text
            width: parent.width
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            //anchors.centerIn: parent
            text:comboBox.displayText
            color: horusTheme.textColor
        }
    }
    popup: Popup {
            y: comboBox.height +5
            width: comboBox.width
            implicitHeight: contentItem.implicitHeight
            padding: 1

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: comboBox.popup.visible ? comboBox.delegateModel : null
                currentIndex: comboBox.highlightedIndex

                ScrollIndicator.vertical: ScrollIndicator { }
            }

            background: Rectangle {
                border.color: horusTheme.borderColor
                color:horusTheme.foregroundColor
                radius: horusTheme.baseRadius
            }
        }
}
