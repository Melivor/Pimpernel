import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
//import QtQuick.Dialogs 1.3

Dialog {
    property var wallpaperPainter
    modal: true//Qt.WindowModal
    visible: false
    onWallpaperPainterChanged: if(wallpaperPainter){
                                   wallpaperPainter.done.connect(close)
                               }
    contentItem: ColumnLayout{
        Text{
            Layout.alignment: Qt.AlignCenter
            text:wallpaperPainter.message
        }

        ProgressBar{
            Layout.alignment: Qt.AlignCenter
            width: 150
            height: 15
            from:0
            to: wallpaperPainter.maxProgress
            value:wallpaperPainter.progress
        }
    }



}
