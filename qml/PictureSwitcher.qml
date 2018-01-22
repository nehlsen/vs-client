import QtQuick 2.7
import "./" as VsModules

Flipable {
    id: flipable
    anchors.fill: parent

    front: VsModules.Picture {
        id: frontImage
    }
    back: VsModules.Picture {
        id: backImage
    }

    function setPicture(venuePicture) {
        console.info("setPicture: " + venuePicture.createdBy + " : " + venuePicture.createdAt)

        if (flipable.flipped) {
            frontImage.setPicture(venuePicture)
        } else {
            backImage.setPicture(venuePicture)
        }

        flipable.flipped = !flipable.flipped
    }

    property bool flipped: false

    transform: Rotation {
        id: rotation
        origin.x: flipable.width/2
        origin.y: flipable.height/2
        axis.x: 1; axis.y: 0; axis.z: 0     // set axis.y to 1 to rotate around y-axis
        angle: 0    // the default angle
    }

    states: State {
        name: "back"
        PropertyChanges { target: rotation; angle: 180 }
        when: flipable.flipped
    }

    transitions: Transition {
        NumberAnimation { target: rotation; property: "angle"; duration: 400 }
    }
}
