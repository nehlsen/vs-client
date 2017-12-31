import QtQuick 2.7
import org.nehlsen.vs 1.0
import org.nehlsen.vs.client 1.0

Rectangle {
    id: root
    anchors.fill: parent
    color: "#646464"
    state: "offline"

    property string myServer: "http://vesh.local"
    property string myUsername: "nehlsen"
    property string myPassword: "xc1337"
    property string myVenue: "1720105c-ed00-4743-815a-39b1fd3801ba"
    property string myCacheFolder: "/tmp"
    property int mySlideShowInterval: 1500

    VenueShot {
        id: vs
        server: myServer
        autoFetchPicturesEnabled: true
        cacheFolder: myCacheFolder
        interval: mySlideShowInterval
        onStatusChanged: {
            if (status == ClientStatus.Offline) {
                root.state = "offline"
            } else if (status == ClientStatus.Online) {
                root.state = "online"
            } else if (status == ClientStatus.Ready) {
                root.state = "ready"
            } else if (status == ClientStatus.Error) {
                root.state = "error"
            }
        }
        onCurrentPictureChanged: {
            display.source = currentPicture.localPath
        }
    }

    onStateChanged: {
        console.info("state changed " + state)
    }

    states: [
        State {
            name: "offline"
            StateChangeScript {
                script: vs.acquireToken(myUsername, myPassword)
            }
        },
        State {
            name: "online"
            StateChangeScript {
                script: vs.venueToken = myVenue
            }
        },
        State {
            name: "ready"
            StateChangeScript {
                script: {
                    display.visible = true
                    loadingLabel.visible = false
                }
            }
        },
        State {
            name: "error"
        }
    ]

    Text {
        id: loadingLabel
        anchors.fill: parent
        text: "Loading..."
    }
    Image {
        id: display
        anchors.fill: parent
        visible: false
        smooth: true
        fillMode: Image.PreserveAspectFit
        asynchronous: true
    }
}