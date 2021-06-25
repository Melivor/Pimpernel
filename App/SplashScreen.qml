import QtQuick 2.12
import QtQuick.Window 2.2
import "qrc:/import"

Window {
    id: splash
    color: "transparent"
    title: qsTr("Pimpernel loading...")
    property string message: "loading..."
    property int textMargins: 20
    property color textColor: "#665d4a"
    property string textFontFamily: "Segoe Script"
    visible : true
    modality: Qt.ApplicationModal
    flags: Qt.SplashScreen
    width: 900
    height: 600
    Rectangle{
        id:rect
        anchors.fill:parent
        color:"#f8f2e4"
        border.color: "lightgrey"
        radius: 5
    }
    Image {
        id: splashImage
        source: "qrc:/splash.png"
        anchors.centerIn: parent
    }


    Text {
        id:titleBox
        text:"Pimpernel"
        color: textColor
        font.pointSize: 20
        font.family: textFontFamily
        anchors.bottom: textBox.top
        anchors.left: rect.left
        anchors.bottomMargin: 5
        anchors.leftMargin: textMargins
    }
    Text {
        id:textBox
        text:splash.message
        color: textColor
        font.family: textFontFamily
        anchors.bottom: rect.bottom
        anchors.left: rect.left
        anchors.bottomMargin: textMargins
        anchors.leftMargin: textMargins
    }
//    Text {
//        id:infoBox
//        text:"Author: Lucas Poupaud"
//        color: textColor
//        anchors.bottom: rect.bottom
//        anchors.right: rect.right
//        anchors.bottomMargin: textMargins
//        anchors.rightMargin: textMargins
//    }
}
