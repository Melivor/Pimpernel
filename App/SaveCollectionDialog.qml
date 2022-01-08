import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.3

Dialog {
    property var wallpaperPainter
    modality: Qt.WindowModal
    Component.onCompleted: wallpaperPainter.saveCollectionDone.connect(close)
    Column{
        Text{
            text:wallpaperPainter.message
        }

        ProgressBar{
            from:0
            to: wallpaperPainter.rowCount
            value:wallpaperPainter.progress
        }
    }



}
