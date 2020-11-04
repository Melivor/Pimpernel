import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

Dialog {
    width:500
    height:350
    property alias name:textField.text
    standardButtons: Dialog.Ok | Dialog.Cancel

    RowLayout{
        width: 300
        anchors.centerIn: parent
        //spacing: 10
        Text{
            text:"Name: "
            Layout.alignment: Qt.AlignHCenter
        }

        TextField{
            id:textField
            Layout.alignment: Qt.AlignHCenter|Qt.AlignRight
            //onEditingFinished:pngWidth=Number(text)

        }
    }

}
