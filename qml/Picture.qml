import QtQuick 2.7

Rectangle {
    anchors.fill: parent

    Image {
        id: img
        anchors.fill: parent
        smooth: true
        fillMode: Image.PreserveAspectCrop
    }

    Rectangle {
        id: imgInfo

        color: "grey"
        opacity: 0.7
        width: 100
        height: parent.height*0.1
        border.color: "black"
        border.width: 2
        radius: 3

        anchors.bottom: img.bottom
        anchors.left: img.left
        anchors.bottomMargin: 15
        anchors.leftMargin: 15

        Text {
            id: infoAuthor
            anchors.fill: parent
            color: "black"
        }
    }

    function setPicture(venuePicture) {
//        console.info("setPicture: " + venuePicture.createdBy + " : " + venuePicture.createdAt)

        img.source = venuePicture.localPath
        infoAuthor.text = venuePicture.createdBy
        imgInfo.visible = !!venuePicture.createdBy
    }
}
