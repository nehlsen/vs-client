import QtQuick 2.4
import "./" as VS
//import "Picture.qml" as Picture

Rectangle {
    id: root
    anchors.fill: parent
    color: "#646464"

//    property bool forward: true

//    Text {
//        id: title
//        anchors.top: root.top
//        text: "VenueShot"
//    }

    Component {
        id: pictureDelegate
//        Item {
            Image {
                id: pdImage
                source: localPath
                width: view.width
                height: view.height
                smooth: true
        //        fillMode: Image.PreserveAspectCrop
                fillMode: Image.PreserveAspectFit
                asynchronous: true
                z: 80

                Rectangle {
                    anchors.bottom: pdImage.bottom
                    anchors.right: pdImage.right
                    z: 90
                    color: "lime"
                    Text {
                        z: 100
                        text: "Photo by:" + createdBy
                        color: "red"
                    }
                }
            }
//            Rectangle {
//                width: 50
//                height: 50
//                color: "black"
//
//                anchors.bottom: pdImage.bottom
//                anchors.right: pdImage.right
//                z: pdImage.z + 10
//            }
//        }
    }

    ListView {
        id: view
        anchors.fill: root
//        anchors.top: title.bottom
//        anchors.left: root.left; anchors.right: root.right
//        anchors.bottom: root.bottom
        model: venuePicturesModel
//        delegate: Image {
//            source: localPath
//            width: view.width
//            height: view.height
//            smooth: true
////            fillMode: Image.PreserveAspectCrop
//            fillMode: Image.PreserveAspectFit
//            asynchronous: true
//        }
        delegate: pictureDelegate
        orientation: ListView.Horizontal
        snapMode: ListView.SnapToItem
        cacheBuffer: width * 4
        highlightMoveDuration: 350
//        highlightMoveDuration: 0
    }

    Timer {
        interval: 1500; running: true; repeat: true
        onTriggered: {
//            if (view.currentIndex == view.count-1) {
//                forward = false
//            } else if (view.currentIndex == 0) {
//                forward = true
//            }
//
//            view.currentIndex = view.currentIndex + (forward ? 1 : -1)

            view.currentIndex = Math.random() * (view.count)
        }
    }
}
