import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

Dialog {
    width:500
    height:350
    standardButtons: Dialog.Ok | Dialog.Cancel
    property alias model:listView.model

    ListView{
        id:listView
        anchors.fill:parent
        Component.onCompleted: console.log("Animate model: ", model)
        header: Row{
            height: 25
            TextField{
                text:"Parameter"
                readOnly: true
                width: 100
                height: 25
            }
            TextField{
                text:"From"
                readOnly: true
                width: 100
                height: 25
            }
            TextField{
                text:"To"
                readOnly: true
                width: 100
                height: 25
            }
            TextField{
                text:"Start"
                readOnly: true
                width: 100
                height: 25
            }
            TextField{
                text:"End"
                readOnly: true
                width: 100
                height: 25
            }
        }
        delegate: Row{
            height: 25
            TextField{
                text:name
                readOnly: true
                width: 100
                height: 25

            }
            TextField{
                text:min
                width: 100
                height: 25
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                onEditingFinished: min=Number(text)
            }
            TextField{
                text:max
                width: 100
                height: 25
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                onEditingFinished: max=Number(text)
            }
            TextField{
                text:from
                width: 100
                height: 25
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                onEditingFinished: from=Number(text)

            }
            TextField{
                text:to
                width: 100
                height: 25
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                onEditingFinished: to=Number(text)
            }
        }
    }
}
