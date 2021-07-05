import Qt.labs.platform 1.1
import QtQuick 2.12
import QtQuick.Dialogs 1.3


FileDialog {

    id: fileDialog
    title: qsTr("Please choose a file")
     nameFilters: [ "Png files (*.png)", "Svg files (*.svg)" ]
     selectExisting: false

     //property var urls


    onRejected: {
        close();
    }
}
