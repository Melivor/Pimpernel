import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
ColorDialog {
    id: colorDialog
    title: qsTr("Please choose a color")
    //showAlphaChannel: false
    // x: (window.width - width) / 2
    // y: (window.height - height) / 2

    onAccepted: {
        //fbitem.setColor(colorDialog.color)
        value=colorDialog.color
        post()
        //root.parent.delegateModel.display=colorDialog.color
        close()
    }
    onRejected: {
        close()
    }
    //Component.onCompleted: console.log("I am: ", this)
}
