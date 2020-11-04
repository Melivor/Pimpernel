import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

Dialog {
    width:500
    height:350
    property alias pngWidth: widthInput.text
    property alias pngHeight: heightInput.text
    standardButtons: Dialog.Ok | Dialog.Cancel
    ColumnLayout{
        anchors.centerIn: parent
        //width: parent.width
        RowLayout{
            Layout.preferredWidth: 300
            //spacing: 10
            Text{
                text:"Width (px): "
                Layout.alignment: Qt.AlignHCenter
            }

            TextField{
                id:widthInput
                Layout.alignment: Qt.AlignHCenter|Qt.AlignRight
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                validator: IntValidator{}
                //Component.onCompleted: text=pngWidth
                //onEditingFinished:pngWidth=Number(text)

            }
        }
        RowLayout{
            Layout.preferredWidth: 300
            //spacing: 10
            Text{
                text:"Height (px): "
                Layout.alignment: Qt.AlignHCenter
            }

            TextField{
                id:heightInput
                Layout.alignment:  Qt.AlignHCenter|Qt.AlignRight
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                validator: IntValidator{}
                //Component.onCompleted: text=pngHeight
                //onEditingFinished:pngHeight=Number(text)
//                onFocusChanged: if(!focus){
//                                    if(Number(text)!==pngHeight){
//                                        text=height
//                                    }
//                                }

            }
        }
    }
}
