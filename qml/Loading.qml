import QtQuick 2.7

Rectangle {
    anchors.fill: parent
    color: parent.color

    Text {
        id: loadingLabel
        color: "white"
        anchors.centerIn: parent
        text: "Loading..."
        font.pointSize: 24
    }
}
