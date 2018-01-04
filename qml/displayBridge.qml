import QtQuick 2.7
import Qt.labs.settings 1.0
import org.nehlsen.vs 1.0
import org.nehlsen.vs.client 1.0

Rectangle {
    id: root
    anchors.fill: parent
    color: "#646464"
    state: "offline"

    Settings {
        id: settings
        property alias url: vs.server
        property string username: "my-username"
        property string password: "my-password"
        property string venue: "my-venue-token"
        property alias cacheFolder: vs.cacheFolder
        property alias slideShowInterval: vs.interval
    }

    VenueShot {
        id: vs
//        server: myServer
        autoFetchPicturesEnabled: true
//        cacheFolder: myCacheFolder
//        interval: mySlideShowInterval
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
                script: vs.acquireToken(settings.username, settings.password)
            }
        },
        State {
            name: "online"
            StateChangeScript {
                script: vs.venueToken = settings.venue
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