import QtQuick 2.7
import org.nehlsen.vs.picture 1.0

Image {
    id: display
    anchors.fill: parent
    visible: false
    smooth: true
    fillMode: Image.PreserveAspectFit
    asynchronous: true

    function setPicture(venuePicture) {
        console.info("setPicture: " + venuePicture.createdBy + ":" + venuePicture.createdAt)
        display.source = venuePicture.localPath
    }
}
