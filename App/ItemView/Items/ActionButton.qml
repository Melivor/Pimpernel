import QtQuick 2.0
import QtQuick.Controls 2.12
Button {
    text:parent.dmodel.name
    onClicked: parent.dmodel.display=1
}
